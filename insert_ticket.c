#include <limits.h>
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "hashdb.h"
#include "readsubs.h"


//#define MYCODE
#ifndef MYCODE
TODO(USING THE SOLUTION FUNCTION NOT MY CODE)
#else

/*
 * insert_ticket
 *      add a ticket (summons) to the database
 *
 *      if the vehicle (plate state) is not in the database then
 *      a new struct vehicle entry must be made for it in the hash table.
 *      New vehicle entries are made at the the FRONT of the collison chain
 *
 *      If the vehicle is already in the hash chain, you only need to add 
 *      a new ticket to the ticket chain for this summons.
 *
 *      Tickets are added to the ticket chain linked to the struct vehicle
 *      at the END of the ticket chain.
 *
 *      Also you must update the tot_fine and cnt_tickets in the struct vehicle
 *      to account for the new summons(ticket) just added
 * args:
 *  hashtab pointer to hashtable (pointer to an array of pointers)
 *  tabsz   number of elements in the hash table
 *          example hashval = hash(plate, argv) % tabsz;
 *  fineTab pointer fine table. maps code number to text description & fine cost 
 *          you use this table to get the amount of the fine when calculating
 *          the tot_fine field in struct vehicle.
 *          example:
 *              struct vehicle *vhpt;
 *              vhpt->tot_fine += fineTab[code].fine;
 *  summ    summons id string to be added
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string         
 *          Conversion function is supplied. use example:
 *               long long summid;
 *               if (strtosumid(summ, argv, &summid) != 0)
 *                  error handling
 *               new_ticket->summons = summid;
 *  plate   plate id string to be added
 *  state   state id string to be added
 *  date    date of summons string
 *          this string needs to be converted to a number to be stored in the
 *          database. This saves space and is faster to compare than a string
 *          The encoding into a number uses Linux time format.
 *          Conversion function is supplied. use example:
 *              time_t dateval;
 *              if (strtoDate(date, &dateval, argv) != 0)
 *                  error handling
 *              new_ticket->date = dateval;
 *  code    summons code integer value used an an index into the fines table
 *  argv    for error printing
 *
 * returns 0 if ok -1 for all errors
 */

int
insert_ticket(struct vehicle **hashtab, uint32_t tabsz, struct fine *fineTab,
 char *summ,char *plate, char *state, char *date, int code, char **argv)
{
        //Checking if summons ID is valid 
        unsigned long long summid;
        if (strtosumid(summ, &summid, argv) != 0)
           return -1;
        //Checking if date is valid 
          time_t dateval;
        if (strtoDate(date, &dateval, argv) != 0)
            return -1;
        //Creating ticket to be added to database 
        struct ticket* ticket_to_add= malloc(sizeof(*ticket_to_add));
        ticket_to_add->summons=summid;
        ticket_to_add->date=dateval;
        ticket_to_add->next=NULL;
        ticket_to_add->code=code;

        uint32_t hash_value = hash(plate,argv);

    if(vehicle_lookup(hashtab, tabsz, plate, state, argv)==NULL)
    {
    //Creating vehicle struct to be added if not already present in database
	    struct vehicle* insert_vehicle=malloc(sizeof(*insert_vehicle));

        insert_vehicle->head=NULL;
        insert_vehicle->state=strdup(state);
        insert_vehicle->plate=strdup(plate);
        insert_vehicle->tot_fine=0;
        insert_vehicle->cnt_ticket= 0;
        insert_vehicle->next=NULL;

        //Finding Which hash chain to insert new vehicle into 
        uint32_t index = hash_value%tabsz;
        //Adding new vehicle to hash chain 
        insert_vehicle->next=hashtab[index];
        hashtab[index]=insert_vehicle;
        //Adding first ticket to new vehicle 
        insert_vehicle->head=ticket_to_add;
        //Updating vehicle's attribute to reflect the new ticket 
        insert_vehicle->tot_fine += fineTab[code].fine;
        insert_vehicle->cnt_ticket++;
        return 0;
    }
    //When vehicle already exists in the databse 
    else if(vehicle_lookup(hashtab, tabsz, plate, state, argv)!=NULL)
    {
        //Finding the vehicle to which ticket is added 
        struct vehicle* wanted = vehicle_lookup(hashtab, tabsz, 
        plate, state, argv);
        //Setting pointer to that vehicle's ticket chain 
        struct ticket* tptr=wanted->head;
        //Creaeting acopy pointer of tptr which follows tptr
        struct ticket* tptr_copy = tptr;
        //Iterating through ticket chain of vehicle 
         while(tptr!=NULL) 
        {
            //Copying pointer in every iteration (copy is one ticket behind)
            tptr_copy=tptr;
            tptr=tptr->next;
        }
        //Adding the ticket to vehicle's head if 
        if(tptr_copy==NULL&&wanted->head==NULL)
            wanted->head=ticket_to_add;
        //If pointer copy is at the end of the chain add new ticket 
        else if(tptr_copy!=NULL)
            tptr_copy->next=ticket_to_add;
        //Setting added ticket's next to NULL since it as at the end of chain.
        ticket_to_add->next=NULL;
        //Updating vehicle's attributes to reflect new ticket
        wanted->tot_fine += fineTab[code].fine;
        wanted->cnt_ticket++;
        return 0;
    }
    else
        return 0;
}
#endif
