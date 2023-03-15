# savebox

## Connection flow
- C++ client sends json to django server endpoint (`/create`)
- server checks (w/ uuid/unique token) if user is logged in 
- document is created

- `/update` - resend whole contents or just additions/deletions?


## TODO:
- store cursor position (start at 0,0 when program starts, increment/decrement when moving)
- for reading vim swp, require shebang at beginning of file to find start of data
