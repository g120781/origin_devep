#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include "protocal_376.h"

int main(int argc, char **argv)
{
    int client_sockid;
    struct sockaddr_in server_addr;
    int writesize;

    if (argc !=3)
    {
        printf("parater error!");
	exit(1);
    }

    client_sockid = socket(AF_INET, SOCK_STREAM, 0);
    if (client_sockid < 0)
    {
        perror("socket error");
	exit(2);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    if(connect(client_sockid, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect error\n");
	exit(3);
    }

    unsigned char buf[4096]={0};
    unsigned char userdata[] ={0x01,0x02,0x03,0x04,0x05,0x06};

    ADDRESS_AREA addr;
    CTL_AREA ctrl;

    //fitst step , pad 0x68
    pad_0x68(buf);

    //set length
    set_length_376(buf,sizeof(userdata));

    //set address
    addr.A1 = 0x1234;
    addr.A2 = 0x56;
    addr.A3 =0;
    set_address_area(buf, &addr);

    //set control area
    ctrl.DIR = 1;
    ctrl.PRM = 1;
    ctrl.FRAME_CNT = 0;
    ctrl.FRAME_FLAG = 0;
    ctrl.FUNC = 0x04;
    set_ctrl_area(buf, &ctrl);

    //add data to area
    add_data_to_buffer(buf, userdata, 6);

    //calculate checksum
    unsigned char checksum =0;
    checksum = calculate_checksum(buf, 6);
    set_checksum(buf, checksum);

    write(client_sockid, buf, 60);
    
    sleep(1);

    unsigned char recvdata[4096]={0};
    unsigned int readsize;
    readsize = read(client_sockid, recvdata, 4096);
    for(int i=0; i<readsize; i++)
	    printf("%x ",recvdata[i]);
    printf("\n");
    
    //first check legal packet, then process data
    if (0 == check_376packet_is_legal(recvdata))
    {
        analyse_376packet(recvdata, readsize);
    }
    else
    {
        printf("warning ! receive illegal packet1\n");
    }

    close(client_sockid);

    return 0;

}
