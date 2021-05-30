#!/bin/bash

set -e

source /conf.env

( cd /src && ./configure.sh Release && ./build.sh ) || exit 1

( cd /src && echo "Starting Webserv..." && ./run.sh "/nginx.conf" || { echo "EXITED WITH ERROR $?"; exit 1; } ) &
pid=$!; sleep 2

curl localhost:$port &1>dev/null || { echo "Server is not up!"; exit 3; }

cd / && echo "Running ubuntu_tester..." && echo -n "\n\n\n\n" | ./ubuntu_tester http://localhost:$port 2>&1 | tee output_log | tail -n +29 || exit 4

grep "FATAL" output_log &>/dev/null && exit 5

kill -0 $pid || { echo "Webserv crashed during or before testing..."; exit 6; } 
