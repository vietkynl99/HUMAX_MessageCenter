# Humax-chat-app
## **1. Specification**
Message Center is a background service that:
- Receive messages from one or more applications that acts as Talkers
- Send the received messages to one or more applications that act as Listeners
- An application can register to Message Center to be a either Talker or Listener, or both of them
- Work in a Local Network

## **2. Requirements**
- No Login, No Database, No History
- Select a role when app starts
- Apps do not poll for incoming data
- No limited number of App connected to Message Center
