#include "proxy.h"

int resolve_hostname(char *hostname, char *ipstr, int size) {
	/* credit: http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#getaddrinfo */

	struct addrinfo hints, *res, *p;
    int status;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET; // AF_INET or AF_INET6 to force version
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(hostname, NULL, &hints, &res)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 0;
    }
 //   printf("IP address for %s:\n\n", hostname);

    for(p = res; p != NULL; p = p->ai_next) {
        void *addr;
        char *ipver;
        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (p->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } 
        // convert the IP to a string and print it:
        inet_ntop(p->ai_family, addr, ipstr, size);
 //       printf("Resolved %s\n", ip);
    	break;
    }

    freeaddrinfo(res); // free the linked list 

    return 1;
}