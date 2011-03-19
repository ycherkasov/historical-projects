#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>


using namespace std;

int main(int argc, char** argv) {
    struct sockaddr_in peer = {};
    int s;
    int rc;
    char buf [ 1 ];
    peer.sin_family = AF_INET;
    peer.sin_port = htons(7500);
    peer.sin_addr.s_addr = inet_addr("127.0.0.1");

    // client: socket-connect pattern

    // open inet socket for TCP connection (0 for TCP)
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
	perror("error open socket");
	exit(1);
    }

    // connect to just opened socket
    rc = connect(s, (struct sockaddr *) &peer, sizeof (peer));
    if (rc) {
	perror("error connect to socket");
	exit(1);
    }

    // send string data "1" to server (1 - length, 0 - flags)
    rc = send(s, "1", 1, 0);
    if (rc <= 0) {
	perror("error send data");
	exit(1);
    }

    // receive responce to buf[]
    rc = recv(s, buf, 1, 0);
    if (rc <= 0) {
	perror("error receive data");
	exit(1);
    }

    printf("%c\n", buf[ 0 ]);

    exit(0);
}
