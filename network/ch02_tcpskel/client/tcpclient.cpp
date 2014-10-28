#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../skel.h"
#include <string>

char *program_name;

/* error - print a diagnostic and optionally exit */
void error(int status, int err, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    fprintf(stderr, "%s: ", program_name);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    if (err)
	fprintf(stderr, ": %s (%d)\n", strerror(err), err);
    if (status)
	EXIT(status);
}

/* set_address - fill in a sockaddr_in structure */
static void set_address( const char * hname, const char * sname,
	struct sockaddr_in *sap, const char *protocol) {

    struct servent *sp;
    struct hostent *hp;
    char *endptr;
    short port;

    // set inet protocol
    bzero(sap, sizeof ( *sap));
    sap->sin_family = AF_INET;

    // resolve server address or hostname
    if ( strcmp(hname, "") ) {
	// if it isn't IP-address, resolve name
	if (!inet_aton(hname, &sap->sin_addr)) {
	    hp = gethostbyname(hname);
	    if (hp == NULL)
		error(1, 0, "unknown host: %s\n", hname);
	    sap->sin_addr = *(struct in_addr *) hp->h_addr;
	}
    }
    // or listen all interfaces
    else{
	sap->sin_addr.s_addr = htonl(INADDR_ANY);
    }

    // set port number from string number
    port = strtol(sname, &endptr, 0);
    if (*endptr == '\0'){
	sap->sin_port = htons(port);
    }
    // or from service name
    else {
	sp = getservbyname(sname, protocol);
	if (sp == NULL)
	    error(1, 0, "unknown service: %s\n", sname);

	// if unable, use port order
	sap->sin_port = sp->s_port;
    }
}

/* client - place holder for the client code */
void client(SOCKET s, struct sockaddr_in *peerp) {
    
}

/* main - connect to the server */
int main(int argc, char **argv) {
    
    // connection
    struct sockaddr_in peer = {};

    INIT();
    
    std::string hname; // hostname (address)
    std::string sname; // port number (service name)
    // usage:
    // tcpserver "127.0.0.1"
    // tcpserver "127.0.0.1" "21"
    if (argc == 2) {
	hname = argv[ 1 ];
	sname = argv[ 2 ];
    }
    else{
	printf("usage: tcpclient host port");
	EXIT(0);
    }

    // transform string parameters to client data
    set_address(hname.c_str(), sname.c_str(), &peer, "tcp");

    // create client socket
    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (!isvalidsock(s)){
	error(1, errno, "socket call failed");
    }
	
    // connect client socket to server
    if (connect(s, (struct sockaddr *) &peer, sizeof ( peer))){
	error(1, errno, "connect failed");
    }

    // custom client handler
    client(s, &peer);

    EXIT(0);
}

