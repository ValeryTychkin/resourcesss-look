cp /app/UNIXSocket/client.c /app/UNIXSocket/base_socket.c /app/Sources/CClientSocket/ \
&& cp /app/UNIXSocket/client.h /app/UNIXSocket/base_socket.h /app/Sources/CClientSocket/include/ \
&& swift run App serve --env production --hostname 0.0.0.0 --port 8080
