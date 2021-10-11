#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>

int main()
{
	char buf[128];
	struct hostent *host;
	int	i;

	for( ; ; ) {
		printf("Enter hostname:");
		gets(buf);
		host = gethostbyname(buf);
		if(host == NULL) {
			printf("gethostbyname failed\n");
			continue;
		}
		printf("Official name=%s\n", host->h_name);
		for(i = 0; host->h_aliases[i]; i++)
		  printf("Alias:%s\n", host->h_aliases[i]);
		for(i = 0; host->h_addr_list[i]; i++)
		  printf("IPaddr %s\n", inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
	}
	return(0);
}
