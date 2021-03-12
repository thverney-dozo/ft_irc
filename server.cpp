#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define PORT 18000
#define SA struct sockaddr

typedef struct sockaddr_in SA_IN;

int main(int ac, char **av)
{
	int servfd, clientfd;
	struct sockaddr_in servaddr;
	uint8_t buff[1024];
	uint8_t recvline[1024];
	std::string str;
	
	//Create the server fd
	if((servfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		std::cerr << "Socket error" << std::endl;
		exit(EXIT_FAILURE);
	}
	printf("servfd = %d\n", servfd);
	
	servaddr.sin_family = AF_INET; // Internet address
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Server will respond to anything
	servaddr.sin_port = htons(PORT); // Server will listen to this port
	
	if((bind(servfd, (SA *)&servaddr, sizeof(servaddr))) < 0)
	{
		std::cerr << "Bind error" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	if ((listen(servfd, 100)) < 0)
	{
		std::cerr << "Listen error" << std::endl;
		exit(EXIT_FAILURE);
	}
	for ( ; ; )
	{
		struct sockaddr_in addr;
		int	addr_size = sizeof(SA_IN);
		SA_IN client_addr;

		
		// Accept wait untill a connection arrives, it returns a fd to the connection.
		clientfd = accept(servfd, (SA *)&client_addr, (socklen_t*)&addr_size); 
		// Both NULL params allow you to get the address of whoever connected
		if (clientfd == -1)
			write(1, "clientfd error\n", 16);
		std::cout << std::endl;
		connect(clientfd, (SA*)&servaddr, addr_size);	
		fcntl(clientfd, F_SETFL, O_NONBLOCK);
		write(clientfd, "Bonjour bienvenue\r\n", strlen("Bonjour bienvenue\r\n"));
		//Read what the client sent and put it in str

		char str_recv[1024];
		int msg_size = 0;
		int check = 0;
		size_t n;
		while ((n = read(clientfd, str_recv, 1023)) > 0)
		{
			msg_size += n;
			if (msg_size > 1024 -1 || str_recv[msg_size - 1] == '\n')
				break;
		}
		write(1, str_recv, strlen(str_recv));
		str_recv[msg_size -1] = 0;
		if (n < 0)
		{
			std::cerr << "Read error" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	return 0;
}