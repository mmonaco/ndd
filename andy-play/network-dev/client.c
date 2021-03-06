/* Andy Sayler
 * client.c
 * Creates 11/15/11
 * Basic dev client
 */

/* Original base source from:
 * http://www.linuxhowtos.org/C_C++/socket.htm
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAXCOMMANDSIZE 256

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[])
{
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[256];
	char cmdbuffer[MAXCOMMANDSIZE];

	int run = 1;

	if (argc < 3) {
		fprintf(stderr, "usage %s hostname port\n", argv[0]);
		exit(0);
	}

	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}

	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr,
	      (char *) &serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);

	if (connect(sockfd, (struct sockaddr *) &serv_addr,
		    sizeof(serv_addr)) < 0) {
		error("ERROR connecting");
	}

	while(run){

		/* Zero Buffers */
		bzero(cmdbuffer, sizeof(cmdbuffer));
		bzero(buffer, sizeof(buffer));

		/* Get Input */
		printf("Command:");
		fgets(cmdbuffer, sizeof(cmdbuffer), stdin);

		n = write(sockfd, cmdbuffer, strnlen(cmdbuffer,
							sizeof(cmdbuffer)));
		if (n < 0) {
			error("ERROR writing to socket");
		}
		
		n = read(sockfd, buffer, sizeof(buffer));
		if (n < 0) {
			error("ERROR reading from socket");
		}

		switch(buffer[0]){
		case 'q':
		{
			run = 0;
			break;
		}
		default:
		{
			break;
		}
		}

		printf("%s\n", buffer);
	}

	close(sockfd);
	return 0;
}
