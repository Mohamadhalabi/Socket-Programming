#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>

#define PORT 1888
#define DEFAULT_BUFLEN 2000




void do_job(int fd)
{
int length,rcnt,client,listenfd ,rv;
char command[DEFAULT_BUFLEN];
	char newline='\n';
 char directory[DEFAULT_BUFLEN];
    		while((client =recv(fd, command, DEFAULT_BUFLEN - 1, 0)) >0 )
    {
        command[client] = '\0';
        
        if (strcmp(command,"LIST\n") ==0)
        {	   
    struct stat file_stats;
    DIR *dirp;
    struct dirent* dent;

    dirp=opendir("."); 
    do {
        dent = readdir(dirp);
        if (dent)
        {
        	//rv=send(fd,&file_stats.st_size,sizeof(file_stats.st_size),0);
		rv=send(fd,dent->d_name,strlen(dent->d_name),0);
       	if (!stat(dent->d_name,&file_stats)){
           	 rv=send(fd,&newline,1,0);

            }
            else
            {
                printf("(stat() failed for this file)\n");
            }
        }
    } while (dent);
    closedir(dirp);
}
       
    
   
  else if(strncmp(command,"GET\n",3)==0)
   { 


int i= strlen(command)-5 ;


	char subbuf[DEFAULT_BUFLEN];
	memcpy(subbuf,&command[4],i);
	subbuf[i]='\0';
	printf("%s",subbuf);
FILE * fPtr;

    char buffer[DEFAULT_BUFLEN];
    int totalRead = 0;


    fPtr = fopen(subbuf, "r");


    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* Unable to open file hence exit */
        printf("Please check whether file exists and you have read privilege.\n");
        exit(EXIT_FAILURE);
    }


    /* File open success message */
    printf("File opened successfully. Reading file contents line by line. \n\n");


    /* Repeat this until read line is not NULL */
    while(fgets(buffer, DEFAULT_BUFLEN, fPtr) != NULL) 
    {
        /* Total character read count */
        totalRead = strlen(buffer);


        /*
         * Trim new line character from last if exists.
         */
        buffer[totalRead - 1] = buffer[totalRead - 1] == '\n' 
                                    ? '\0' 
                                    : buffer[totalRead - 1];


        /* Print line read on cosole*/
	send(fd,buffer,strlen(buffer),0);
        
        }




    
  
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
    
   
  
