/* include host_serv */
#include	"unp.h"

/*
	addrinfo 能处理名字到地址以及服务到端口这两种转换，返回的是一个 sockaddr 结构而不是一个地址列表
	通过result指针返回一个指向adaddrinfo结构链表的指针


	int getaddrinfo(const char *hostname, const char *service,
					const struct addrinfo *hints, struct addrinfo **result);

	1> TCP 和 UDP 客户进程： 指定 hostname 和 service
	2> 典型的服务器进程: 只指定 service，同时在 hints 结构中指定 AI_PASSIVE 标志
		
*/

/* 例子: 
	struct addrinfo hints, *res;
	bezero(&hints, sizeof(hints));
	hints.ai_flags = AI_CANONNAME;
	hints.ai_family = AF_INET;
	getaddrinfo("freebsd4", "domain", &hints, &res);
*/
struct about_addrinfo {
	int ai_flags; // AI_PASSIVE, AI_CANONNAME
	int ai_family; // AF_XXX
	int ai_socktype; // SOCK_XXX
	int ai_protocol; // 0 or IPPROTO_xxx for IPv4 and Ipv6
	socklen_t ai_addrlen; // length of ai_addr
	char *ai_canonname; // ptr to canonical name for host
	struct sockaddr *ai_addr; // ptr to socket address structure
	struct addrinfo *ai_naxt; // ptr to next structure in linked list 
};

// host_serv 函数
struct addrinfo *
host_serv(const char *host, const char *serv, int family, int socktype)
{
	int				n;
	struct addrinfo	hints, *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
	hints.ai_family = family;		/* AF_UNSPEC, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		return(NULL);

	return(res);	/* return pointer to first on linked list */
}
/* end host_serv */

/*
 * There is no easy way to pass back the integer return code from
 * getaddrinfo() in the function above, short of adding another argument
 * that is a pointer, so the easiest way to provide the wrapper function
 * is just to duplicate the simple function as we do here.
 */

struct addrinfo *
Host_serv(const char *host, const char *serv, int family, int socktype)
{
	int				n;
	struct addrinfo	hints, *res;

	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
	hints.ai_family = family;		/* 0, AF_INET, AF_INET6, etc. */
	hints.ai_socktype = socktype;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

	if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
		err_quit("host_serv error for %s, %s: %s",
				 (host == NULL) ? "(no hostname)" : host,
				 (serv == NULL) ? "(no service name)" : serv,
				 gai_strerror(n));

	return(res);	/* return pointer to first on linked list */
}
