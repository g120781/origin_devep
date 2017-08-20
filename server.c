#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "protocal_376.h"

int main(int argc, char **argv)
{
    int server_sockid,client_sockid;
    int iRet;
    struct sockaddr_in server_addr,client_addr;
    int client_size, readsize;
    char *ip = "127.0.1.1";
    pid_t pid;

    if (argc !=2)
    {
         printf("parameter error!");
	 exit(1);
    }
    
    server_sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockid < 0)
    {
        perror("socket create error!");
	exit(2);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    server_addr.sin_family = AF_INET;
  //  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_sockid, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("bind error");
	exit(1);
    }

    iRet = listen(server_sockid,10);
    if(iRet != 0)
    {
        perror("listern error");
	exit(2);
    }

    client_size = sizeof(client_addr);
    
    while(1)
    {
        client_sockid = accept(server_sockid, (struct sockaddr_in *)&client_addr, &client_size);
        if(client_sockid < 0)
        {
            perror("accept eroror");
	    exit(3);
        }

        pid = fork();
        if (pid < 0)
        {
            perror("fork error");
	    exit(4);
        }
        else if(pid == 0)
        {
            unsigned char buf[4096];
            close(server_sockid);

            while(1)
            {
  	        memset(buf, 0, sizeof(buf));
                readsize = read(client_sockid, buf, sizeof(buf)-1);
	        if (readsize > 0)
	        {
	            for(int i=0; i<readsize; i++)
	            printf("%x ",buf[i]);
		    printf("\n");
	        }
	        else
	        {
	            printf("no data to read, close client_sockid =%d\n", client_sockid);
	            break;
	        }

		//check packet wheather leagal	
		if (0 == check_376packet_is_legal(buf))
		{		
		    write(client_sockid, buf, readsize);
                    sleep(2);
		}
	    }
	    close(client_sockid);
	    exit(0);
        }
        else
        { 
            close(client_sockid);
        }

    }
    wait();
    return 0;

}
