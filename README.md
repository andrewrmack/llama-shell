# llama-shell
llama-shell is a (currently) very minimal shell for Linux, and possibly other
UNIXes, although I have only tested on Linux. It is, as of yet, a work in
progres, so you probably shouldn't replace bash just yet, but it is steadily
developing and will hopefully be a full-featured, POSIX compliant shell in the
near future.

##Update:
llama-shell now handles signals correctly :). SIGINT and SIGQUIT both function
exactly as expeded, terminating the process in question. However, as support
for background processes has not yet been implemented, SIGTSTP, SIGTTIN, and
SIGTTOU will also terminate the process and print a message. This will be
addressed when background processes are implemented

##TO-DO List:
- [x] Implement signal handling for SIGINT, SIGQUIT, and SIGTSTP
- [ ] Allow for process backgrounding
- [ ] Implement I/O redirection (i.e. |, >, >>, etc.)
- [ ] Make a config file
- [ ] Tab completion
- [ ] History
- [ ] Other cool stuff?

##Installation
To install llama shell, just clone the repository to a directory of your choice
and run the make command:
```
$ make
```
If you want to formally install it (it's not really a full featured shell yet
so I'd avise against it), run as root:
```
$ make install
```
