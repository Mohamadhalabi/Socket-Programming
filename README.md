#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>
#include <linux/in.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 1888
#define DEFAULT_BUFLEN 2000






int main()
{
		struct sockaddr_in addr;
	int listenfd, client, addr_size=sizeof(addr),valread;
	char Message[256]="Welcome to mohamad's file server\n";
	char buffer[DEFAULT_BUFLEN];  
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	pthread_t thread;
	
	bzero(&addr,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	
	(bind(listenfd, (struct sockaddr *) &addr, sizeof(addr)));
	listen(listenfd,SOMAXCONN);
	{
		printf("File server listening on localhost port %d\n",PORT);
	}
	
	
	while(1)
	{
	
		client=accept(listenfd,(struct sockaddr*)&addr, &addr_size);
		     	 send(client,Message,strlen(Message),0) !=strlen(Message);
        	printf("Connected: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)); 
       
        	
        	if(pthread_create(thread,NULL,command,&client)!=0) 
        		perror("Thread creation");
        	else
        		pthread_detach(thread);
        }
    
    return 0;
    }
        		
        	
        	
        	
        	
        	
