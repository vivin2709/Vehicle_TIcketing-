#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"

//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * free_tickets
 *      tears down the database freeing all allocated memory
 *
 *      go down each hash chain and free all the vehicles and the tickets
 *      for each vehicle
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 */

void
free_tickets(struct vehicle **hashtab, uint32_t tabsz)
{
    struct vehicle* wanted; 
  
      int counter=0;

    for(uint32_t chain_number=0; chain_number<tabsz;chain_number++)
    {
      //Accessing the start of each hashchain 
      wanted=hashtab[chain_number];
      //Setting copy of vehicle 
      struct vehicle* wanted_cpy=wanted; 

      while(wanted!=NULL)
      {
         //Accessing the ticket of each vehicle 
         struct ticket* tptr=wanted->head;
         struct ticket* tptr_cpy =tptr;
         //Iterating thorugh all tickets 
         while(tptr!=NULL)
         {
            counter++;
            //Saving copy pointer to current tptr
            tptr_cpy=tptr;
            tptr=tptr->next;
            //Freeing all tickets in the ticket chain for each vehicle 
            free(tptr_cpy);
            tptr_cpy=NULL;
         }
         //Moving pointers to next vehicle 
         wanted_cpy=wanted;
         wanted=wanted->next;

        //Freeing the Vehicle's plate pointer
        free(wanted_cpy->plate);
        wanted_cpy->plate=NULL;

        //Freeing the Vehicle's state pointer
        free(wanted_cpy->state);
        wanted_cpy->state=NULL;

        //Freeing the vehicle 
        free(wanted_cpy);
        wanted_cpy=NULL;
      }
    }
    //Freeing entire hashtable 
    free(hashtab);
    hashtab=NULL;
   //Print statement to ouput all freed tickets 
     printf("Total tickets freed: %u\n", counter);
}
#endif
