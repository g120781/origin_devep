#include <stdio.h>
#include <string.h>
#include "protocal_376.h"

int get_length_376(unsigned char *pdata)
{
    unsigned short *plen;
    unsigned short len;

    plen = (unsigned short*)(pdata + 1);
    
    len = NTOHS(*plen);

    return (len>>2);
}


void set_length_376(unsigned char *pdata, unsigned short length)
{
    unsigned short *plen;
    unsigned char protocal_flag = 0;
    //fix length
    length += 6;

    plen = (unsigned short*)(pdata +1);
    protocal_flag = NTOHS(*plen) & 0x3;
    *plen = HTONS((length << 2) + protocal_flag);

    plen = (unsigned short*)(pdata +3);
    protocal_flag = NTOHS(*plen) & 0x3;
    *plen = HTONS((length << 2) + protocal_flag);

    return;
}



void set_ctrl_area(unsigned char *pdata, CTL_AREA *ctl_area_struct)
{
    unsigned char *ctl_area;
    //ctl offset 6 bytes
    ctl_area = pdata + 6;
    memcpy(ctl_area, ctl_area_struct, sizeof(CTL_AREA));

    return;
}

void get_ctrl_area(unsigned char *pdata, CTL_AREA *ctl_area_struct)
{
    unsigned char *ctl_area;
    ctl_area = pdata +6;
    memcpy(ctl_area_struct, ctl_area, sizeof(CTL_AREA));
}


void set_address_area(unsigned char *pdata, ADDRESS_AREA *addr)
{
    unsigned short *A1;
    unsigned short *A2;
    unsigned char *A3;

    A1 = (unsigned short *)(pdata + 7);
    *A1 = HTONS(addr->A1);

    A2 = (unsigned short *)(pdata +9);
    *A2 = HTONS(addr->A2);

    A3 = pdata + 11;
    *A3 = addr->A3;

    return;
}



void get_address_area(unsigned char *pdata, ADDRESS_AREA *addr)
{
   unsigned short *A1;
   unsigned short *A2;
   unsigned char *A3;

   A1 = (unsigned short *)(pdata + 7);
   A2 = (unsigned short *)(pdata + 9);
   A3 = pdata + 11;

   addr->A1 = NTOHS(*A1);
   addr->A2 = NTOHS(*A2);
   addr->A3 = *A3;

   return; 
}

void add_data_to_buffer(unsigned char *pdata, unsigned char *ori_data, unsigned int length)
{
    memcpy(pdata+12, ori_data, length);
    
    return;
}



unsigned char calculate_checksum(unsigned char *pdata, unsigned short length)
{
    int i=0;
    unsigned char sum =0;
    
    for(i=6; i<6+length+6; i++)
	    sum += *(pdata+i);

    return sum;
    
}


void set_checksum(unsigned char *pdata, unsigned char checksum)
{
    unsigned short len;

    len= get_length_376(pdata);

    *(pdata + 6 + len) = checksum;

    return;
}

int get_checksum(unsigned char *pdata)
{
    unsigned short len;
    len = get_length_376(pdata);

    return *(pdata+6+len);
}

void pad_0x68(unsigned char *pdata)
{
  *pdata = 0X68;
  *(pdata + 5)= 0X68;

  return;
}


int check_376packet_is_legal(unsigned char *pdata)
{
    unsigned short len;
    unsigned char checksum1,checksum2;

    len = get_length_376(pdata);

    //check checksum
    checksum1 = calculate_checksum(pdata, len-6);
    checksum2 = get_checksum(pdata);

    if(checksum1 == checksum2)
	    return 0;
    return 1;
}

void analyse_376packet(unsigned char *pdata, int total_len)
{
    unsigned short len;
    ADDRESS_AREA addr;
    CTL_AREA ctl_area_struct;

    get_ctrl_area(pdata, &ctl_area_struct);
    get_address_area(pdata, &addr);
    printf("*******RECV PKT FORM SVC***********\n");
    for (int i=0; i<total_len; i++)
	    printf("%x ",pdata[i]);
    printf("\n");
    printf("packet_len =%d\n", get_length_376(pdata));
    printf("A1=%x, A2=%X, A3=%x\n",addr.A1,addr.A2,addr.A3);
    printf("PRM=%d, FARMA_FLAG=%d, FRMAE_CNT=%d, FUNC=%d\n", ctl_area_struct.PRM,ctl_area_struct.FRAME_FLAG, ctl_area_struct.FRAME_CNT, ctl_area_struct.FUNC);
}

#if 0
int main()
{
 // unsigned char pdata[] = {0x68,0x32,0x00,0x32,0x00,0x68,0xc9,0x01,0x32,0x02,0x00,0x00,0x02,0x71,0x00,0x00,0x01,0x00};
 // int i =0;
  
  //printf("checksum=%x\n", calculate_checksum(pdata,12));
  //for(i=0; i<sizeof(pdata); i++)
  //    printf("%x ",pdata[i]);
 // printf("\n");
  unsigned int len =0;

  unsigned char data[4096] = {0};

  unsigned char userdata[6]={0x1,0x2,0x3,0x4,0x5,0x7};

  ADDRESS_AREA a;
  //set length

  CTL_AREA b;

  int i=0;

  unsigned char checksum =0;

  pad_0x68(data);

  set_length_376(data, 6);

  a.A1 = 0x1234;
  a.A2 = 0x56;
  a.A3= 0;

  set_address_area(data, &a);

  b.DIR = 1;
  b.PRM = 1;
  b.FRAME_CNT = 0;
  b.FRAME_FLAG=0;
  b.FUNC = 0x4;
  
  set_ctrl_area(data, &b);

  add_data_to_buffer(data,userdata,6);
 
  checksum = calculate_checksum(data,6);

  set_checksum(data, checksum);

  printf("checksum=%d\n",checksum);

  for(i=0; i<60; i++)
	  printf("%x ", data[i]) ;

  printf("\n"); 
  return 0;
}
#endif
