#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <termios.h>
#include <signal.h>
#include <errno.h>
#include <customer.h>

#define MAXLENGTH   512

char FILENAME[ MAXLENGTH ]; 
typedef enum
{
    ADD=1,
    DISPLAY,
    SEARCH,
    FIND_BY_FIRSTNAME,
    FIND_BY_LASTNAME,
    FIND_BY_COMPANY,
    EDIT,
    DELETE
} option_t;

struct termios saved_term;
static int login( void );
static entry_t *entry_prompt( void );
static void restore_terminal( void );
static void sigint_handler( int sig );

int main( int argc, char *argv[ ] )
{
    int logged;

    logged = login( );

    if( logged == 1 )
    {
        customer_t *customer;
        FILE *file;
        int option;


        printf( "User logged in successful\n" );

        if( ( file = fopen( FILENAME, "r" ) ) == NULL )
        {
            if( errno == ENOENT )
            {
                customer = customer_create( );
            }
            else
            {
                perror( "fopen" );
                return EXIT_FAILURE;
            }
        }
        else
        {
            customer = customer_read( file );
            fclose( file );
        }

        do {
            printf( "\
[1] Add new entry\n\
[2] Display a list of all entries\n\
[3] Find entries by first and last name\n\
[4] Find entries by first name only\n\
[5] Find entries by last name only\n\
[6] Find entries by company name\n\
[7] Edit an entry\n\
[8] Delete an entry\n\
[0] Exit\n\
--> " );
            scanf( "%d", &option );
            while( getchar( ) != '\n' );

            switch( option )
            {
                case ADD:
                    {
                        entry_t *entry;

                        entry = entry_prompt( );

                        if( customer_add( customer, entry ) == -1 )
                        {
                            perror( "customer_add" );
                        }
                        else
                        {
                            printf( "Contact added to customer agenda!\n" );
                            entry_destroy( entry );
                        }
                    } break;
                case DISPLAY:
                    {
                        entry_node_t *it;

                        it = customer->a_head;

                        while( it != NULL )
                        {
                            printf( "%s, %s (%s)\n",
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );

                            it = it->n_next;
                        }
                    } break;
                case SEARCH:
                    {
                        char firstname[ MAXLENGTH ],
                             lastname[ MAXLENGTH ];
                        customer_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter first name: " );
                        scanf( "%[^\n]", firstname );
                        while( getchar( ) != '\n' );

                        printf( "Enter last name: " );
                        scanf( "%[^\n]", lastname );
                        while( getchar( ) != '\n' );

                        result = customer_search( customer, firstname, lastname );
                        printf( "List of entries found\n" );
                        it = result->a_head;
                        count = 0;

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s %s\"\n", firstname, lastname );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] First name\n\
[ 2] Last name\n\
[ 3] Telephone\n\
[ 4] Cellphone\n\
[ 5] Personal e-mail\n\
[ 6] Personal address\n\
[ 7] Company name\n\
[ 8] Job title\n\
[ 9] Work phone\n\
[10] work e-mailn\n\
[11] Company address\n\
[12] Details\n\
[13] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *firstname;

                                                        printf( "Enter first name: " );
                                                        firstname = string_scan( stdin );

                                                        entry_set_firstname( entry, firstname );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *lastname;

                                                        printf( "Enter last name: " );
                                                        lastname = string_scan( stdin );

                                                        entry_set_lastname( entry, lastname );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *telephone;

                                                        printf( "Enter telephone: " );
                                                        telephone = string_scan( stdin );

                                                        entry_set_telephone( entry, telephone );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *cellphone;

                                                        printf( "Enter cellphone: " );
                                                        cellphone = string_scan( stdin );

                                                        entry_set_cellphone( entry, cellphone );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *email;

                                                        printf( "Enter personal e-mail: " );
                                                        email = string_scan( stdin );

                                                        entry_set_email( entry, email );
                                                    } break;
                                                case 6:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_homeaddr( entry, address );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *company;

                                                        printf( "Enter company name: " );
                                                        company = string_scan( stdin );

                                                        entry_set_company( entry, company );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *jobtitle;

                                                        printf( "Enter job title: " );
                                                        jobtitle = string_scan( stdin );

                                                        entry_set_jobtitle( entry, jobtitle );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *workphone;

                                                        printf( "Enter work phone: " );
                                                        workphone = string_scan( stdin );

                                                        entry_set_workphone( entry, workphone );
                                                    } break;
                                                case 10:
                                                    {
                                                        string_t *workemail;

                                                        printf( "Enter work e-mail: " );
                                                        workemail = string_scan( stdin );

                                                        entry_set_workemail( entry, workemail );
                                                    } break;
                                                case 11:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_compaddr( entry, address );
                                                    } break;
                                                case 12:
                                                    {
                                                        string_t *details;

                                                        printf( "Details: " );
                                                        details = string_scan( stdin );

                                                        entry_set_details( entry, details );
                                                    } break;
                                                case 13:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        it = customer->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        customer_remove( customer, it );
                                        printf( "Contact removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        unsigned index;
                                        entry_t *entry;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        customer_destroy( result, 0 );
                    } break;
                case FIND_BY_FIRSTNAME:
                    {
                        char firstname[ MAXLENGTH ];
                        customer_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter first name: " );
                        scanf( "%[^\n]", firstname );
                        while( getchar( ) != '\n' );

                        result = customer_find_by_firstname( customer, firstname );
                        it = result->a_head;
                        count = 0;

                        printf( "List of entries found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s\"", firstname );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] First name\n\
