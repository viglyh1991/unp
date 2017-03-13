#include	"unp.h"

void
sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	// pid_t waitpid(pid_t pid, int *statloc, int options);
	// pid 参数 允许我们指定想等待的进程ID, -1 表示等待第一个终止的子进程
	// statloc 指针返回子进程终止状态
	// options参数允许我们指定附加选项, 如 WNOHANG, 告知内核在没有已终止子进程时不要阻塞
	
	// 在一个循环内调用 waitpid, 以获取所有已终止子进程的状态
	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}
