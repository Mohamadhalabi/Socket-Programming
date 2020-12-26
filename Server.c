#include <stdio.h>
#include <string.h>   
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>  
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
  
#define PORT 1888
#define DEFAULT_BUFLEN 2000
#define MAXSIZE 100

int main()
{
    int server,addrlen ,new_socket,client_socket[MAXSIZE],maxclients = MAXSIZE;
    int maxsd;
    int i;
    int actv;
    int sd;
    int valread;
    struct sockaddr_in address;
      
    char buffer[DEFAULT_BUFLEN];  
      
    fd_set readfds;

    char message[256]= "Welcome to Mohamad's file server \r\n";
  
    for (i = 0; i < maxclients; i++) 
    {
        client_socket[i] = 0;
    }
    if( (server = socket(AF_INET,SOCK_STREAM,0)) == 0) 
    {
        perror("socket failed");
        exit(1);
    }
    address.sin_family =AF_INET;
    address.sin_addr.s_addr =INADDR_ANY;
    address.sin_port =htons( PORT );
      
    if (bind(server,(struct sockaddr *)&address,sizeof(address))<0) 
    {
        perror("bind failed");
        exit(1);
    }
    printf("File server listening on localhost port %d \n", PORT);
     
    if (listen(server, 10) < 0)
    {
        perror("listen");
        exit(1);
    }
      
    addrlen = sizeof(address);
     
    while(1) 
    {
        FD_ZERO(&readfds);
  
        FD_SET(server,&readfds);
        maxsd = server;
         
        for ( i = 0 ; i < maxclients ; i++) 
        {
            sd=client_socket[i];
             
            if(sd>0)
                FD_SET(sd,&readfds);
        
            if(sd > maxsd)
                maxsd = sd;
        }
  
      
       actv = select(maxsd + 1,&readfds,NULL,NULL,NULL);
    
        if (actv < 0)
        {
            printf("error");
        }
      
          
        if (FD_ISSET(server,&readfds)) 
        {
            if ((new_socket = accept(server,(struct sockaddr*)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(1);
            }
          
            printf("New connection from %s:%d \n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
        
          
            if( send(new_socket,message,strlen(message),0) !=strlen(message) ) 
            {
                perror("Message is not sent");
            }
              
            for (i = 0;i < maxclients; i++) 
            {
             
                if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    break;
                }
            }
        }
     
        for (i = 0; i < maxclients; i++) 
        {
            sd = client_socket[i];
              
            if (FD_ISSET(sd,&readfds)) 
            {

                if ((valread = read(sd,buffer,2000))==0)
                {
                    getpeername(sd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
                    printf("Host disconnected from %s:%d \n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
                      
                    close(sd);
                    client_socket[i] = 0;
                }
                  
            }
        }
    }
      
    return 0;
}
