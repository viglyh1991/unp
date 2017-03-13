#include	"unp.h"

// 使用 Select 正确处理 EOF 的 str_cli 函数
// 使用select ：只要服务器关闭它那一端的连接就会通知我们
// Shutdown: 允许我们正确地处理批量输入
void str_cli(FILE *fp, int sockfd)
{
	int			maxfdp1, stdineof;
	fd_set		rset;
	char		buf[MAXLINE];
	int		n;

	// stdineof 初始化为0的标志
	stdineof = 0;
	FD_ZERO(&rset);

	for ( ; ; ) {
		// stdineof 为0, 每次在主循环中我们总是 select 标准输入的可读性。
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);

		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(sockfd, &rset)) {	/* socket is readable */
		   // 这里改用 read 和 write 对缓冲区而不是文本进行操作，使得select能够如期地工作
			if ( (n = Read(sockfd, buf, MAXLINE)) == 0) {
				// 在套接字遇到EOF时，如果我们已经在标准输入上遇到EOF，那就是正常的终止。
				if (stdineof == 1)
					return;		/* normal termination */
				else // 如果在标准输入没有遇到EOF,那么服务器进程已过早终止。
					err_quit("str_cli: server terminated prematurely");
			}

			Write(fileno(stdout), buf, n);
		}

		if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */
	       // 这里改用 read 和 write 对缓冲区而不是文本进行操作，使得select能够如期地工作
			if ( (n = Read(fileno(fp), buf, MAXLINE)) == 0) {
				// 遇到EOF, 把标志设置为 1
				stdineof = 1;

				// SHUT_RD: 关闭连接的读这一半，套接字不再有数据可接收，而且套接字接收缓冲区中的现有数据都被丢弃
				// SHUT_WR: 关闭连接的写这一半，当前留在套接字发送缓冲区中的数据将被发送掉，后跟TCP的正常连接终止序列
				// SHUT_RDWR: 连接的读半部和写半部都关闭
				Shutdown(sockfd, SHUT_WR);	/* send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}

			Writen(sockfd, buf, n);
		}
	}
}
