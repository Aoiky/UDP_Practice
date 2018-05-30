/*
  *File:client.c
  *Author:Aoiky
  *Creat on:2018/05/11
  *Description:UDP Broadcast
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc,char* argv[])
{
	int sockfd = 0;
	struct sockaddr_in server_addr;
	unsigned short port = 8000;
	int opt = 1;

	sockfd = socket(AF_INET,SOCK_DGRAM,0);

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,(char*)&opt,sizeof(opt));

	while (1)
	{
		char send_buf[2048] = "";
		int len = 0;

		fgets(send_buf, sizeof(send_buf), stdin);
		send_buf[strlen(send_buf) - 1] = '\0';

		sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

		len = recvfrom(sockfd, send_buf, sizeof(send_buf), 0, NULL, NULL);

		printf("%s\n", send_buf);

	}

	close(sockfd);
	return 0;
}

