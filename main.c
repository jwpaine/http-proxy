#include "proxy.h"


int local_sock; // comm_sock socket used for communicating to/from client
struct sockaddr_in my_addr, client_addr;
pthread_t thread;

int main() {

	char ip[16];
	int max_clients = 1000;
	int port;

	printf("Enter TCP Port for self (server): ");
    scanf("%d", &port);
    getchar();
    printf("Enter IP to bind to, or * for all: ");
    fgets(ip, sizeof(ip), stdin);
    strtok(ip, "\n"); // strip newline character 

	local_sock = socket(AF_INET, SOCK_STREAM, 0);
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(port);
    /* does user want to bind to any address? */
    
    if (ip[0] == '*') {
    	my_addr.sin_addr.s_addr = htons(INADDR_ANY); // accept connection from any address 
    } else {
    	inet_pton(AF_INET, ip , &(my_addr.sin_addr)) ;
    }
    /* make sure we can bind socket to ip/port */
    if (bind(local_sock, (struct sockaddr*) &my_addr, sizeof(my_addr)) < 0) { 
    	perror("Error Binding"); exit(1); 
    } 

    printf("TCP Server Bound to %s:%d\n", inet_ntoa(my_addr.sin_addr), ntohs(my_addr.sin_port));

	/* start listening for an incomming connection (max 1 connection) from client */
    listen(local_sock, max_clients); 

    wait_on_clients();
    

}
    
void wait_on_clients() {

	printf("Waiting on request from client\n");

	int len = sizeof(my_addr);
	int *client_sock;

	while(1) {

        client_sock = (int*)malloc(sizeof(int));
    	*client_sock = accept(local_sock, (struct sockaddr *)&client_addr, &len); // accept a connection. we will read/write to comm_sock
    	if (*client_sock < 0) { perror("Error on accept\n"); exit(1); }

        printf("Client connected: %s\n",inet_ntoa(client_addr.sin_addr));
        pthread_create(&thread, 0, &SocketHandler, (void*)client_sock );
        pthread_detach(thread);

        }

        printf("END\n");
}


		
		


    	









