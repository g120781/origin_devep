#ifndef PROTOCAL_376_H
#define PROTOCAL_376_H

#define LITTLE_ENDINE 0
#define __VOS_BYTE_ORDER LITTLE_ENDINE

//376 is little_endine in network
#if defined(LITTLE_ENDINE)
#define HTONS(K) (K)
#define HTONL(K) (K)
#define NTOHS(K) (K)
#define NTOHL(K) (K)
#elif defined(BIG_ENDINE)
#define HTONS(K) ((unsigned short)K & 0xff00 >> 8 | ((unsigned short)K & 0xff) << 8)
#define HTONL(K) ((unsigned int)K & 0xff000000 >> 24 |\
		  (unsigned int)K & 0x00ff0000 >> 8 |\
		  (unsigned int)K & 0x0000ff00 << 8 |\
		  (unsigned int)K & 0xff << 24 )
#else
#error you shuold define LITTLE_ENDINE or BIG_ENDINE
#endif


typedef struct
{
#if __VOS_BYTE_ORDER == BIG_ENDINE
        unsigned char DIR:1;
	unsigned char PRM:1;
	unsigned char FRAME_CNT:1;
	unsigned char FRAME_FLAG:1;
	unsigned char FUNC:4;
#elif __VOS_BYTE_ORDER == LITTLE_ENDINE
	unsigned char FUNC:4;
	unsigned char FRAME_FLAG:1;
	unsigned char FRAME_CNT:1;
	unsigned char PRM:1;
	unsigned char DIR:1;
#else
#error __VOS_BYTE_ORDER undefined!
#endif
}CTL_AREA;

typedef struct
{
    unsigned short A1;
    unsigned short A2;
    unsigned short A3;

}ADDRESS_AREA;

/*get length area in data stream*/
int get_length_376(unsigned char *pdata);

/*set length area for data stream*/
void set_length_376(unsigned char *pdata, unsigned short length);

/*set contol area for data stream*/
void set_ctrl_area(unsigned char *pdata, CTL_AREA *ctl_area_struct) ;

/*set address area for data stream*/
void set_address_area(unsigned char *pdata, ADDRESS_AREA *addr);

/*get address area in data stream*/
void get_address_area(unsigned char *pdata, ADDRESS_AREA *addr);

/*calculate checksum*/
unsigned char calculate_checksum(unsigned char *pdata, unsigned short length);

/*add data to buffer*/
void add_data_to_buffer(unsigned char *pdata, unsigned char *ori_data, unsigned int length);


#endif
