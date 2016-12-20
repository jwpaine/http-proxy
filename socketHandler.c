#include "proxy.h"

void* SocketHandler(void* p) {
	int *socket = (int*)p;
	printf("Socket Handler Initiated\n");


	handle_requests(socket);

	close(*socket);
	free(socket);

	return NULL;

	}

/* return socket */
int server_connect(char *ip) {
    /* setup and connect to requested server */
    printf("IP of external server: %s\n", ip);

    struct sockaddr_in server;
    int server_sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family=AF_INET;
    server.sin_port=htons(80);
    inet_pton (AF_INET, ip, &(server.sin_addr)); // setup address information for server struct

    if (connect(server_sock,(struct sockaddr *)&server,sizeof(server)) < 0){
        perror("client connect failed. Error");
        return -1;
    } 

    printf("Connected!\n");

    return server_sock;

}

void handle_requests(int *client_sock) {
	/* wait for client to send http request */
	char message[65535];
	char response[4096];

	time_t last_response;


	while(1) {

			bzero(message,sizeof(message)); // clear message buffer
			recv(*client_sock, &message, sizeof(message), 0); // receive message from comm_sock
			
			if (strncmp(message, "GET", 3) == 0) {
                break;
            }
        }
				/* get server address */
				int i = 11;
				int j = 11;
				while(message[j] != '/') {
					j++;
				}
				char *server_address = malloc(sizeof(char)*(j-i));
				memcpy(server_address, &message[i], j-i);
				printf("Server Address: %s\n", server_address);

				/* build filename + http version */
		//		char *server_request = malloc(sizeof(char)*sizeof(&message[j])+5);
				char server_request[2000];
				strcpy(server_request, "GET ");
				char *c = &server_request[4];
				i = j;
				while(message[j] != '\0') {
					*c = message[j];
					*c++;
					j++;

				}

				int request_len = j-i+4;
				/* setup connection information for external server */
			//	char *c = &message[j];
				
				/* resolve hostname */
				char ipstr[INET6_ADDRSTRLEN];
				
				if (!resolve_hostname(server_address, ipstr, INET6_ADDRSTRLEN)) {
					printf("Unknown server address, closing connection\n");
					return;
				}
				
                int server_sock = server_connect(ipstr);

                if (server_sock < 0) {

                	printf("Error connecting to external server\n");

                }

                if (send(server_sock, &server_request, strlen(server_request), 0) < 0) { // send ACK to server
                    perror("Failed to deliver request to server\n");
                } 

    		//	printf("Sent:%s\n", server_request);
    			/* get server reply */
    			bzero(response, sizeof(response));
    			int response_length;
    			while(1) { // receive message from comm_sock) { 

	                double elapsed = (clock() - last_response)/((double)CLOCKS_PER_SEC);

                    if (elapsed > 10) {
                    	printf("Server hasn't responded in %.3f, closing\n", elapsed);
                    	break;
                    }

					if (recv(server_sock, response, sizeof(response), 0) > 0) {

						last_response = clock(); // update last_response with current cpu time
					
                		if ( (send(*client_sock, &response, sizeof(response), 0)) < 0) {
                    		perror("Failed to deliver data back to client\n");
                        	break;
                     	} 

                     	printf("Sent data back to client\n");
                    	bzero(response, sizeof(response));
                    }



    			} 

    			printf("closing connection\n");
    			close(server_sock);

}