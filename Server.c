#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>


#define PORT 1888
#define DEFAULT_BUFLEN 2000
#define SIZE 1024

typedef struct
{
	int sock;
	struct sockaddr address;
	int addr_len;
} connection_t;



void do_job(int fd)
{
int length,rcnt,client,listenfd;
char command[DEFAULT_BUFLEN];
	char newline='\n'; //
	char error[256]="wrong command"; 

    
    		while((client =recv(fd, command, DEFAULT_BUFLEN - 1, 0)) >0 )
    {
        command[client] = '\0'; // add null terminator
        if (strcmp(command,"LIST\n") ==0)
        {
        
     		     
         	DIR *pDIR;
    struct dirent *pDirEnt;

    /* Open the current directory */
	int rv;
    pDIR = opendir(".");

    if ( pDIR == NULL ) {
        fprintf( stderr, "%s %d: opendir() failed (%s)\n",
                __FILE__, __LINE__, strerror( errno ));
        exit( -1 );
    }

    /* Get each directory entry from pDIR and print its name */

    pDirEnt = readdir( pDIR );
    while ( pDirEnt != NULL ) {
        pDirEnt = readdir( pDIR );
       rv =send( fd,pDirEnt->d_name,strlen(pDirEnt->d_name),0);
       if (rv !=pDirEnt->d_name)
       rv=send(fd,&newline,1,0);
    }

    /* Release the open directory */

    closedir( pDIR );
   }
   }
    close(client);
    
    
}


int main(int argc ,char *argv[])
{


	struct sockaddr_in addr;
	struct sockaddr_in remote_addr;
	int listenfd, addr_size,client,length,fd;
	char command[DEFAULT_BUFLEN];
	char Message[256]="Welcome to mohamad's file server\n",buffer[DEFAULT_BUFLEN];
	char client_message[256];  
	connection_t * connection;
	pthread_t thread;
	pid_t pid;
			
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

	
	
	while(1) {  // main accept() loop
    length = sizeof remote_addr;
    if ((fd = accept(listenfd, (struct sockaddr *)&remote_addr, \
          &length)) == -1) {
          perror("Accept Problem!");
          continue;
    }

    printf("Server: got connection from %s\n", \
            inet_ntoa(remote_addr.sin_addr));

    /* If fork create Child, take control over child and close on server side */
    if ((pid=fork()) == 0) {
        close(listenfd);
        do_job(fd);
        printf("Child finished their job!\n");
        close(fd);
        exit(0);
    }

} 
       close(listenfd);
    
    return 0;
    } 
        	
