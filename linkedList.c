#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

typedef struct nameNumber 
{
	char *name ;
	char *number ;
	struct nameNumber *next ;
} NameNumber ;


#define HASH_SIZE 100

static NameNumber *list[HASH_SIZE] ;

int getHashValue (char *name)
{
	// avoid core dumps
	if (!name)
		return (0) ;

	int Hash = strcmp (name, "My Hashing String") ;
	if (Hash < 0)
		Hash *= -1 ;

	Hash = Hash % HASH_SIZE ;
	return (Hash) ;
}


void addEntryToList(char *name, char *number) 
{
	NameNumber *new = (NameNumber *) calloc (1, sizeof (NameNumber)) ;
	
	new->name = strdup (name) ;
	new->number = strdup (number) ;

	int hash = getHashValue (new->name) ;

	// slot into beginning of list
	new->next = list[hash] ;
	list[hash] = new ;
}

NameNumber *findByName (char *name)
{
	NameNumber *ptr = NULL ;

	int hash = getHashValue (name) ;

	for (ptr = list[hash]; ptr; ptr = ptr->next)
	{
		if (!strcmp (ptr->name, name))
			return (ptr) ;
	}
	return (NULL) ;
}

void deleteVictim (NameNumber *victim) 
{
	if (victim)
	{
		free (victim->name) ;
		free (victim->number) ;
		free (victim) ;
	}
}

int deleteFromList (NameNumber *victim)
{
	NameNumber *ptr = NULL ;

	if (!victim)
		return -1;

	int hash = getHashValue (victim->name) ;

	// victim may be the first entry in the list
	if (list[hash] == victim)
	{
		list[hash] = victim->next ;
		deleteVictim (victim) ;

		// success return
		return 0 ;
	}

	for (ptr = list[hash]; ptr; ptr = ptr->next)
	{
		if (ptr->next == victim)
		{
			ptr->next = victim->next ;
			deleteVictim (victim) ;

			// success return
			return 0 ;
		}
	}
	return -1 ;
}
		
	
main (int argc, char **argv)
{
	addEntryToList ("Joe Bloggs", "+44123456789") ;
	addEntryToList ("John Doe", "+1123456789") ;
}

