Program: main.c
	gcc -o proxy-server -g main.c socketHandler.c resolve_hostname.c -lpthread