[ 2] Last name\n\
[ 3] Telephone\n\
[ 4] Cellphone\n\
[ 5] Personal e-mail\n\
[ 6] Personal address\n\
[ 7] Company name\n\
[ 8] Job title\n\
[ 9] Work phone\n\
[10] work e-mailn\n\
[11] Company address\n\
[12] Details\n\
[13] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *firstname;

                                                        printf( "Enter first name: " );
                                                        firstname = string_scan( stdin );

                                                        entry_set_firstname( entry, firstname );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *lastname;

                                                        printf( "Enter last name: " );
                                                        lastname = string_scan( stdin );

                                                        entry_set_lastname( entry, lastname );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *telephone;

                                                        printf( "Enter telephone: " );
                                                        telephone = string_scan( stdin );

                                                        entry_set_telephone( entry, telephone );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *cellphone;

                                                        printf( "Enter cellphone: " );
                                                        cellphone = string_scan( stdin );

                                                        entry_set_cellphone( entry, cellphone );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *email;

                                                        printf( "Enter personal e-mail: " );
                                                        email = string_scan( stdin );

                                                        entry_set_email( entry, email );
                                                    } break;
                                                case 6:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_homeaddr( entry, address );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *company;

                                                        printf( "Enter company name: " );
                                                        company = string_scan( stdin );

                                                        entry_set_company( entry, company );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *jobtitle;

                                                        printf( "Enter job title: " );
                                                        jobtitle = string_scan( stdin );

                                                        entry_set_jobtitle( entry, jobtitle );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *workphone;

                                                        printf( "Enter work phone: " );
                                                        workphone = string_scan( stdin );

                                                        entry_set_workphone( entry, workphone );
                                                    } break;
                                                case 10:
                                                    {
                                                        string_t *workemail;

                                                        printf( "Enter work e-mail: " );
                                                        workemail = string_scan( stdin );

                                                        entry_set_workemail( entry, workemail );
                                                    } break;
                                                case 11:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_compaddr( entry, address );
                                                    } break;
                                                case 12:
                                                    {
                                                        string_t *details;

                                                        printf( "Details: " );
                                                        details = string_scan( stdin );

                                                        entry_set_details( entry, details );
                                                    } break;
                                                case 13:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        it = customer->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        customer_remove( customer, it );
                                        printf( "Contact removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        customer_destroy( result, 0 );
                    } break;
                case FIND_BY_LASTNAME:
                    {
                        char lastname[ MAXLENGTH ];
                        customer_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter last name: " );
                        scanf( "%[^\n]", lastname );
                        while( getchar( ) != '\n' );

                        result = customer_find_by_lastname( customer, lastname );
                        it = result->a_head;
                        count = 0;

                        printf( "List of entry found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s\"", lastname );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] First name\n\
