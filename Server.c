#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>


#define PORT 1888
#define DEFAULT_BUFLEN 2000
#define SIZE 1024

typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;



void process(int socket);
void command(int sid,char *str);



int main(int argc ,char *argv[])
{
	struct sockaddr_in addr;
	int listenfd, addr_size,client;
	
	char Message[256]="Welcome to mohamad's file server\n",buffer[DEFAULT_BUFLEN];  
	connection_t * connection;
	pthread_t thread;
			
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	

	
	bzero(&addr,sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	memset(&(addr.sin_zero), 0, 8);
	
	
	bind(listenfd, (struct sockaddr *) &addr, sizeof(addr));
	listen(listenfd,SOMAXCONN);
	
	printf("File server listening on localhost port %d\n",PORT);
	
	addr_size=sizeof(struct sockaddr_in);

	
	
	while(1)
	{
		connection = (connection_t *)malloc(sizeof(connection_t));
       		 connection->sock = accept(listenfd, &connection->address, &connection->addr_len);
		
		if (connection->sock <= 0)
		{
			free(connection);
		}
		else
		{
		pthread_create(&thread,NULL,(void *) &process,(void *)connection);
			
        	}
       }
    
    return 0;
    }
    
    void process (int socket) {
	char buf[SIZE];
	bzero(buf, SIZE);
	read(socket, buf, SIZE);		
	command(socket, buf);	
    }
    
    
    
    void command(int sid,char *str){
    
    	int select;
    	char *e="Invalid command. \n";
    	
    	if(strncmp(str,"LIST",4)==0)
    		select=1;
    	else if(strncmp(str,"GET",3)==0)
    		select=2;
    	else if(strncmp(str,"PUT",3)==0)
    		select=3;
    	else if(strncmp(str,"DEL",3)==0)
    		select=4;
    	else if(strncmp(str,"QUIT",4)==0)
    		select=5;
    	else
    		select=0;
    	switch(select){
    		case 1:
    			list(sid,str);
    			break;
    		case 2:
    			get(sid,str);
    			break;
    		case 3:
    			put(sid,str);
    			break;
    		case 4:
    			del(sid,str);
    			break;
    		case 5:
    			quit(sid,str);
    			break;
    		default:
    			printf("invalid command\n");
    			write(sid, e, strlen(e));
    		}
    	}
    			
    	
    	
        	
