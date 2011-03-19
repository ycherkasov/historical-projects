#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <cstdlib>

int main(int argc, char** argv) {
    struct sockaddr_in local;
    int s;
    int s1;
    int rc;
    char buf [ 1 ];
    local.sin_family = AF_INET;
    local.sin_port = htons(7500);
    local.sin_addr.s_addr = htonl(INADDR_ANY);

    // server: socket-bind-listen pattern
    // TCP store information about packet order (TCP window)
    // if packets 1-3 have been received, TCP window is 4-12

    // open inet socket for TCP connection (0 for TCP)
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	perror(" socket");
	exit(1);
    }

    // bind to just opened socket for listening
    rc = bind(s, (struct sockaddr *) &local, sizeof ( local));
    if (rc < 0) {
	perror("bind");
	exit(1);
    }

    // start listening (5 is maximum waited connections)
    rc = listen(s, 5);
    if (rc) {
	perror("listen");
	exit(1);
    }

    // accetp first connection from incoming queue
    // (params 2&3 are client information fields)
    s1 = accept(s, NULL, NULL);
    if (s1 < 0) {
	perror("accept");
	exit(1);
    }

    // receive and print data from client
    rc = recv(s1, buf, 1, 0);
    if (rc <= 0) {
	perror("recv");
	exit(1);
    }
    printf("%c\n", buf[ 0 ]);

    // send response "2", 1 is length, 0 is flags
    rc = send(s1, "2", 1, 0);
    if (rc <= 0){
	perror("send");
	exit(1);
    }

    exit(0);

}
