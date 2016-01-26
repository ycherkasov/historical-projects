/* include includes */
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <errno.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../skel.h"

using std::string;

char *program_name;

/* end includes */

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

    struct servent *sp = 0;
    struct hostent *hp = 0;
    char *endptr = 0;
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

/* server - place holder for server */
static void server(SOCKET s, struct sockaddr_in *peerp) {
}

/* main - TCP setup, listen, and accept */
int main(int argc, char **argv) {

    // server
    struct sockaddr_in local = {};

    //client
    struct sockaddr_in peer = {};

    std::string hname; // hostname (address)
    std::string sname; // port number (service name)

    socklen_t peerlen;
    SOCKET s;
    const int on = 1;

    INIT();

    // usage:
    // tcpserver "127.0.0.1"
    // tcpserver "127.0.0.1" "21"
    if (argc == 2) {
	sname = "0";
	sname = argv[ 1 ];
    }
    else {
	hname = argv[ 1 ];
	sname = argv[ 2 ];
    }

    // transform string parameters to server data
    set_address(hname.c_str(), sname.c_str(), &local, "tcp");

    // open socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (!isvalidsock(s))
	error(1, errno, "socket call failed");

    // set option "reuse address"
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof ( on)))
	error(1, errno, "setsockopt failed");

    // bind socket to address:port
    if ( bind(s, (struct sockaddr *) &local, sizeof (local)) )
	error(1, errno, "bind failed");

    // start listening
    if (listen(s, NLISTEN)){
	error(1, errno, "listen failed");
    }

    // accept incoming messages in loop
    do {
	peerlen = sizeof ( peer);
	SOCKET s1 = accept(s, (struct sockaddr *) &peer, &peerlen);
	if (!isvalidsock(s1))
	    error(1, errno, "accept failed");
	// custom
	server(s1, &peer);
	CLOSE(s1);
    } while (1);

    EXIT(0);
}

