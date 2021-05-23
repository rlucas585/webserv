#!/bin/sh

# If ran with argument, the arguments are passed on to Webserv.run
cd build && { [ $# -eq 0 ] && { ./WebServ.run; exit $?; } || ./WebServ.run "$*"; }
