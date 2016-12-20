#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <time.h> // clock()
#include <unistd.h> // close()
#include <pthread.h>

void wait_on_clients();
void handle_requests(int*);
char *get_data_from_server(char *request, int req_length, int sock);
int server_connect(char *ip);

void* SocketHandler(void*);
int resolve_hostname(char *hostname, char *ipstr, int size);
