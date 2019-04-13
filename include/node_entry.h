#ifndef NODE_ENTRY_H
#define NODE_ENTRY_H

/*! \file node_entry.h
 *  \brief Definitions for memory allocated entries.
 *
 *  The entry datatype supports the storage of attributes pertaining to
 *  entries from an customer agenda, such as first name, last name, phone
 *  numbers and physical addresses.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "node_string.h"

/*! \typedef address_t
 *  \brief Type definition for physical addresses.
 */
typedef struct
{
    string_t *a_line1;
    string_t *a_line2;
    string_t *a_city;
    string_t *a_state;
    string_t *a_zipcode;
    string_t *a_country;
} address_t;

/*! \typedef entry_t
 *  \brief Type definition for customer agenda entries.
 */
typedef struct
{
    string_t    *e_firstname;
    string_t    *e_lastname;
    string_t    *e_telephone;
    string_t    *e_cellphone;
    string_t    *e_email;
    address_t   *e_homeaddr;
    string_t    *e_company;
    string_t    *e_jobtitle;
    string_t    *e_workphone;
    string_t    *e_workemail;
    address_t   *e_compaddr;
    string_t    *e_details;
} entry_t;

/*! \fn address_t *address_create( void )
 *  \brief Creates an address.
 *  \return On success a new address is returned with its members empty.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the address.
 */
extern address_t *address_create( void );

/*! \fn address_t *address_duplicate( const address_t *addr )
 *  \brief Duplicates an address.
 *  \param addr The address to be duplicated.
 *  \return On success a duplicate address is returned. Otherwise NULL is
 *  returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the address.
 */
extern address_t *address_duplicate( const address_t *addr );

/*! \fn void address_set_line1( address_t *addr, string_t *line1 )
 *  \brief Sets member line1 of address structure.
 *  \param addr The address to be modified.
 *  \param line1 A string containing a value for line1.
 */
extern void address_set_line1( address_t *addr, string_t *line1 );

/*! \fn void address_set_line2( address_t *addr, string_t *line2 )
 *  \brief Sets member line2 of address structure.
 *  \param addr The address to be modified.
 *  \param line2 A string containing a value for line2.
 */
extern void address_set_line2( address_t *addr, string_t *line2 );

/*! \fn void address_set_city( address_t *addr, string_t *city )
 *  \brief Sets member city of address structure.
 *  \param addr The address to be modified.
 *  \param city A string containing a value for city.
 */
extern void address_set_city( address_t *addr, string_t *city );

/*! \fn void address_set_state( address_t *addr, string_t *state )
 *  \brief Sets member state of address structure.
 *  \param addr The address to be modified.
 *  \param state A string containing a value for state.
 */
extern void address_set_state   ( address_t *addr, string_t *state   );

/*! \fn void address_set_zipcode( address_t *addr, string_t *zipcode )
 *  \brief Sets member zipcode of address structure.
 *  \param addr The address to be modified.
 *  \param zipcode A string containing a value for zipcode.
 */
extern void address_set_zipcode ( address_t *addr, string_t *zipcode );

/*! \fn void address_set_country( address_t *addr, string_t *country )
 *  \brief Sets member country of address structure.
 *  \param addr The address to be modified.
 *  \param country A string containing a value for country.
 */
extern void address_set_country ( address_t *addr, string_t *country );

/*! \fn string_t *address_get_line1( address_t *addr )
 *  \brief Gets member line1 of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for line1.
 */
extern string_t *address_get_line1( address_t *addr );

/*! \fn string_t *address_get_line2( address_t *addr )
 *  \brief Gets member line2 of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for line2.
 */
extern string_t *address_get_line2( address_t *addr );

/*! \fn string_t *address_get_city( address_t *addr )
 *  \brief Gets member city of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for city.
 */
extern string_t *address_get_city( address_t *addr );

/*! \fn string_t *address_get_state( address_t *addr )
 *  \brief Gets member state of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for state.
 */
extern string_t *address_get_state( address_t *addr );

/*! \fn string_t *address_get_state( address_t *addr )
 *  \brief Gets member state of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for state.
 */
extern string_t *address_get_state( address_t *addr );

/*! \fn string_t *address_get_zipcode( address_t *addr )
 *  \brief Gets member zipcode of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for zipcode.
 */
