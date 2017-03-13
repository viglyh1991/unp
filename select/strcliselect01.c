#include	"unp.h"

/*
 IO 复用：使得内核一旦发现进程指定的一个或多个I/O条件就绪
 (也就是说输入已准备好被读取，或者描述符已能承接更多的输出)，它就通知进程

 // 若有就绪描述符则为其数目，若超时则为0， 若出错则为-1
 int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, const struct timeval *timeout)
 
 void FD_ZERO(fd_set *fdset);
 void FD_SET(int fd, fd_set *fd_set);
 void FD_CLR(int fd, fd_set *fd_set);
 int FD_ISSET(int fd, fd_set *fdset);
*/

// 使用 select 的 str_cli 函数的实现
void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1;
	fd_set		rset;
	char		sendline[MAXLINE], recvline[MAXLINE];

	FD_ZERO(&rset);

	// 无限循环中...
	for ( ; ; ) {
		// fileno函数把标准IO文件指针转换为对应的描述符,select 和 poll 只工作在描述符上
		FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;

		Select(maxfdp1, &rset, NULL, NULL, NULL);

		// select 返回时，套接字是可读的
		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
			if (Readline(sockfd, recvline, MAXLINE) == 0)
				err_quit("str_cli: server terminated prematurely");
			Fputs(recvline, stdout);
		}

		// 如果标准输入可读时...
		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
			if (Fgets(sendline, MAXLINE, fp) == NULL)
				return;		/* all done */
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}
