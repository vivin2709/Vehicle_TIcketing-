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
 * vehicle_lookup
 *          look for vehicle in the database
 *          vehicle must match both plate and state strings
 * args
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  argv    for error printing
 *
 * returns  pointer to vehicle if found NULL otherwise
 */

struct vehicle *
vehicle_lookup(struct vehicle **hashtab, uint32_t tabsz, char *plate,
    char *state, char **argv)
{
    //Finding the Hash chain this vehicle belongs to
    uint32_t hash_value = hash(plate,argv);
    uint32_t index = hash_value%tabsz;
    //Checker flags initialised 
    int t1=0;
    int t2=0;
    //First vehicle in the required hash chain
    struct vehicle *wanted = hashtab[index]; 
    //Iterating through the hash chain's vehicles 
     while(wanted!=NULL) 
    {
        //Checking if both state and plate match
         t1= strcmp(wanted->plate,plate);
         t2= strcmp(wanted->state,state);
        //Vehicle found 
        if(t2==0&&t1==0)
            return wanted;
        //Changing pointer to move to next vehicle 
        wanted = wanted->next;
    }
    //Return NULL if not found 
    return NULL;
}
#endif
