#include	"unp.h"

// tcp 回射客户程序
int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	// 1. 创建一个 TCP 套接字
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	printf("客户端调用Socket的sockfd:%d\n",sockfd);

	// 2. 用服务器的IP地址和端口号装填一个网际套接字地址结构
	// 从命令行参数取得服务器的IP地址
	// 从头文件 unp.h 取得服务器的众所周知的端口号 SERV_PORT
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	// 3. Connect 建立与服务器的连接
	printf("Connect 开始");
	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));
	printf("客户端Connect返回后的sockfd:%d\n",sockfd);
	printf("Connect 返回");

	// str_cli 完成剩余部分的客户处理工作
	str_cli(stdin, sockfd);		/* do it all */

	exit(0);
}
