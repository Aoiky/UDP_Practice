/*
*File:client.c
*Author:Aoiky
*Creat on:2018/05/10
*Description:Practice for UDP
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>


int main(int argc,char *argv[]) 
{
	int sockfd = 0;
	struct sockaddr_in server_addr;
	unsigned short port = 8000;
	char ser_ip[INET_ADDRSTRLEN] = "192.168.11.73";

	
	if(argc>1)
	{
		strcpy(ser_ip , argv[1]);
	}

	if(argc>2)
	{
		port=atoi(argv[2]);
	}
	


	//创建套接字
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if (sockfd<0) 
	{
		perror("socket error!");
		exit(-1);
	}
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET,ser_ip,&server_addr.sin_addr);//点十进制转二进制
	//server_addr.sin_addr.s_addr = inet_addr(ser_ip);//点十进制转u_long（长整数型）
	
	//sento->revefrom
	printf("ready send data to UDP server %s:%d!\n",ser_ip,port);
	while (1)
	{
		char send_buf[2048] = "";
		int len = 0;

		fgets(send_buf,sizeof(send_buf),stdin);
		send_buf[strlen(send_buf)-1] = '\0';

		sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&server_addr,sizeof(server_addr));

		len = recvfrom(sockfd,send_buf,sizeof(send_buf),0,NULL,NULL);

		printf("%s\n",send_buf);

	}

	close(sockfd);
	return 0;
}
