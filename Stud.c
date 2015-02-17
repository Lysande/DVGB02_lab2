/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"
#include <string.h>

#define DEBUG 1
#define A_SIDE 0
#define B_SIDE 1
#define TIMER_INTERVAL 50

int acks = 0;
int seq_num=0;
//TODO
struct pkt p;

void print_payload(char *p)
{
  int i;
  for(i=0;i<strlen(p);i++)
  {
    printf("%c",p[i]);
  }
  printf("\n");
}

int gen_seq_num()
{ 
  if(seq_num)
    return --seq_num;
  else
    return ++seq_num;
}

int calc_checksum(struct pkt p)
{
  int i=0, sum=p.seqnum+p.acknum;
  for(i=0;i<20;i++)
  {
   //printf("sum:  %d payload[i]:%d\n",sum,p.payload[i]);
    sum=sum+p.payload[i];
    i++;
  }
  return sum;
  
}
void resend_pkt() {
  if(DEBUG)printf("resend\n");
    tolayer3(A_SIDE, p);
}

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  if(DEBUG)printf("Send A seq_num%d\n",seq_num);
  strcpy(p.payload,message.data);
  p.seqnum=seq_num;
  p.acknum=seq_num;
  p.checksum=calc_checksum(p);
  //("\nA-side checksum %d",p.checksum);
  starttimer(A_SIDE,TIMER_INTERVAL);
  printf("A deliver:");
  print_payload(p.payload);
  tolayer3(A_SIDE,p);
}

void B_output(struct msg message)  /* need be completed only for extra credit */
{}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
    
   if(packet.acknum==seq_num)
   {
    // printf("			Acknum rcv A %d\n",packet.acknum);
    stoptimer(A_SIDE);
     //TODO
    seq_num=!seq_num;
    acks++;
  if(DEBUG)printf("acks:%d\n",acks);
   }
   else
   {
     //printf("			Acknum rcv A %d\n",packet.acknum);
      stoptimer(A_SIDE);
    starttimer(A_SIDE, TIMER_INTERVAL);
    resend_pkt();
   }
}


/* called when A's timer goes off */
void A_timerinterrupt()
{
  if(DEBUG)printf("timeout\n");
  starttimer(A_SIDE,TIMER_INTERVAL);
  resend_pkt();
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
int checkseqnum(struct pkt packet)
{
  return (packet.seqnum==seq_num);
}
int verify_checksum(struct pkt packet)
{
 // printf("\npacket checksum%d",packet.checksum);
 // printf("    calc checksum%d\n",calc_checksum(packet));
  return (packet.checksum==calc_checksum(packet));
}
void send_ack(int acknum)
{
  struct pkt ack;
  ack.acknum=acknum;
  tolayer3(B_SIDE,ack);
}

void B_input(struct pkt packet)
{
  if(!checkseqnum(packet) || !verify_checksum(packet)) //discard and nack
  { 
    if(DEBUG)printf("nack sent\n");
    send_ack(!seq_num);
  }
  else
  {
    if(DEBUG)printf("ack sent\n");
    printf("B deliver:");
    print_payload(packet.payload);
    tolayer5(B_SIDE,packet.payload);
    send_ack(seq_num);
  } 
}

/* called when B's timer goes off */
void B_timerinterrupt()
{}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{}
