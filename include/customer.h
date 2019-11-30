#ifndef CUSTOMER_H
#define CUSTOMER_H

/*! \file customer.h
 *  \brief Definitions for customer agenda manipulation.
 *
 *  The customer datatype is implemented as a bag using a linked list. It
 *  supports CRUD opertions of entries as well as IO operations for storing
 *  and retrieving from a file.
 */

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "node_string.h"
#include "node_entry.h"

/*! \typedef entry_node_t
 *  \brief Type definition for a nodes of a linked list.
 *
 *  This is an implementation of a node that contains a previous and next
 *  link in order to implement a doubly linked list.
 */
typedef struct entry_node
{
    struct entry_node *n_prev;
    entry_t *n_entry;
    struct entry_node *n_next;
} entry_node_t;

/*! \typedef customer_t
 *  \brief Type definition of an customer agenda.
 *
 *  This is a doubly linked list implementation that contains a head and
 *  tail.
 */
typedef struct
{
    entry_node_t *a_head;
    entry_node_t *a_tail;
} customer_t;

/*! \fn customer_t *customer_create( void )
 *  \brief Creates an customer agenda.
 *  \return On success an customer agenda is returned. Otherwise NULL is
 *  returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the customer agenda.
 */
extern customer_t *customer_create( void );

/*! \fn customer_T *customer_duplicate( const customer_t *customer )
 *  \brief Duplicates an customer agenda.
 *  \param customer The customer agenda to be duplicated.
 *  \return On success a duplicate customer agenda and its entries are
 *  returned. Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the customer agenda or its
 *  entries.
 */
extern customer_t *customer_duplicate( const customer_t *customer );

/*! \fn customer_t *customer_search( customer_t *customer, const char *firstname, const char *lastname )
 *  \brief Searches for entries by first and last names.
 *  \param customer The customer agenda for which to search.
 *  \param firstname A null-terminated string containing the value for
 *  first name.
 *  \param lastname A null-terminated string containig the value for last
 *  name.
 *  \return On success a new customer agenda containing matched entries is
 *  returned. Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the customer agenda or
 *  duplicate entries.
 */
extern customer_t *customer_search( customer_t *customer, const char *firstname, const char *lastname );

/*! \fn customer_t *customer_get( const customer_t *customer, unsigned index  )
 *  \brief Gets an entry node from the customer agenda given its index.
 *  \param customer The customer agenda from which the entry will be retrieve.
 *  \param index A zero-based index of the entry in the customer agenda.
 *  \return On success the correspoding entry node is returned. Otherwise
 *  NULL is returned and errno is set appropriately.
 *  \exception EINVAL An invalid number was provided.
 */
extern entry_node_t *customer_get( const customer_t *customer, unsigned index );

/*! \fn void customer_write( FILE *file, customer_t *customer )
 *  \brief Writes an customer agenda in binary format to a specified stream.
 *  \param file The stream where to write the customer agenda.
 *  \param customer The customer agenda to be written.
 */
extern void customer_write( FILE *file, customer_t *customer );

/*! \fn void customer_t *customer_read( FILE *file )
 *  \brief Reads an customer agenda in binary format from a specified stream.
 *  \param file The stream from where to read the entry.
 *  \return On success a new customer agenda with read values is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memeory to allocate the entry.
 */
extern customer_t *customer_read( FILE *file );

/*! \fn int customer_add( customer_t *customer, entry_t *entry )
 *  \brief Duplicates and adds an entry to the end of an customer agenda.
 *  \param customer The customer agenda for which an entry is to be added.
 *  \param entry The entry to be duplicated and added.
 *  \return On success the entry is added and zero is returned. Otherwise -1
 *  is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate an entry node or
 *  duplicate an entry.
 */
extern int customer_add( customer_t *customer, entry_t *entry );

/*! \fn int customer_add_all( customer_t *customer, customer_t *some_customer )
 *  \brief Duplicates and adds all entries from another customer agenda.
 *  \param customer The customer agenda for which entries are too be added.
 *  \param some_customer The customer agenda containing all entries to be added.
 *  \return On success all entries are added and zero is returned. Otherwise
 *  -1 is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate an entry node or
 *  duplicate an entry.
 */
extern int customer_add_all( customer_t *customer, customer_t *some_customer );

/*! \fn int customer_add_many customer_add_many( customer_t *customer, int count, ... )
 *  \brief Duplicates and adds many entries into an customer agenda at once.
 *  \param customer The customer agenda for which the entries are to be added.
 *  \param count The number of arguments to follow.
 *  \return On success all entries are added and zero is returned. Otherwise
 *  -1 is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate an entry node or
 *  duplicate an entry.
 */
extern int customer_add_many( customer_t *customer, int count, ... );

/*! \fn customer_t *customer_find_by_firstname( const customer_t *customer, const char *firstname )
 *  \brief Finds entries in an customer agenda by first name.
 *  \param customer The customer agenda from which entries are to be searched.
 *  \param firstname A null-terminated string containing the value for first name.
 *  \return On success an customer agenda containing found entries is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate a new customer agenda or
 *  entry nodes.
 */
extern customer_t *customer_find_by_firstname( const customer_t *customer, const char *firstname );

/*! \fn customer_t *customer_find_by_lastname( const customer_t *customer, const char *lastname )
 *  \brief Finds entries in an customer agenda by last name.
 *  \param customer The customer agenda from which entries are to be searched.
 *  \param lastname A null-terminated string containing the value for last name.
 *  \return On success an customer agenda containing found entries is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate a new customer agenda or
 *  entry nodes.
 */
extern customer_t *customer_find_by_lastname( const customer_t *customer, const char *lastname );

/*! \fn customer_t *customer_find_by_company( const customer_t *customer, const char *company )
 *  \brief Finds entries in an customer agenda by company name.
 *  \param customer The customer agenda from which entries are to be searched.
 *  \param company A null-terminated string containing the value for company name.
 *  \return On success an customer agenda containing found entries is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate a new customer agenda or
 *  entry nodes.
 */
extern customer_t *customer_find_by_company( const customer_t *customer, const char *company );

/*! \fn entry_t *customer_remove( customer_t *customer, entry_node_t *entry_node )
 *  \brief Removes an entry from an customer agenda.
 *  \param customer The customer agenda for which an entry is to be removed.
 *  \param entry_node The entry node to be removed from the customer agenda.
 *  \return On success the entry is removed from the list and returned.
 *  Otherwise NULL is returned.
 */
extern entry_t *customer_remove( customer_t *customer, entry_node_t *entry_node );

/*! \fn customer_t *customer_remove_all( customer_t *customer, customer_t *some_customer )
 *  \brief Removes all entries from an customer agenda.
 *  \param customer The customer agenda from which entries are to be removed.
 *  \param some_customer The customer agenda containing all entries to be removed.
 *  \return On success an customer agenda containing all removed entries is
 *  returned. Otherwise NULL is returned.
 */
extern customer_t *customer_remove_all( customer_t *customer, customer_t *some_customer );

/*! \fn customer_t *customer_destroy( customer_t *customer, int all )
 *  \brief Destroys an customer agenda and its entries.
 *  \param customer The customer agenda to be destroyed.
 *  \param all An integer signifying whether or not entries are to be destroyed.
 */
extern void customer_destroy( customer_t *customer, int all );

#endif /* customer_H */
