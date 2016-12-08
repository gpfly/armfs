#! /bin/sh

export LD_LIBRARY_PATH=/mjpg
./mjpg_streamer   -o "./output_http.so -w ./www"&
