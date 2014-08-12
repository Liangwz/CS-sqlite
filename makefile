server:server.c
	gcc -Wall -g server.c -o server
clean:
	rm -f client
	rm -f server

client:
	gcc -Wall -g client.c sqlite_main.c -o client -lpthread -lsqlite3
clcli:
	rm -f client

arm_client:
	arm-none-linux-gnueabi-gcc -Wall  -g  client.c -o arm_client  -lpthread

arm_server:
	arm-none-linux-gnueabi-gcc -Wall  -g  server.c -o arm_server 

sql:sqlite_main.c
	gcc -Wall -g sqlite_main.c -o sql -lsqlite3

