make:
	gcc newclient.c -l pthread -l curses -o client
	gcc hashmap.c -c
	gcc selectServer.c hashmap.o -o server