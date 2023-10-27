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
 * del_ticket
 *      remove ticket (summons) from the database
 *      look up the summons for a vehicle and pay it (remove the ticket) 
 *
 *      Find the vehicle by the state and plate. if not found return -1
 *
 *      convert the summ string to a long long int using
 *      using strtosumid. This long long int is how the summons number
 *      is stored in the database in struct ticket.
 *      After being converted you can use it find a summons id match
 *      in the linked list of tickets attached to the strcut vehicle.      
 *
 *      After you have the vehicle, find the summons on the ticket chain
 *      that matches the converted summons string (summid in the example)
 *      example:
 *        unsigned long long summid
 *        if (strtosumid(summ, &summid, argv) != 0)
 *           return -1;
 *        now search the ticket chain to find that summons
 *        struct ticket *tptr;
 *        if (tptr->summons == summid)
 *          found it
 *           
 *      If found, delete it from the ticket chain. If the vehicle
 *      no longer has any tickets, then remove the vehicle from the hash chain
 *      Otherwise you must update the tot_fine and cnt_tickets fields in
 *      the struct vehicle so the are accurate after removing the summons
 *      Make sure to delete all space that malloc()'d 
 *
 * Inputs:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *  fineTab table mapping fine code to description and fine   
 *  plate   plate id string to be found
 *  state   state id string to be found
 *  summ    summon id string to be found
 *  argv    for error printing
 *
 * returns 0 if ok -1 if not found or error
 */

int
del_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
    char *plate, char *state, char *summ, char **argv)
{
    uint32_t hash_value = hash(plate,argv);
    uint32_t index = hash_value%tabsz;
    int t1=0;
    int t2=0;
    int vehicle_checker=0;
    int ticket_checker=0;

    //Finding the correct hash chain for the vehicle to be found 
    struct vehicle *wanted_new = hashtab[index]; 
    struct vehicle *wanted_new_prev=wanted_new;
    
    //Iterating through the hash chain's vehicles 
     while(wanted_new!=NULL)
    {
        //Checking if both state and plate match
         t1= strcmp(wanted_new->plate,plate);
         t2= strcmp(wanted_new->state,state);
        //Vehicle Found
        if(t2==0&&t1==0)
        {
            vehicle_checker=1;
            break;
        }
        //Updating pointers to iterate through hash chain 
        wanted_new_prev=wanted_new;
        wanted_new = wanted_new->next;
    }
    //If vehicle not found return 0
    if(vehicle_checker==0)
        return -1;
    
    else 
    {
            // Setting Pointer to first ticket in the chain 
            struct ticket *tptr=wanted_new->head;  
            //Pointer to be one ticket behind tptr 
            struct ticket* tptr_prev = tptr;
            unsigned long long summid;
            //Checking if Summons ID is valid 
            if (strtosumid(summ, &summid, argv) != 0)
                return -1;
        //Iterating through the ticket chain
        while(tptr!=NULL) 
        {
        //If ticket is found with same summons ID 
        if (tptr->summons==summid)
            {
                //Setting flag to 1 since ticket is found 
                ticket_checker=1;
                //Updating the vehicle number of tickets and total count 
                wanted_new->cnt_ticket--;
                wanted_new ->tot_fine -= fineTab[tptr->code].fine;

                /*
                For any other ticket being removed pointers skip current tptr
                set prev's next to current's next
                */ 

                if(tptr!=wanted_new->head)
                {
                    tptr_prev->next=tptr->next;
                    break;
                }
                /*
                If the ticket being removed is the head replace head's next 
                with deleted ticket's next
                */ 
                else
                {
                    wanted_new->head=tptr->next;
                    break;
                }
            }   
            //Updating pointers to move to the next ticket
            tptr_prev=tptr;
            tptr=tptr->next;
        }
        //If ticket not found return -1
        if(ticket_checker!=1)
            return -1;
        //Freeing the tptr pointer and setting to NULL,defensive progrmming
        free(tptr);
        tptr=NULL;

        /*
        When the last ticket is being removed, remove vehicle from databse
        */
        struct ticket* first_tckt=wanted_new->head;
        struct vehicle* curr_next_vehicle= wanted_new->next;
        if(first_tckt==NULL)
        {
            //For when the vehicle being removed is first in its hash chain
            if (wanted_new == hashtab[index])
                    hashtab[index]=curr_next_vehicle;
            else // For Any other vehicle to be removed from chain
                    wanted_new_prev->next=curr_next_vehicle;
            //Freeing state pointer 
            free(wanted_new->state);
            wanted_new->state=NULL;

            //Freeing Vehicle's plate pointer
            free(wanted_new->plate);
            wanted_new->plate=NULL;

            //Freeing vehicle pointer 
            free(wanted_new);            
            wanted_new=NULL;
            return 0;
        }

    return 0;
    }
}
#endif
