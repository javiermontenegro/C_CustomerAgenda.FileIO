#ifndef NODE_STRING_H
#define NODE_STRING_H

/*! \file node_string.h
 *  \brief Definitions for dynammically allocated strings.
 *
 *  The string datatype is used for easy management of dynamically allocated
 *  strings. It supports creation, duplication, and IO operations of
 *  null-terminated strings.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/*! \typedef string_t
 *  \brief Type definition of a dynamically allocated string.
 *
 *  This implementation consists of a null-terminated pointer to a string
 *  and a variable to record its length.
 */
typedef struct
{
    char *s_ptr;
    unsigned long long s_len;
} string_t;

/*! \fn string_t *string_create( const char *s )
 *  \brief Creates a string.
 *  \param s The string used for initializing the string.
 *  \return On success a new string is returned. Otherwise NULL is returned
 *  and errno is set appropriately.
 *  \exception ENOMEM  Not enough memory to allocate the string.
 */
extern string_t *string_create( const char *s );

/*! \fn string_t *string_duplicate( const string_t *str )
 *  \brief Duplicates an existing string.
 *  \param str The string to be duplicated.
 *  \return On success a duplicate string is returned. Otherwise NULL
 *  is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the string.
 */
extern string_t *string_duplicate( const string_t *str );

/*! \fn void string_print( FILE *file, string_t *str )
 *  \brief Prints a string to a specified stream.
 *  \param file The stream where to print the string.
 *  \param str The string to be printed.
 */
extern void string_print( FILE *file, string_t *str );

/*! \fn string_t *string_scan( FILE *file )
 *  \brief Scans a string from a specified stream.
 *  \param file The stream where to read a string.
 *  \return On success a string with the scanned input is returned.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the string.
 */
extern string_t *string_scan( FILE *file );

/*! \fn void string_write( FILE *file, string_t *str )
 *  \brief Writes a string in binary format into a specified stream.
 *  \param file The stream where to write the string.
 *  \param str The string to be written.
 */
extern void string_write( FILE *file, string_t *str );

/*! \fn string_t *string_read( FILE *file )
 *  \brief Reads a string in binary format from a specified stream.
 *  \param file The stream where to read the string object.
 *  \return On success a new string object is returned with the input read.
 *  Otherwise NULL is returned and errno is set appropriately.
 *  \exception ENOMEM Not enough memory to allocate the string.
 */
extern string_t *string_read( FILE *file );

/*! \fn void string_destroy( string_t *str )
 *  \brief Destroys a string.
 *  \param str The string object to be destroyed.
 */
extern void string_destroy( string_t *str );

#endif /* NODE_STRING_H */
