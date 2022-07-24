# **Client**
## 1. Functional
- able to connect to the server
- no login
- able to select role when app starts
- has log out option
- 3 roles:
  - listener:
    - only receive messages from the time of connecting to the server successfully. 
    - Not able to poll server for incoming data
  - talker: only send messages to server
  - listener and talker: do both things
- UI:
  - use command line
  - received message section and input message section are seperated
- run on Linux
## 2. Non-functional


# **Server**
## 1. Functional
- able to receive from and send message to clients
- no limited number of clients connected to
- no database, no history
- be aware of new client
- use multi-thread
## 2. Non-functional


