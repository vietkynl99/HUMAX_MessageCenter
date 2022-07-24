#include "client.h"
#include "common.h"

char input_buf[BUFFER_SIZE] = "";

/**
 * @brief clear all character in stdin buffer
 * 
 * @param str input string
 */
void clearStdinBuffer(char *str, int _size_max)
{
    if(strlen(str) == _size_max - 1 && str[strlen(str) - 1] != '\n')
        while ((getchar()) != '\n');
    fflush(stdin);
}
/**
 * @brief get character from stdin
 *
 * @return char
 */
char getch(void)
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

/**
 * @brief print message at select line
 *
 * @param lines
 * @param msg
 */
void Client_printMessageInLine(int lines, char *msg)
{
    printf("\r"        // Moves cursor to beginning of line.
           "\033[1B"   // Move cursor down 1 lines.
           "\033[s"    // Save cursor position.
           "\033[%dA"  // Move cursor up d lines.
           "\x1b[2K"   // Clear entire line
           "%s"        // String to print.
           "\033[u"    // Restore cursor position.
           "\033[1A"   // Move cursor up d lines.
           "\033[%ldC" // Move cursor to end of lines.
           ,
           lines,
           msg,
           strlen(input_buf) + 9);
    fflush(stdin);
    fflush(stdout);
}

/**
 * @brief show buffer (user input temp)
 *
 */
void Client_showBuffer()
{
    char _buf[BUFFER_SIZE + 9];
    sprintf(_buf, "Message: %s", input_buf);
    Client_printMessageInLine(1, _buf);
}

/**
 * @brief insert new message in screen
 *
 * @param msg message
 */
void Client_printMessage(char *msg)
{
    printf("\n\n");
    Client_printMessageInLine(4, msg);
    Client_printMessageInLine(3, "_____________________");
    Client_printMessageInLine(2, "Enter ':q' to quit");
    Client_showBuffer();
}

/**
 * @brief check valid input message
 *
 * @param msg
 * @return int: return 1 if the message is valid (len>0, not all are spacebars)
 */
int Client_checkValidString(char *msg)
{
    for (int i = 0; i < strlen(msg); i++)
        if (isprint(msg[i]) && msg[i] != ' ' && msg[i] != '\t')
            return 1;
    return 0;
}

/**
 * @brief handle user input string
 *
 * @param ch input character
 * @param pfunc_process the function will be executed when the user enters the message
 * @param pfunc_exit The function will be executed when the program exits
 */
void Client_processInput(char ch, void (*pfunc_process)(), void (*pfunc_exit)())
{
    if (ch == '\n') // newline
    {
        if (!strcmp(input_buf, ":q"))
            pfunc_exit();
        if (Client_checkValidString(input_buf))
        {
            // Client_printMessage(input_buf);
            pfunc_process();
        }
        input_buf[0] = '\0';
    }
    else if (ch == 127) // backspace
    {
        input_buf[strlen(input_buf) - 1] = '\0';
    }
    else if (isprint(ch) && strlen(input_buf) < BUFFER_SIZE) // insert ch to buffer
    {
        int _len = strlen(input_buf);
        input_buf[_len] = ch;
        input_buf[_len + 1] = '\0';
    }
    // display entered characters
    Client_showBuffer();
}

/**
 * @brief function tto init client
 *
 * @return int: return file discription of client socket
 */
int Client_init(void)
{
    int _fd;
    _fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return _fd;
}

/**
 * @brief: function to connect server
 *
 * @return int: return file discription of client socket
 */
int Client_connect()
{
    struct sockaddr_in _saddr;
    int _ret_val, _count = 0, _fd_socket;
    char _buffer_IP[50];
    _fd_socket = Client_init();
    if (_fd_socket == -1)
    {
        fprintf(stderr, "socket failed [%s]\n", strerror(errno));
        return -1;
    }

    _saddr.sin_family = AF_INET;
    _saddr.sin_port = htons(SOCKET_PORT);
    while (1)
    {
        if (_count > 2)
        {
            printf("Close app!\n");
            close(_fd_socket);
            return -1;
        }
        else
        {
            printf("\nEnter server IP to connect: ");
            fgets(_buffer_IP, sizeof(_buffer_IP), stdin);
            //clear stdin buffer
            clearStdinBuffer(_buffer_IP, NAME_SIZE);
            //clear \n in the end of string
            if(strlen(_buffer_IP) > 0 && _buffer_IP[strlen(_buffer_IP) - 1] == '\n')
                _buffer_IP[strlen(_buffer_IP) - 1] = '\0';
            _saddr.sin_addr.s_addr = inet_addr(_buffer_IP);
            printf("trying to connect...\n");
            _ret_val = connect(_fd_socket, (struct sockaddr *)&_saddr, sizeof(struct sockaddr_in));
            if (_ret_val == -1)
            {
                printf("The IP address %s is incorrect or server is not running. Please try again!\n", _buffer_IP);
            }
            else
            {
                printf("The Server is now connected\n");
                break;
            }
        }
        _count++;
    }
    return _fd_socket;
}

/**
 * @brief function to choose role of user(as: talker, listener, both)
 *
 * @return int: return an interger corresponds chosen role: 1- Listener
 *                                                          2- Talker
 *                                                          3- Both
 */
int Client_chooseRole(void)
{
    char _buf[NAME_SIZE];
    printf("\nPlease choose your role: \n");
    while (1)
    {
        printf("1. Listener\n2. Talker\n3. Both\n");
        printf("Enter your choice: ");
        fgets(_buf, NAME_SIZE, stdin);
        //clear stdin buffer
        clearStdinBuffer(_buf, NAME_SIZE);
        //check valid
        if (strlen(_buf) > 2 || _buf[0] < '1' || _buf[0] > '3')
            printf("\nYour choice is invalid. Please choose again!\n");
        else
            break;
    }
    return _buf[0] - '0';
}

/**
 * @brief check valid of user's name
 * 
 * @param _pname user's name
 * @return int 
 */
int checkValidName(char *_pname)
{
    if(_pname[0] == '[' || _pname[0] == ' ' || _pname[strlen(_pname)-1] == ' ' ||  _pname[strlen(_pname)-1] == ']')
        return 0;
    for(int i=0; i<strlen(_pname); i++)
        if(_pname[i] != ' ' && !isalpha(_pname[i]) && !isdigit(_pname[i]))
            return 0;
    return 1;
}

/**
 * @brief get user's name
 * 
 * @param _pname user's name
 */
void Client_getUserName(char *_pname)
{
    while (1)
    {
        printf("\nEnter your name: ");
        fgets(_pname, NAME_SIZE, stdin);
        //clear stdin buffer
        //clearStdinBuffer(_pname, NAME_SIZE);
        //clear \n in the end of string
        if(strlen(_pname) > 0 && _pname[strlen(_pname) - 1] == '\n')
            _pname[strlen(_pname) - 1] = '\0';
        //check valid of user's name
        if (strlen(_pname) < 2)
            printf("\nYour name is too short. Please enter your name again!\n");
        else if (strlen(_pname) > 20)
            printf("\nYour name is too long. Please enter your name again!\n");
        else if(!checkValidName(_pname))
            printf("\nYour name is invalid. Please enter your name again!\n");
        else
            break;
    }
}