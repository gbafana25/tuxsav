# tuxsav 

Tuxsav allows files being edited in Vim to be automatically saved remotely.  

## Server

The server component is written in Django, and exposes an API that the client program communicates with.

## Client

The client is written in C++, and sends data to the server every four seconds 

## Document sync connection flow
- client reads vim `.swp` file
- posts to `/update` endpoint every four seconds
- server verifies key validity and updates document
- server sends success/failure response


## TODO:
- server checks if request method is correct


