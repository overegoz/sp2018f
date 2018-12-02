#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
	
	signal(SIGCHLD, SIG_IGN); // do not create zombies

	while(1){
		char ch;
		printf("[Server] server waiting\n");
		
		// accept a connection
		client_len = sizeof(client_address);
		client_sockfd = accept(server_sockfd, 
							   (struct sockaddr *)&client_address, 
							   &client_len);
	
		if( fork() == 0 ) { // child
			// read and write to client on client_sockfd
			read(client_sockfd, &ch, 1);
			sleep(1); // just for demonstration 
					  // (assuming a time-taking task is being done)
			printf("[Server] msg rcvd from client: %c\n", ch);
			ch++;
			write(client_sockfd, &ch, 1);
			close(client_sockfd);
			exit(EXIT_SUCCESS);
		} // end of child process
		else { // parent
			close(client_sockfd); // parent do not need this
		}
	}

	// will never reach here...
	return 0;
}
