#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int listenfd,connfd,n,m,i;
  	struct sockaddr_in servaddr,cliaddr;
  	socklen_t clilen;
   	pid_t     childpid;
   	char mesg[500];
	char response[] = "Response";

   	listenfd=socket(AF_INET,SOCK_STREAM,0);

   	bzero(&servaddr,sizeof(servaddr));
   	servaddr.sin_family = AF_INET;
  	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
   	servaddr.sin_port=htons(5050);
   	bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr));

   	listen(listenfd,10);
	printf("Start listen...\n");

   for(;;)
   {
      clilen=sizeof(cliaddr);
      connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&clilen);

      if ((childpid = fork()) == 0)
      {
         close (listenfd);

         for(;;)
         {
            //n = recvfrom(connfd,mesg,1000,0,(struct sockaddr *)&cliaddr,&clilen);
			n = recv(connfd, mesg, sizeof(mesg), 0);
			printf("recved n=%d bytes\n", n);
            //sendto(connfd,mesg,n,0,(struct sockaddr *)&cliaddr,sizeof(cliaddr));
			m = send(connfd, response, sizeof(response)-1, 0);
            printf("-------------------------------------------------------\n");
            //mesg[n] = 0;
            printf("Received the following:\n");
            printf("%s\n",mesg);
			printf("HEX: ");
			for(i=0;i<n;i++){
				printf("%x ", mesg[i]);
			}
			printf("\nSend %d bytes", m);
            printf("-------------------------------------------------------\n");
         }
         
      }
      close(connfd);
   }	
	return 0;
}
