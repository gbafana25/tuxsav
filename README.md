# savebox

## Connection flow
- C++ client sends json to django server endpoint (`/create`)
- server checks (w/ uuid/unique token) if user is logged in 
- document is created

- `/update` - resend whole contents or just additions/deletions?


## TODO:
- store cursor position (start at 0,0 when program starts, increment/decrement when moving)
- detect when file closes, then send final copy of file to server (swp file leaves artifacts behind sometimes)
