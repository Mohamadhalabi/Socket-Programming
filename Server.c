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
#include "iniparser.h"
#define PORT 1888
#define DEFAULT_BUFLEN 2000


void do_job(int fd)
{

int length,rcnt,client,listenfd ,rv;
char command[DEFAULT_BUFLEN],comm[DEFAULT_BUFLEN];
	char newline='\n';
	char errormessage[DEFAULT_BUFLEN]="Please enter a valid command";
 char directory[DEFAULT_BUFLEN];
 char dot='.';
 
char login[256]="login successful";
char login2[256]="username or password incorrect!";
    		while((client =recv(fd, comm, DEFAULT_BUFLEN - 1, 0)) >0 )
    {
        command[client] = '\0';
                  comm[client] = '\0';
          if(strncmp(comm,"USER\n",4)==0)
          {
     int i= strlen(comm)-6 ;
     
     char subbuf[DEFAULT_BUFLEN];
	memcpy(subbuf,&comm[5],i);
	subbuf[i]='\0';
 

     dictionary *dict;
	char *str;
	char *sp;
	char space[20]=" ";

	dict=iniparser_load("a.ini");

	str=iniparser_getstring(dict,"sec1:key1","NULL");
	sp=iniparser_getstring(dict,"sec1:key2","NULL");

	strcat(str,space);
	strcat(str,sp);
	
	if(strcmp(str,subbuf)==0){
	send(fd,login,strlen(login),0);
	send(fd,&newline,1,0);
	
	
     while((client =recv(fd, command, DEFAULT_BUFLEN - 1, 0))>0)
     {
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
    
    memset(command,0,strlen(command));
}
       
  
  else if(strncmp(command,"GET\n",3)==0)
   { 
   
int i= strlen(command)-5 ;

	char subbuf[DEFAULT_BUFLEN];
	memcpy(subbuf,&command[4],i);
	subbuf[i]='\0';
	FILE * fPtr;

    char buffer[DEFAULT_BUFLEN];
    int totalRead = 0;


    fPtr = fopen(subbuf, "r");


    if(fPtr == NULL)
    {
	    char Messagee="can't open folder";
	send(fd,Messagee,strlen(Messagee),0);
    
    }


    while(fgets(buffer, DEFAULT_BUFLEN, fPtr) != NULL) 
    {

        totalRead = strlen(buffer);

        buffer[totalRead - 1] = buffer[totalRead - 1] == '\n' 
                                    ? '\0' 
                                    : buffer[totalRead - 1];

	rv=send(fd,buffer,strlen(buffer),0);
	if(rv!=buffer)
	rv=send(fd,&newline,1,0);

        
        }
	rv=send(fd,&dot,1,0);
  	rv=send(fd,&newline,1,0);
   memset(command,0,strlen(command));
}  
     
     
     else if(strncmp(command,"DEL\n",3)==0)
{
	char deleted[DEFAULT_BUFLEN]="The file is deleted successfully";
	char notdeleted[DEFAULT_BUFLEN]="The file is not deleted";
	int i= strlen(command)-5 ;


	char subbuf[DEFAULT_BUFLEN];
	memcpy(subbuf,&command[4],i);
	subbuf[i]='\0';
	

  	 int del = remove(subbuf);
  	 if (!del)
	send(fd,deleted,strlen(deleted),0);
  	 else
	send(fd,notdeleted,strlen(notdeleted),0);
	
  	send(fd,&newline,1,0);
	send(fd,&dot,1,0);
}


	else if(strncmp(command,"PUT",3)==0)
	{
	
	
	int e;
	int i= strlen(comm)-6 ;
     
     	char subbuf[DEFAULT_BUFLEN];
	memcpy(subbuf,&comm[5],i);
	subbuf[i]='\0';
	
	printf("%s",subbuf);
	
	
	
	}


	else if(strcmp(command,"QUIT"))
	{
		char message[DEFAULT_BUFLEN]="Goodbye!";
		
		send(fd,message,strlen(message),0);
	
	close(fd);
		
	
	}
	
	else 
	{
	char msg[DEFAULT_BUFLEN]="insert a valid command!";
			send(fd,msg,strlen(msg),0);
	
	
	}
  
  
}  
}
else
	send(fd,login2,strlen(login2),0);
	  	send(fd,&newline,1,0);
	  void do_job(int fd);
  }
   }


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
    send(fd,Message,strlen(Message),0);
        close(listenfd);
        do_job(fd);
        printf("job finished !\n");
        close(fd);
        exit(0);
    }

} 
       close(listenfd);
    
    return 0;
    }
