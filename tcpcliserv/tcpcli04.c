#include	"unp.h"

// 与服务器建立了5个连接的TCP客户程序
int main(int argc, char **argv)
{
	int					i, sockfd[5];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: tcpcli <IPaddress>");

	for (i = 0; i < 5; i++) {
		sockfd[i] = Socket(AF_INET, SOCK_STREAM, 0);

		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(SERV_PORT);

		Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

		Connect(sockfd[i], (SA *) &servaddr, sizeof(servaddr));
	}

	// 调用 str_cli 函数时仅用第一个连接
	str_cli(stdin, sockfd[0]);		/* do it all */

	// exit时，所有打开的描述符由内核自动关闭，基本在同一时刻终止，
	// 引发了5个FIN，每个连接一个,它们反过来使服务器的5个子进程基本在同一时刻终止
	// 导致差不多在同一时刻有5个 SIGCHLD 信号递交给父进程
	exit(0);
}