extern string_t *address_get_zipcode( address_t *addr );

/*! \fn string_t *address_get_country( address_t *addr )
 *  \brief Gets member zipcode of address structure.
 *  \param addr The address to be accessed.
 *  \return A duplicate string containing the value for country.
 */
extern string_t *address_get_country( address_t *addr );

/*! \fn void address_destroy( address_t *addr )
 *  \brief Destroys an address.
 *  \param addr The address to be destroyed.
 */
extern void address_destroy( address_t *addr );

/*! \fn entry_t *entry_create( void )
 *  \brief Creates an entry.
 *  \return On success a new entry is returned with its members empty.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_create( void );

/*! \fn entry_t *entry_duplicate( const entry_t *entry )
 *  \brief Duplicates an entry.
 *  \param entry The entry to be duplicated.
 *  \return On success a duplicate entry is returned. Otherwise NULL is
 *  returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_duplicate ( const entry_t *entry );

/*! \fn void entry_print( FILE *file, entry_t *entry )
 *  \brief Prints an entry to a specified stream.
 *  \param file The stream where to print the entry.
 *  \param entry The entry to be printed.
 */
extern void entry_print( FILE *file, entry_t *entry );

/*! \fn entry_t *entry_scan( FILE *file )
 *  \brief Scans an entry from a specified stream.
 *  \param file The stream from where to scan the entry.
 *  \return On success a new entry with the scanned values is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_scan( FILE *file );

/*! \fn void entry_write( FILE *file, entry_t *entry )
 *  \brief Writes an entry in binary format to a specifed stream.
 *  \param file The stream where to write the entry.
 *  \param entry The entry to be written.
 */
extern void entry_write( FILE *file, entry_t *entry );

/*! \fn entry_t *entry_read( FILE *file )
 *  \brief Reads an entry in binary format from a specified stream.
 *  \param file The stream from where to read the entry.
 *  \return On success a new entry with read values is returned. Otherwise
 *  NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the entry.
 */
extern entry_t *entry_read( FILE *file );

/*! \fn void entry_set_firstname( entry_t *entry, string_t *firstname )
 *  \brief Sets member firstname of entry structure.
 *  \param entry The entry to be modified.
 *  \param firstname A string containing a value for firstname.
 */
extern void entry_set_firstname( entry_t *entry, string_t *firstname );

/*! \fn void entry_set_lastname( entry_t *entry, string_t *lastname )
 *  \brief Sets member lastname of entry structure.
 *  \param entry The entry to be modified.
 *  \param lastname A string containing a value for lastname.
 */
extern void entry_set_lastname( entry_t *entry, string_t *lastname );

/*! \fn void entry_set_telephone( entry_t *entry, string_t *telephone )
 *  \brief Sets member telephone of entry structure.
 *  \param entry The entry to be modified.
 *  \param telephone A string containing a value for telephone.
 */
extern void entry_set_telephone( entry_t *entry, string_t *telephone );

/*! \fn void entry_set_cellphone( entry_t *entry, string_t *cellphone )
 *  \brief Sets member cellphone of entry structure.
 *  \param entry The entry to be modified.
 *  \param cellphone A string containing a value for cellphone.
 */
extern void entry_set_cellphone( entry_t *entry, string_t *cellphone );

/*! \fn void entry_set_email( entry_t *entry, string_t *email )
 *  \brief Sets member email of entry structure.
 *  \param entry The entry to be modified.
 *  \param email A string containing a value for email.
 */
extern void entry_set_email( entry_t *entry, string_t *email );

/*! \fn void entry_set_homeaddr( entry_t *entry, address_t *homeaddr )
 *  \brief Sets member homeaddr of entry structure.
 *  \param entry The entry to be modified.
 *  \param homeaddr An address containing a value for home address.
 */
extern void entry_set_homeaddr( entry_t *entry, address_t *homeaddr );

/*! \fn void entry_set_company( entry_t *entry, string_t *company )
 *  \brief Sets member company of entry structure.
 *  \param entry The entry to be modified.
 *  \param company A string containing a value for company.
 */
extern void entry_set_company( entry_t *entry, string_t *company );

