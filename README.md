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

## Note on reading from Vim swp files
- offset for start of data found in [this](https://github.com/adamhotep/misc-scripts/blob/master/ls.swp) script.
- look for Vim documentation that gives a better explanation of the file data starting point
- keep experimenting with offset index
- in future: could potentially store some of the metadata on the server

## TODO:
- update CSS to html files
- rewwrite client in C