[ 2] Last name\n\
[ 3] Telephone\n\
[ 4] Cellphone\n\
[ 5] Personal e-mail\n\
[ 6] Personal address\n\
[ 7] Company name\n\
[ 8] Job title\n\
[ 9] Work phone\n\
[10] work e-mailn\n\
[11] Company address\n\
[12] Details\n\
[13] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *firstname;

                                                        printf( "Enter first name: " );
                                                        firstname = string_scan( stdin );

                                                        entry_set_firstname( entry, firstname );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *lastname;

                                                        printf( "Enter last name: " );
                                                        lastname = string_scan( stdin );

                                                        entry_set_lastname( entry, lastname );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *telephone;

                                                        printf( "Enter telephone: " );
                                                        telephone = string_scan( stdin );

                                                        entry_set_telephone( entry, telephone );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *cellphone;

                                                        printf( "Enter cellphone: " );
                                                        cellphone = string_scan( stdin );

                                                        entry_set_cellphone( entry, cellphone );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *email;

                                                        printf( "Enter personal e-mail: " );
                                                        email = string_scan( stdin );

                                                        entry_set_email( entry, email );
                                                    } break;
                                                case 6:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_homeaddr( entry, address );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *company;

                                                        printf( "Enter company name: " );
                                                        company = string_scan( stdin );

                                                        entry_set_company( entry, company );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *jobtitle;

                                                        printf( "Enter job title: " );
                                                        jobtitle = string_scan( stdin );

                                                        entry_set_jobtitle( entry, jobtitle );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *workphone;

                                                        printf( "Enter work phone: " );
                                                        workphone = string_scan( stdin );

                                                        entry_set_workphone( entry, workphone );
                                                    } break;
                                                case 10:
                                                    {
                                                        string_t *workemail;

                                                        printf( "Enter work e-mail: " );
                                                        workemail = string_scan( stdin );

                                                        entry_set_workemail( entry, workemail );
                                                    } break;
                                                case 11:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_compaddr( entry, address );
                                                    } break;
                                                case 12:
                                                    {
                                                        string_t *details;

                                                        printf( "Details: " );
                                                        details = string_scan( stdin );

                                                        entry_set_details( entry, details );
                                                    } break;
                                                case 13:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        it = customer->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        customer_remove( customer, it );
                                        printf( "Contact removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        customer_destroy( result, 0 );
                    } break;
                case FIND_BY_COMPANY:
                    {
                        char company[ MAXLENGTH ];
                        customer_t *result;
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        printf( "Enter company: " );
                        scanf( "%[^\n]", company );
                        while( getchar( ) != '\n' );

                        result = customer_find_by_company( customer, company );
                        it = result->a_head;
                        count = 0;

                        printf( "List of entries found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "No results found for \"%s\"", company );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit found entry\n\
[2] Delete found entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] First name\n\
[ 2] Last name\n\
[ 3] Telephone\n\
[ 4] Cellphone\n\
[ 5] Personal e-mail\n\
[ 6] Personal address\n\
[ 7] Company name\n\
[ 8] Job title\n\
[ 9] Work phone\n\
[10] work e-mailn\n\
[11] Company address\n\
[12] Details\n\
[13] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *firstname;

                                                        printf( "Enter first name: " );
                                                        firstname = string_scan( stdin );

                                                        entry_set_firstname( entry, firstname );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *lastname;

                                                        printf( "Enter last name: " );
                                                        lastname = string_scan( stdin );

                                                        entry_set_lastname( entry, lastname );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *telephone;

                                                        printf( "Enter telephone: " );
                                                        telephone = string_scan( stdin );

                                                        entry_set_telephone( entry, telephone );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *cellphone;

                                                        printf( "Enter cellphone: " );
                                                        cellphone = string_scan( stdin );

                                                        entry_set_cellphone( entry, cellphone );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *email;

                                                        printf( "Enter personal e-mail: " );
                                                        email = string_scan( stdin );

                                                        entry_set_email( entry, email );
                                                    } break;
                                                case 6:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_homeaddr( entry, address );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *company;

                                                        printf( "Enter company name: " );
                                                        company = string_scan( stdin );

                                                        entry_set_company( entry, company );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *jobtitle;

                                                        printf( "Enter job title: " );
                                                        jobtitle = string_scan( stdin );

                                                        entry_set_jobtitle( entry, jobtitle );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *workphone;

                                                        printf( "Enter work phone: " );
                                                        workphone = string_scan( stdin );

                                                        entry_set_workphone( entry, workphone );
                                                    } break;
                                                case 10:
                                                    {
                                                        string_t *workemail;

                                                        printf( "Enter work e-mail: " );
                                                        workemail = string_scan( stdin );

                                                        entry_set_workemail( entry, workemail );
                                                    } break;
                                                case 11:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_compaddr( entry, address );
                                                    } break;
                                                case 12:
                                                    {
                                                        string_t *details;

                                                        printf( "Details: " );
                                                        details = string_scan( stdin );

                                                        entry_set_details( entry, details );
                                                    } break;
                                                case 13:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        it = customer->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        customer_remove( customer, it );
                                        printf( "Contact removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );

                        customer_destroy( result, 0 );
                    } break;
                case EDIT:
                    {
                        entry_node_t *it;
                        int next_option;
                        unsigned count;

                        it = customer->a_head;
                        count = 0;

                        printf( "List of entries found\n" );

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "There are no entries in your customer agenda\n" );
                            break;
                        }

                        do {
                            printf( "\
[1] Edit entry\n\
[2] Delete entry\n\
[3] Display entry information\n\
[0] Exit\n\
--> " );
                            scanf( "%d", &next_option );
                            while( getchar( ) != '\n' );

                            switch( next_option )
                            {
                                case 1:
                                    {
                                        entry_t *entry;
                                        int field_option;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;

                                        do {
                                            printf( "\
What field to wish to edit?\n\
[ 1] First name\n\
[ 2] Last name\n\
[ 3] Telephone\n\
[ 4] Cellphone\n\
[ 5] Personal e-mail\n\
[ 6] Personal address\n\
[ 7] Company name\n\
[ 8] Job title\n\
[ 9] Work phone\n\
[10] work e-mailn\n\
[11] Company address\n\
[12] Details\n\
[13] All fields\n\
[ 0] Exit\n\
--> " );
                                            scanf( "%d", &field_option );
                                            while( getchar( ) != '\n' );

                                            switch( field_option )
                                            {
                                                case 1:
                                                    {
                                                        string_t *firstname;

                                                        printf( "Enter first name: " );
                                                        firstname = string_scan( stdin );

                                                        entry_set_firstname( entry, firstname );
                                                    } break;
                                                case 2:
                                                    {
                                                        string_t *lastname;

                                                        printf( "Enter last name: " );
                                                        lastname = string_scan( stdin );

                                                        entry_set_lastname( entry, lastname );
                                                    } break;
                                                case 3:
                                                    {
                                                        string_t *telephone;

                                                        printf( "Enter telephone: " );
                                                        telephone = string_scan( stdin );

                                                        entry_set_telephone( entry, telephone );
                                                    } break;
                                                case 4:
                                                    {
                                                        string_t *cellphone;

                                                        printf( "Enter cellphone: " );
                                                        cellphone = string_scan( stdin );

                                                        entry_set_cellphone( entry, cellphone );
                                                    } break;
                                                case 5:
                                                    {
                                                        string_t *email;

                                                        printf( "Enter personal e-mail: " );
                                                        email = string_scan( stdin );

                                                        entry_set_email( entry, email );
                                                    } break;
                                                case 6:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_homeaddr( entry, address );
                                                    } break;
                                                case 7:
                                                    {
                                                        string_t *company;

                                                        printf( "Enter company name: " );
                                                        company = string_scan( stdin );

                                                        entry_set_company( entry, company );
                                                    } break;
                                                case 8:
                                                    {
                                                        string_t *jobtitle;

                                                        printf( "Enter job title: " );
                                                        jobtitle = string_scan( stdin );

                                                        entry_set_jobtitle( entry, jobtitle );
                                                    } break;
                                                case 9:
                                                    {
                                                        string_t *workphone;

                                                        printf( "Enter work phone: " );
                                                        workphone = string_scan( stdin );

                                                        entry_set_workphone( entry, workphone );
                                                    } break;
                                                case 10:
                                                    {
                                                        string_t *workemail;

                                                        printf( "Enter work e-mail: " );
                                                        workemail = string_scan( stdin );

                                                        entry_set_workemail( entry, workemail );
                                                    } break;
                                                case 11:
                                                    {
                                                        address_t *address;
                                                        string_t *string;

                                                        address = address_create( );

                                                        printf( "Enter line #1: " );
                                                        string = string_scan( stdin );
                                                        address_set_line1( address, string );

                                                        printf( "Enter line #2: " );
                                                        string = string_scan( stdin );
                                                        address_set_line2( address, string );

                                                        printf( "Enter city: " );
                                                        string = string_scan( stdin );
                                                        address_set_city( address, string );

                                                        printf( "Enter state: " );
                                                        string = string_scan( stdin );
                                                        address_set_state( address, string );

                                                        printf( "Enter zipcode: " );
                                                        string = string_scan( stdin );
                                                        address_set_zipcode( address, string );

                                                        printf( "Enter country: " );
                                                        string = string_scan( stdin );
                                                        address_set_country( address, string );

                                                        entry_set_compaddr( entry, address );
                                                    } break;
                                                case 12:
                                                    {
                                                        string_t *details;

                                                        printf( "Details: " );
                                                        details = string_scan( stdin );

                                                        entry_set_details( entry, details );
                                                    } break;
                                                case 13:
                                                    {
                                                        entry_destroy( entry );
                                                        entry = entry_prompt( );
                                                    } break;
                                            }
                                        } while( field_option != 0 );

                                    } break;
                                case 2:
                                    {
                                        entry_node_t *it;
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        it = customer->a_head;

                                        while( it != NULL && it->n_entry != entry )
                                        {
                                            it = it->n_next;
                                        }

                                        customer_remove( customer, it );
                                        printf( "Contact removed successfully\n" );
                                    } break;
                                case 3:
                                    {
                                        entry_t *entry;
                                        unsigned index;

                                        do {
                                            printf( "Enter index from list: " );
                                            scanf( "%d", &index );
                                            while( getchar( ) != '\n' );
                                        } while( index-1 > count );

                                        entry = customer_get( customer, index-1 )->n_entry;
                                        entry_print( stdout, entry );
                                    } break;
                            }
                        } while( next_option != 0 );
                    } break;
                case DELETE:
                    {
                        unsigned count, index;
                        entry_node_t *it;

                        it = customer->a_head;
                        count = 0;

                        while( it != NULL )
                        {
                            printf( "%d. %s, %s (%s)\n", count + 1,
                                    it->n_entry->e_lastname->s_ptr,
                                    it->n_entry->e_firstname->s_ptr,
                                    it->n_entry->e_company->s_ptr );
                            it = it->n_next;
                            count++;
                        }

                        if( count == 0 )
                        {
                            printf( "There are not entries in the customer agenda.. EXITING\n" );
                            break;
                        }

                        printf( "Enter index from list: " );
                        scanf( "%d", &index );
                        while( getchar( ) != '\n' );

                        it = customer->a_head;

                        while( index > 1 )
                        {
                            it = it->n_next;
                            index--;
                        }

                        customer_remove( customer, it );
                        printf( "Entry successfully removed\n" );
                    } break;
            }
        } while( option != 0 );

        if( ( file = fopen( FILENAME, "w+" ) ) == NULL )
        {
            perror( "fopen" );
            return EXIT_FAILURE;
        }
        else
        {
            customer_write( file, customer );
            fclose( file );
        }

        customer_destroy( customer, 1 );
    }
    else if( logged == 0 )
    {
        printf( "Fail to login\n" );
    }

    return 0;
}

