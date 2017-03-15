/* include tcp_connect */
#include	"unp.h"

//  使用 getaddrinfo 处理 TCP 客户和服务器大多数情形的两个函数之一 
// tcp_connect 执行客户的通常步骤

int tcp_connect(const char *host, const char *serv)
{
	int				sockfd, n;
	struct addrinfo	hints, *res, *ressave;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	// getaddrinfo
	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("tcp_connect error for %s, %s: %s", host, serv, gai_strerror(n));
	
	// ressave
	ressave = res;

	// 尝试 getaddrinfo 返回的每个IP地址，针对它们调用 socket 和 connect
	do {
		// socket
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (sockfd < 0)
			continue;	/* ignore this one */

		// connect
		if (connect(sockfd, res->ai_addr, res->ai_addrlen) == 0)
			break;		/* success */

		Close(sockfd);	/* ignore this one */
	} while ( (res = res->ai_next) != NULL);

	if (res == NULL)	/* errno set from final connect() */
		err_sys("tcp_connect error for %s, %s", host, serv);

	// void freeaddrinfo(struct addrinfo *ai)
	// ai 参数应指向由 getaddrinfo 返回的第一个 addrinfo 结构
	// 这个链表中的所有结构以及由它们指向的任何动态存储空间都被释放掉
	freeaddrinfo(ressave);

	return(sockfd);
}
/* end tcp_connect */

/*
 * We place the wrapper function here, not in wraplib.c, because some
 * XTI programs need to include wraplib.c, and it also defines
 * a Tcp_connect() function.
 */

int
Tcp_connect(const char *host, const char *serv)
{
	return(tcp_connect(host, serv));
}
