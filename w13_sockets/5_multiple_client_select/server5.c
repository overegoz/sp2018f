#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>


int server_sockfd;

static void sigint_handler(int signo){
	printf("Prepping to exit...\n");
	close(server_sockfd);
	fflush(stdout);
	exit(EXIT_SUCCESS);
}

int main(void){
	int client_sockfd;
	int server_len, client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	int result;
	fd_set readfds, testfds;

	// register SIGINT handler for clean-up process
	// kill the server by CTRL^C
	if( signal(SIGINT, sigint_handler) == SIG_ERR ){
		fprintf(stderr, "Cannot handle SIGINT!!!\n");
		exit(EXIT_FAILURE);
	}

	// creates an unnamed socket for the server
	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

	// name & configure the server socket
	// - used 'inet_addr()' to convert text IP addr to the right form
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // accept from any IP
	server_address.sin_port = htons(9734); // listening on port 9734
	server_len = sizeof(server_address);

	bind(server_sockfd, (struct sockaddr *)&server_address, server_len);
	
	// create a connection queue and wait for client
	listen(server_sockfd, 5);
	
	// ignore SIGCHLD signals in order not to create zombies
	signal(SIGCHLD, SIG_IGN);

	FD_ZERO(&readfds);
	FD_SET(server_sockfd, &readfds);

	while(1){
		char ch;
		int fd;
		int nread;

		testfds = readfds;
		printf("[Server] server waiting\n");
		
		result = select(FD_SETSIZE,
						&testfds,
						(fd_set *) 0, // I don't care
						(fd_set *) 0,
						(struct timeval *) 0); // wait for forever
		if( result < 1 ) {
			perror("server 5");
			close(server_sockfd);
			exit(EXIT_FAILURE);
		}

		for( int fd = 0 ; fd < FD_SETSIZE ; fd++ ) {
			if( FD_ISSET(fd, &testfds) ) {
				if( fd == server_sockfd ) { // new connection request
					// accept a connection
					client_len = sizeof(client_address);
					client_sockfd = accept(server_sockfd, 
									   (struct sockaddr *)&client_address, 
									   &client_len);
					FD_SET(client_sockfd, &readfds);
					printf("[server] adding client on fd %d\n", fd);
				}
				else {
					ioctl(fd, FIONREAD, &nread);

					if( nread == 0 ) {
						close(fd);
						FD_CLR(fd, &readfds);
						printf("[server] removing client on fd %d\n", fd);
					} 
					else {
						read(fd, &ch, 1);
						sleep(1);
						printf("[server] serving client on fd %d\n", fd);
						ch++;
						write(fd, &ch, 1);
					}
				}
			}
		}
	}
	// will never reach here...
	return 0;
}