int login( void )
{
    struct termios tmp_term;
    struct sigaction sa_sigint;
    char username[ MAXLENGTH ],
         password[ MAXLENGTH ],
         scanned_line[ MAXLENGTH ],
         scanned_username[ MAXLENGTH ],
         scanned_password[ MAXLENGTH ];
    FILE *file;

    printf( "Enter your username: " );
    scanf( "%s", username );

    if( tcgetattr( fileno( stdin ), &saved_term ) == -1 ) 
    {
        perror( "tcgetattr" );
        exit( EXIT_FAILURE );
    }

    tmp_term = saved_term;

    memset( &sa_sigint, 0, sizeof( struct sigaction ) );
    sa_sigint.sa_handler = sigint_handler;
    sa_sigint.sa_flags = 0;

    if( sigaction( SIGINT, &sa_sigint, NULL ) < 0 )
    {
        perror( "sigaction" );
        exit( EXIT_FAILURE );
    }

    tmp_term.c_lflag &= ~ECHO;
    if( tcsetattr( fileno( stdin ), TCSANOW, &tmp_term ) == -1 ) 
    {
        perror( "tcgetattr" );
        exit( EXIT_FAILURE );
    }

    printf( "Enter password: " );
    scanf( "%s", password );

    restore_terminal( );

    if( ( file = fopen( "credentials.txt", "r" ) ) == NULL )
    {
        perror( "fopen" );
        return -1;
    }

    while( !feof( file ) )
    {
        int items_scanned;
        char *ptr1, *ptr2;

        items_scanned = fscanf( file, "%[^\n]\n", scanned_line );

        if( scanned_line[ 0 ] == '#' )
        {
            continue;
        }

        /* parse and copy scanned line from credentials file */
        ptr1 = strchr( scanned_line, ':' );
        strncpy( scanned_username, scanned_line, ptr1-scanned_line );
        scanned_username[ ptr1-scanned_line ] = '\0';

        ptr2 = strchr( ptr1 + 1, ':' );
        strncpy( scanned_password, ptr1 + 1, ptr2-ptr1-1 );
        scanned_password[ ptr2-ptr1-1 ] = '\0';
        strcpy( FILENAME, ptr2 + 1 );

        if( items_scanned == 1
                && strcmp( username, scanned_username ) == 0
                && strcmp( password, scanned_password ) == 0 )
        {
            fclose( file );
            return 1;
        }
    }

    fclose( file );
    return 0;
}

