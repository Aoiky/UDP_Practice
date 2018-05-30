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
	int sockfd = 0;//����socket����ֵ���ļ���������
	int error_log = 0;//����bind()����ֵ
	struct sockaddr_in bind_addr;//����һ��internet�µ��׽��ֵ�ַ
	unsigned short port = 8000;//����˿�
	int opt = 1;

	if (argc > 1) //����������ʱ�ɴ�һ���˿ڲ������ַ�����
	{
		port = atoi(argv[1]);//�ַ���ת��Ϊ����
	}

	printf("UDP Server Started!\n");
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);//����socket������1��:Э���壻����2��Э�����ͣ�����3��:������
	if (sockfd < 0)
	{
		perror("socket error");
		exit(-1);
	}

	//sockaddr_in������ֵ�����Ĳ�����ʼ����ַ�����õ�ַ�塢���ö˿ڡ����õ�ַ��
	bzero(&bind_addr, sizeof(bind_addr));//��ַ��ʼ����bzero()�ڴ����㺯��
	bind_addr.sin_family = AF_INET;//��ַ��
	bind_addr.sin_port = htons(port);//16λ�˿ںţ�htons()�������ֽ�˳��ת��Ϊ�����ֽ�˳��host to net short��16λ��
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);//32λip��ַ��unsigned int����host to net long��32λ��
												  //INADDR_ANY:�����κ�IP������������


	setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));


	printf("Bind Server to port %d\n", port);
	error_log = bind(sockfd, (struct sockaddr*)&bind_addr, sizeof(bind_addr));//��Щ����
	if (error_log < 0)
	{
		perror("bind error");
		close(sockfd);
		exit(-1);
	}

	//recvfrom->�������տͻ�������->��������->sendto
	printf("waiting data from other client...\n");
	while (1)
	{
		char recv_buf[1024] = "";//Ϊ���յ���Ϣ�����ڴ�ռ�
		char cli_ip[INET_ADDRSTRLEN] = "";//INET_ADDRSTRLEN��ipv4��ַ�ĳ��ȣ�16λ
		int recv_len = 0;
		struct sockaddr_in client_addr;//�ͻ���socket��ַ
		socklen_t client_addr_len = sizeof(client_addr);//socklen_t -->int

		recv_len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &client_addr_len);//������Ϣ

		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);//ip��ַ�ɶ�����ת��Ϊʮ����
		printf("client ip =%s  message:%s\n", cli_ip, recv_buf);

		sendto(sockfd, recv_buf, recv_len, 0, (struct sockaddr*)&client_addr, client_addr_len);//�����յ����ַ�����Ϣ���ظ��ͻ���

	}

	close(sockfd);
	return 0;
}
