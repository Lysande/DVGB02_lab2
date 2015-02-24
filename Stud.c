/********* STUDENTS WRITE THE NEXT SEVEN ROUTINES *********/
 
#include "Stud.h"
#include <string.h>
 
#define DEBUG 0
#define A_SIDE 0
#define B_SIDE 1
#define TIMER_INTERVAL 50
 
int acks = 0;
int seq_num=0;
int last_sent_seq_num = 1;
//TODO
struct pkt p;

 /**
  * calc_checksum: Create a checksum from a given packet
  * 
  * pre: Packet exists
  * post: Checksum calculated and returned
  * 
  * @param  p
  * @return checksum
  */
int calc_checksum(struct pkt p)
{
  int i=0, sum=p.seq_num+p.ack_num;
  for(i=0;i<20;i++)
  {
    sum=sum+p.payload[i];
    i++;
  }
  return sum;
}

/**
 * resend_pkt: Re-sends a packet
 * pre: Packet exists and has been sent at least once
 * post: Packet is sent once again
 */
void resend_pkt() {
  if(DEBUG)printf("resend\n");
    tolayer3(A_SIDE, p);
}
 
/* called from layer 5, passed the data to be sent to other side */

/**
 * A_output: takes message from application, creates a packet, 
 *           starts the timer and sends the packet to the other side.
 * pre: message received from application layer
 * post: timer running, packet sent 
 * @param message 
 */
void A_output(struct msg message)
{
  if(DEBUG)printf("Send A seq_num%d\n",seq_num);
  strcpy(p.payload,message.data);
  p.seq_num=seq_num;
  p.ack_num=seq_num;
  p.checksum=calc_checksum(p);
  //("\nA-side checksum %d",p.checksum);
  starttimer(A_SIDE,TIMER_INTERVAL);
  if(DEBUG)printf("A deliver:");
  //print_payload(p.payload);
  tolayer3(A_SIDE,p);
}
 
void B_output(struct msg message)  /* need be completed only for extra credit */
{}
 
/**
 * verify_checksum: Compares checksum of given packet with a 
 *                  calculated checksum of the same packet.
 * pre: packet exists, has checksum
 * post: true/false returned 
 * 
 * @param  packet 
 * @return        
 */
int verify_checksum(struct pkt packet)
{
  return (packet.checksum==calc_checksum(packet));
}
 
/* called from layer 3, when a packet arrives for layer 4 */

/**
 * A_input: receives a N/ACK and either toggles seq_num, or 
 *          tries to resend the packet and restarts the timer.
 * pre:     ACK received
 * post:    Either seq_num is toggled, or the timer is restarted 
 *          and the packet sent again
 * @param packet
 */
void A_input(struct pkt packet)
{
     
   if(packet.ack_num==seq_num && verify_checksum(packet))
   {
      stoptimer(A_SIDE);
      seq_num=!seq_num;
      acks++;
      if (DEBUG) printf("ack recieved\n");
      if (DEBUG) printf("acks:%d\n",acks);
   }
   else
   {
      if (DEBUG) printf("nack recieved\n");
      stoptimer(A_SIDE);
      starttimer(A_SIDE, TIMER_INTERVAL);
      resend_pkt();
   }
}
 
 
/* called when A's timer goes off */
/**
 * A_timerinterrupt: called when timer expires
 * pre: timer expired
 * post: timer restarted, packet resent
 */
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

/**
 * check_seq_num: returns whether the seq_num in the packet matches the expected
 * pre: packet exists
 * post: returns true/false
 * 
 * @param  packet [description]
 * @return        [description]
 */
int check_seq_num(struct pkt packet)
{
  return (packet.seq_num==seq_num);
}
 
 /**
  * send_ack: Sens a packet with ack number and checksum
  * pre: packet received
  * post: ack sent 
  * 
  * @param ack_num inverted for nack
  */
void send_ack(int ack_num)
{
  struct pkt ack;
  ack.ack_num=ack_num;
  ack.checksum=calc_checksum(ack);
  tolayer3(B_SIDE,ack);
}
 
/**
 * B_input: receives a packet, verifies checksum and seq_num, 
 *           and delivers the data if they were correct.
 *           Sends an ACK if everything was successful, and a NACK if it wasnt.
 *
 * pre: packet received
 * post: packet payload delivered to application unless the packet was faulty, N/ACK sent
 * @param packet [description]
 */
void B_input(struct pkt packet)
{
  if(!check_seq_num(packet) || !verify_checksum(packet)) //discard and nack
  { 
    if(DEBUG)printf("nack sent\n");
    send_ack(!seq_num);
  }
  else
  {
    if (last_sent_seq_num != seq_num) 
    {
    if(DEBUG)printf("B deliver:");
        //print_payload(packet.payload);
 
 
      tolayer5(B_SIDE,packet.payload); 
      last_sent_seq_num = seq_num;
    }
     
    send_ack(seq_num);
     if(DEBUG)printf("ack sent\n");
  } 
}
 
/* called when B's timer goes off */
void B_timerinterrupt()
{}
 
/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{}