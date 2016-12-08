mv /dev/video15 /dev/video0
#./mjpg_streamer  -o "/mjpg/output_http.so -w ./www"
mjpg_streamer -i "/mjpg/input_uvc.so" -o "/mjpg/output_http.so -w /www"
