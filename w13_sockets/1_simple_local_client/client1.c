#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


int main(void){
	int sockfd;
	int len;
	struct sockaddr_un address;
	int result;
	char ch = 'A';

	// create a socket for the client
	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

	// name the socket as agreed with the server
	address.sun_family = AF_UNIX;	
	strcpy(address.sun_path, "server_socket");
	len = sizeof(address);

	// connect to the server's socket
	result = connect(sockfd, (struct sockaddr *) &address, len);

	if( result == -1 ) {
		perror("oops: client1");
		exit(1);
	}

	// read and write via sockfd
	write(sockfd, &ch, 1);
	read(sockfd, &ch, 1);
	printf("[client] msg from server = %c\n", ch);
	close(sockfd);

	return 0;
}