/*! \fn void entry_set_jobtitle( entry_t *entry, string_t *jobtitle )
 *  \brief Sets member jobtitle of entry structure.
 *  \param entry The entry to be modified.
 *  \param jobtitle A string containing a value for job title.
 */
extern void entry_set_jobtitle( entry_t *entry, string_t *jobtitle );

/*! \fn void entry_set_workphone( entry_t *entry, string_t *workphone )
 *  \brief Sets member workphone of entry structure.
 *  \param entry The entry to be modified.
 *  \param workphone A string containing a value for work phone.
 */
extern void entry_set_workphone( entry_t *entry, string_t *workphone );

/*! \fn void entry_set_workemail( entry_t *entry, string_t *workemail )
 *  \brief Sets member workemail of entry structure.
 *  \param entry The entry to be modified.
 *  \param workemail A string containing a value for work email.
 */
extern void entry_set_workemail( entry_t *entry, string_t *workemail );

/*! \fn void entry_set_compaddr( entry_t *entry, address_t *compaddr )
 *  \brief Sets member compaddr of entry structure.
 *  \param entry The entry to be modified.
 *  \param compaddr An address containing a value for company address.
 */
extern void entry_set_compaddr( entry_t *entry, address_t *compaddr );

/*! \fn void entry_set_details( entry_t *entry, string_t *details )
 *  \brief Sets member details of entry structure.
 *  \param entry The entry to be modified.
 *  \param details A string containing a value for entry details.
 */
extern void entry_set_details( entry_t *entry, string_t *details );

/*! \fn string_t *entry_get_firstname( entry_t *entry )
 *  \brief Gets member firstname of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for firstname.
 */
extern string_t *entry_get_firstname( entry_t *entry );

/*! \fn string_t *entry_get_lastname( entry_t *entry )
 *  \brief Gets member lastname of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for lastname.
 */
extern string_t *entry_get_lastname( entry_t *entry );

/*! \fn string_t *entry_get_telephone( entry_t *entry )
 *  \brief Gets member telephone of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for telephone.
 */
extern string_t *entry_get_telephone( entry_t *entry );

/*! \fn string_t *entry_get_cellphone( entry_t *entry )
 *  \brief Gets member cellphone of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for cellphone.
 */
extern string_t *entry_get_cellphone( entry_t *entry );

/*! \fn string_t *entry_get_email( entry_t *entry )
 *  \brief Gets member email of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for email.
 */
extern string_t *entry_get_email( entry_t *entry );

/*! \fn address_t *entry_get_homeaddr( entry_t *entry )
 *  \brief Gets member homeaddr of entry structure.
 *  \param entry The entry to be accessed.
 *  \return An address containing the value for homeaddr.
 */
extern address_t *entry_get_homeaddr( entry_t *entry );

/*! \fn string_t *entry_get_company( entry_t *entry )
 *  \brief Gets member company of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for company.
 */
extern string_t *entry_get_company( entry_t *entry );

/*! \fn string_t *entry_get_jobtitle( entry_t *entry )
 *  \brief Gets member jobtitle of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for jobtitle.
 */
extern string_t *entry_get_jobtitle( entry_t *entry );

/*! \fn string_t *entry_get_workphone( entry_t *entry )
 *  \brief Gets member workphone of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for workphone.
 */
extern string_t *entry_get_workphone( entry_t *entry );

/*! \fn string_t *entry_get_workemail( entry_t *entry )
 *  \brief Gets member workemail of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for workemail.
 */
extern string_t *entry_get_workemail( entry_t *entry );

/*! \fn address_t *entry_get_compaddr( entry_t *entry )
 *  \brief Gets member compaddr of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A address containing the value for compaddr.
 */
extern address_t *entry_get_compaddr( entry_t *entry );

/*! \fn string_t *entry_get_details( entry_t *entry )
 *  \brief Gets member details of entry structure.
 *  \param entry The entry to be accessed.
 *  \return A string containing the value for details.
 */
extern string_t *entry_get_details( entry_t *entry );

/*! \fn void entry_destroy( entry_t *entry )
 *  \brief Destroys an entry.
 *  \param entry The entry to be destroyed.
 */
extern void entry_destroy( entry_t *entry );

#endif /* NODE_ENTRY_H */