entry_t *entry_prompt( void )
{
    address_t *address;
    entry_t *entry;

    if( ( entry = entry_create( ) ) == NULL )
    {
        perror( "entry_create" );
        return NULL;
    }

    printf( "First Name:        " );
    entry_set_firstname( entry, string_scan( stdin ) );
    printf( "Last Name:         " );
    entry_set_lastname( entry, string_scan( stdin ) );
    printf( "Telephone:         " );
    entry_set_telephone( entry, string_scan( stdin ) );
    printf( "Cellphone:         " );
    entry_set_cellphone( entry, string_scan( stdin ) );
    printf( "Personal e-mail:   " );
    entry_set_email( entry, string_scan( stdin ) );
    printf( "\n" );
    printf( "Personal Address\n" );
    address = address_create( );
    printf( "Line #1:   " );
    address_set_line1( address, string_scan( stdin ) );
    printf( "Line #2:   " );
    address_set_line2( address, string_scan( stdin ) );
    printf( "City:      " );
    address_set_city( address, string_scan( stdin ) );
    printf( "State:     " );
    address_set_state( address, string_scan( stdin ) );
    printf( "Zip code:  " );
    address_set_zipcode( address, string_scan( stdin ) );
    printf( "Country:   " );
    address_set_country( address, string_scan( stdin ) );
    printf( "\n" );
    entry_set_homeaddr( entry, address );
    printf( "Company:       " );
    entry_set_company( entry, string_scan( stdin ) );
    printf( "Job title:     " );
    entry_set_jobtitle( entry, string_scan( stdin ) );
    printf( "Work phone:    " );
    entry_set_workphone( entry, string_scan( stdin ) );
    printf( "Work e-mail:   " );
    entry_set_workemail( entry, string_scan( stdin ) );
    printf( "\n" );
    printf( "Company Address\n" );
    address = address_create( );
    printf( "Line #1:   " );
    address_set_line1( address, string_scan( stdin ) );
    printf( "Line #2:   " );
    address_set_line2( address, string_scan( stdin ) );
    printf( "City:      " );
    address_set_city( address, string_scan( stdin ) );
    printf( "State:     " );
    address_set_state( address, string_scan( stdin ) );
    printf( "Zip code:  " );
    address_set_zipcode( address, string_scan( stdin ) );
    printf( "Country:   " );
    address_set_country( address, string_scan( stdin ) );
    printf( "\n" );
    entry_set_compaddr( entry, address );
    printf( "Details:   " );
    entry_set_details( entry, string_scan( stdin ) );

    return entry;
}

void restore_terminal( void )
{
    if( tcsetattr( fileno( stdin ), TCSANOW, &saved_term ) == -1 )
    {
        perror( "tcsetattr" );
        exit( EXIT_FAILURE );
    }
}

void sigint_handler( int sig )
{
    if( SIGINT == sig )
   {
        restore_terminal( );
    }

    exit( EXIT_SUCCESS );
}
