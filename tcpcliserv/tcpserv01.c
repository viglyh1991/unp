#include	"unp.h"

// tcp 回射服务器程序
int main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;

	// Socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // 通配地址
	servaddr.sin_port        = htons(SERV_PORT); // 端口号为 9877

	// Bind
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	// Listen
	Listen(listenfd, LISTENQ);
	printf("lisent 返回");

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		printf("Accept() 开始");
		// 返回一个已连接套接字
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
		printf("Accept() 返回");

		if ( (childpid = Fork()) == 0 ) {	/* child process */
			printf("通过fork(), 进入子线程");
			// 子进程关闭监听套接字
			Close(listenfd);	/* close listening socket */
			str_echo(connfd);	/* process the request */
			//  服务器子进程终止时，给父进程发送一个 SIGCHLD 信号，如果父进程没有代码中捕获处理，那么子进程进入僵死状态
			exit(0);
		}
		// 父进程关闭已连接套接字
		Close(connfd);			/* parent closes connected socket */
	}
}
