/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/

#include "Stud.h"
#include <string.h>
int seq_num=0;
//TODO
struct pkt p;

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
  while(p.payload[i]!=0)
  {
    sum=sum+p.payload[i];
    i++;
  }
  return sum;
  
}

/* called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  
  strcpy(p.payload,message.data);
  p.seqnum=seq_num;
  p.acknum=seq_num;
  p.checksum=calc_checksum(p);
  printf("\nA-side checksum %d",p.checksum);
  tolayer3(0,p);
}

void B_output(struct msg message)  /* need be completed only for extra credit */
{}

/* called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{}


/* called when A's timer goes off */
void A_timerinterrupt()
{}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{}


/* Note that with simplex transfer from a-to-B, there is no B_output() */

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_input(struct pkt packet)
{
  printf("\nB side:                            %d",packet.checksum);
}

/* called when B's timer goes off */
void B_timerinterrupt()
{}

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{}
