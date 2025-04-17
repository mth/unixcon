#if 0
gcc -Os -fPIC -shared -o unixcon.so $0
exit $?
#endif

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static inline const struct sockaddr_un* special_addr(const struct sockaddr *addr, socklen_t addrlen) {
	static struct sockaddr_un sun;

	if (addr->sa_family == AF_INET && addrlen >= sizeof (struct sockaddr_in)) {
		struct sockaddr_in *sin = (struct sockaddr_in*) addr;
		if (sin->sin_addr.s_addr == htonl(1) && sin->sin_port == htons(23)) {
			if (!sun.sun_path[0]) {
				const char *p = getenv("CONNECT_PATH");
				if (!p)
					return 0;
				strncpy(sun.sun_path, p, sizeof sun.sun_path - 1);
				sun.sun_family = AF_UNIX;
			}
			return &sun;
		}
	}
	return 0;
}

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	static int(*connect_p)(int, const struct sockaddr*, socklen_t) = NULL;

	const struct sockaddr_un *sun = special_addr(addr, addrlen);
	if (sun) {
		int fd = socket(AF_UNIX, SOCK_STREAM, 0);
		if (fd < 0)
			return -1;
		int flags = fcntl(sockfd, F_GETFL);
		int result = dup2(fd, sockfd);
		if (result == -1)
			return -1;
		close(fd);
		if (flags != -1)
			fcntl(sockfd, F_SETFL, flags);
		addr = (const struct sockaddr*) sun;
		addrlen = sizeof *sun;
	}

	if (connect_p || (connect_p = dlsym(RTLD_NEXT, "connect")))
		return connect_p(sockfd, addr, addrlen);
	errno = ENOSYS;
	return -1;
}
