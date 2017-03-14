#include	"unp.h"

// hostent 的结构
struct about_hostent {
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
};

int main(int argc, char **argv)
{
	char			*ptr, **pptr;
	char			str[INET_ADDRSTRLEN];
	struct hostent	*hptr;

	while (--argc > 0) {
		ptr = *++argv;

		// gethostbyname 把主机名映射成 IPV4地址
		// struct hostent *gethostbyname(const char *hostname);
		// 返回一个指向 hostent 结构的指针
		// 只能返回 IPV4地址 
		if ( (hptr = gethostbyname(ptr)) == NULL) {
			err_msg("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
			continue;
		}

		// 输出规范主机名
		printf("official hostname: %s\n", hptr->h_name);

		// 别名列表
		for (pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("\talias: %s\n", *pptr);

		switch (hptr->h_addrtype) 
		{
		case AF_INET:
			// pptr 指向一个指针数组，其中每个指针指向一个地址。
			pptr = hptr->h_addr_list;
			for ( ; *pptr != NULL; pptr++)
				// 对于每个地址，调用 Inet_ntop 并输出返回的字符串
				printf("\taddress: %s\n", Inet_ntop(hptr->h_addrtype, *pptr, str, sizeof(str)));
			break;

		default:
			err_ret("unknown address type");
			break;
		}
	}
	exit(0);
}
