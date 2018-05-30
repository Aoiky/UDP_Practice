/*
*File:server.c
*Author:Aoiky
*Creat on:2018/05/11
*Description::UDP Broadcast
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main(int argc, char *argv[])
{
	int sockfd = 0;//接收socket返回值（文件描述符）
	int error_log = 0;//接收bind()返回值
	struct sockaddr_in bind_addr;//定义一个internet下的套接字地址
	unsigned short port = 8000;//定义端口
	int opt = 1;

	if (argc > 1) //命令行运行时可传一个端口参数（字符串）
	{
		port = atoi(argv[1]);//字符串转化为整形
	}

	printf("UDP Server Started!\n");
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//创建socket，参数1：:协议族；参数2：协议类型；参数3：:描述符
	if (sockfd < 0)
	{
		perror("socket error");
		exit(-1);
	}

	//sockaddr_in变量赋值：共四步（初始化地址、设置地址族、设置端口、设置地址）
	bzero(&bind_addr, sizeof(bind_addr));//地址初始化，bzero()内存清零函数
	bind_addr.sin_family = AF_INET;//地址族
	bind_addr.sin_port = htons(port);//16位端口号，htons()：主机字节顺序转换为网络字节顺序、host to net short（16位）
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);//32位ip地址（unsigned int），host to net long（32位）
												  //INADDR_ANY:接收任何IP（包括本机）


	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));//设置播报方式


	printf("Bind Server to port %d\n", port);
	error_log = bind(sockfd, (struct sockaddr*)&bind_addr, sizeof(bind_addr));//有些疑问->(struct sockaddr*)&bind_addr
	if (error_log < 0)
	{
		perror("bind error");
		close(sockfd);
		exit(-1);
	}

	//recvfrom->阻塞接收客户端数据->处理请求->sendto
	printf("waiting data from other client...\n");
	while (1)
	{
		char recv_buf[1024] = "";//为接收的信息分配内存空间
		char cli_ip[INET_ADDRSTRLEN] = "";//INET_ADDRSTRLEN：ipv4地址的长度，16位
		int recv_len = 0;
		struct sockaddr_in client_addr;//客户端socket地址
		socklen_t client_addr_len = sizeof(client_addr);//socklen_t -->int

		recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &client_addr_len);//接收信息

		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);//ip地址由二进制转化为十进制
		printf("client ip =%s  message:%s\n", cli_ip, recv_buf);

		sendto(sockfd, recv_buf, recv_len, 0, (struct sockaddr*)&client_addr, client_addr_len);//将接收到的字符串消息返回给客户端

	}

	close(sockfd);
	return 0;
}
