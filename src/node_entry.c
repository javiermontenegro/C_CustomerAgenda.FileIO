#include <node_entry.h>

address_t *address_create( void )
{
    address_t *addr;

    if( ( addr = malloc( sizeof( address_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    memset( addr, 0, sizeof( address_t ) );

    return addr;
}

address_t *address_duplicate( const address_t *addr )
{
    address_t *duplicate;

    if( ( duplicate = address_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    duplicate->a_line1      = string_duplicate( addr->a_line1   );
    duplicate->a_line2      = string_duplicate( addr->a_line2   );
    duplicate->a_city       = string_duplicate( addr->a_city    );
    duplicate->a_state      = string_duplicate( addr->a_state   );
    duplicate->a_zipcode    = string_duplicate( addr->a_zipcode );
    duplicate->a_country    = string_duplicate( addr->a_country );

    return duplicate;
}

void address_set_line1( address_t *addr, string_t *line1 )
{
    if( addr->a_line1 != NULL )
        string_destroy( addr->a_line1 );

    addr->a_line1 = line1;
}

void address_set_line2( address_t *addr, string_t *line2 )
{
    if( addr->a_line2 != NULL )
        string_destroy( addr->a_line2 );

    addr->a_line2 = line2;
}

void address_set_city( address_t *addr, string_t *city )
{
    if( addr->a_city != NULL )
        string_destroy( addr->a_city );

    addr->a_city = city;
}

void address_set_state( address_t *addr, string_t *state )
{
    if( addr->a_state != NULL )
        string_destroy( addr->a_state );

    addr->a_state = state;
}

void address_set_zipcode( address_t *addr, string_t *zipcode )
{
    if( addr->a_zipcode != NULL )
        string_destroy( addr->a_zipcode );

    addr->a_zipcode = zipcode;
}

void address_set_country( address_t *addr, string_t *country )
{
    if( addr->a_country != NULL )
        string_destroy( addr->a_country );

    addr->a_country = country;
}

string_t *address_get_line1( address_t *addr )
{
    return addr->a_line1;
}

string_t *address_get_line2( address_t *addr )
{
    return addr->a_line2;
}

string_t *address_get_city( address_t *addr )
{
    return addr->a_city;
}

string_t *address_get_state( address_t *addr )
{
    return addr->a_state;
}

string_t *address_get_zipcode( address_t *addr )
{
    return addr->a_zipcode;
}

string_t *address_get_country( address_t *addr )
{
    return addr->a_country; 
}

void address_destroy( address_t *addr )
{
    string_destroy( addr->a_line1   );
    string_destroy( addr->a_line2   );
    string_destroy( addr->a_city    );
    string_destroy( addr->a_state   );
    string_destroy( addr->a_zipcode );
    string_destroy( addr->a_country );
    free          ( addr            );
}

entry_t *entry_create( void )
{
    entry_t *entry;

    if( ( entry = malloc( sizeof( entry_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    memset( entry, 0, sizeof( entry_t ) );

    return entry;
}

entry_t *entry_duplicate( const entry_t *entry )
{
    entry_t *duplicate;

    if( ( duplicate = entry_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    duplicate->e_firstname  = string_duplicate  ( entry->e_firstname );
    duplicate->e_lastname   = string_duplicate  ( entry->e_lastname  );
    duplicate->e_telephone  = string_duplicate  ( entry->e_telephone );
    duplicate->e_cellphone  = string_duplicate  ( entry->e_cellphone );
    duplicate->e_email      = string_duplicate  ( entry->e_email     );
    duplicate->e_homeaddr   = address_duplicate ( entry->e_homeaddr  );
    duplicate->e_company    = string_duplicate  ( entry->e_company   );
    duplicate->e_jobtitle   = string_duplicate  ( entry->e_jobtitle  );
    duplicate->e_workphone  = string_duplicate  ( entry->e_workphone );
    duplicate->e_workemail  = string_duplicate  ( entry->e_workemail );
    duplicate->e_compaddr   = address_duplicate ( entry->e_compaddr  );
    duplicate->e_details    = string_duplicate  ( entry->e_details   );

    return duplicate;
}

void entry_print( FILE *file, entry_t *entry )
{
    const char *fmt = "\
First name:         %s\n\
Last name:          %s\n\
Telephone:          %s\n\
Cellphone:          %s\n\
Personal e-mail:    %s\n\n\
Personal Address\n\
Line #1:    %s\n\
Line #2:    %s\n\
City:       %s\n\
State:      %s, %s\n\
Country:    %s\n\n\
Company:        %s\n\
Job title:      %s\n\
Work phone:     %s\n\
Work e-mail:    %s\n\n\
Company Address\n\
Line #1:    %s\n\
Line #2:    %s\n\
City:       %s\n\
State:      %s, %s\n\
Country:    %s\n\n\
Details:\n\n\
%s\n";
    fprintf( file, fmt,
        entry_get_firstname( entry )->s_ptr,
        entry_get_lastname( entry )->s_ptr,
        entry_get_telephone( entry )->s_ptr,
        entry_get_cellphone( entry )->s_ptr,
        entry_get_email( entry )->s_ptr,
        address_get_line1( entry_get_homeaddr( entry ) )->s_ptr,
        address_get_line2( entry_get_homeaddr( entry ) )->s_ptr,
        address_get_city( entry_get_homeaddr( entry ) )->s_ptr,
        address_get_state( entry_get_homeaddr( entry ) )->s_ptr,
        address_get_zipcode( entry_get_homeaddr( entry ) )->s_ptr,
        address_get_country( entry_get_homeaddr( entry ) )->s_ptr,
        entry_get_company( entry )->s_ptr,
        entry_get_jobtitle( entry )->s_ptr,
        entry_get_workphone( entry )->s_ptr,
        entry_get_workemail( entry )->s_ptr,
        address_get_line1( entry_get_compaddr( entry ) )->s_ptr,
        address_get_line2( entry_get_compaddr( entry ) )->s_ptr,
        address_get_city( entry_get_compaddr( entry ) )->s_ptr,
        address_get_state( entry_get_compaddr( entry ) )->s_ptr,
        address_get_zipcode( entry_get_compaddr( entry ) )->s_ptr,
        address_get_country( entry_get_compaddr( entry ) )->s_ptr,
        entry_get_details( entry )->s_ptr );
}

entry_t *entry_scan( FILE *file )
{
    address_t *address;
    entry_t *entry;

    entry = entry_create( );
    entry_set_firstname( entry, string_scan( file ) );
    entry_set_lastname( entry, string_scan( file ) );
    entry_set_telephone( entry, string_scan( file ) );
    entry_set_cellphone( entry, string_scan( file ) );
    entry_set_email( entry, string_scan( file ) );
    address = address_create( );
    address_set_line1( address, string_scan( file ) );
    address_set_line2( address, string_scan( file ) );
    address_set_city( address, string_scan( file ) );
    address_set_state( address, string_scan( file ) );
    address_set_zipcode( address, string_scan( file ) );
    address_set_country( address, string_scan( file ) );
    entry_set_homeaddr( entry, address );
    entry_set_company( entry, string_scan( file ) );
    entry_set_jobtitle( entry, string_scan( file ) );
    entry_set_workphone( entry, string_scan( file ) );
    entry_set_workemail( entry, string_scan( file ) );
    address = address_create( );
    address_set_line1( address, string_scan( file ) );
    address_set_line2( address, string_scan( file ) );
    address_set_city( address, string_scan( file ) );
    address_set_state( address, string_scan( file ) );
    address_set_zipcode( address, string_scan( file ) );
    address_set_country( address, string_scan( file ) );
    entry_set_compaddr( entry, address );
    entry_set_details( entry, string_scan( file ) );

    return entry;
}

void entry_write( FILE *file, entry_t *entry )
{
    string_write( file, entry->e_firstname );
    string_write( file, entry->e_lastname  );
    string_write( file, entry->e_telephone );
    string_write( file, entry->e_cellphone );
    string_write( file, entry->e_email     );
    string_write( file, entry->e_homeaddr->a_line1   );
    string_write( file, entry->e_homeaddr->a_line2   );
    string_write( file, entry->e_homeaddr->a_city    );
    string_write( file, entry->e_homeaddr->a_state   );
    string_write( file, entry->e_homeaddr->a_zipcode );
    string_write( file, entry->e_homeaddr->a_country );
    string_write( file, entry->e_company   );
    string_write( file, entry->e_jobtitle  );
    string_write( file, entry->e_workphone );
    string_write( file, entry->e_workemail );
    string_write( file, entry->e_compaddr->a_line1   );
    string_write( file, entry->e_compaddr->a_line2   );
    string_write( file, entry->e_compaddr->a_city    );
    string_write( file, entry->e_compaddr->a_state   );
    string_write( file, entry->e_compaddr->a_zipcode );
    string_write( file, entry->e_compaddr->a_country );
    string_write( file, entry->e_details );
}

entry_t *entry_read( FILE *file )
{
    address_t *address;
    entry_t *entry;

    entry = entry_create( );
    entry_set_firstname( entry, string_read( file ) );
    entry_set_lastname( entry, string_read( file ) );
    entry_set_telephone( entry, string_read( file ) );
    entry_set_cellphone( entry, string_read( file ) );
    entry_set_email( entry, string_read( file ) );

    address = address_create( );
    address_set_line1( address, string_read( file ) );
    address_set_line2( address, string_read( file ) );
    address_set_city( address, string_read( file ) );
    address_set_state( address, string_read( file ) );
    address_set_zipcode( address, string_read( file ) );
    address_set_country( address, string_read( file ) );
    entry_set_homeaddr( entry, address );

    entry_set_company( entry, string_read( file ) );
    entry_set_jobtitle( entry, string_read( file ) );
    entry_set_workphone( entry, string_read( file ) );
    entry_set_workemail( entry, string_read( file ) );

    address = address_create( );
    address_set_line1( address, string_read( file ) );
    address_set_line2( address, string_read( file ) );
    address_set_city( address, string_read( file ) );
    address_set_state( address, string_read( file ) );
    address_set_zipcode( address, string_read( file ) );
    address_set_country( address, string_read( file ) );
    entry_set_compaddr( entry, address );

    entry_set_details( entry, string_read( file ) );

    return entry;
}

void entry_set_firstname( entry_t *entry, string_t *firstname )
{
    if( entry->e_firstname != NULL )
        string_destroy( entry->e_firstname );

    entry->e_firstname = firstname;
}

void entry_set_lastname( entry_t *entry, string_t *lastname )
{
    if( entry->e_lastname != NULL )
        string_destroy( entry->e_lastname );

    entry->e_lastname = lastname;
}

void entry_set_telephone( entry_t *entry, string_t *telephone )
{
    if( entry->e_telephone != NULL )
        string_destroy( entry->e_telephone );

    entry->e_telephone = telephone;
}

void entry_set_cellphone( entry_t *entry, string_t *cellphone )
{
    if( entry->e_cellphone != NULL )
        string_destroy( entry->e_cellphone );

    entry->e_cellphone = cellphone;
}

void entry_set_email( entry_t *entry, string_t *email )
{
    if( entry->e_email != NULL )
        string_destroy( entry->e_email );

    entry->e_email = email;
}

void entry_set_homeaddr( entry_t *entry, address_t *homeaddr )
{
    if( entry->e_homeaddr != NULL )
        address_destroy( entry->e_homeaddr );

    entry->e_homeaddr = homeaddr;
}

void entry_set_company( entry_t *entry, string_t *company )
{
    if( entry->e_company != NULL )
        string_destroy( entry->e_company );

    entry->e_company = company;
}

void entry_set_jobtitle( entry_t *entry, string_t *jobtitle )
{
    if( entry->e_jobtitle != NULL )
        string_destroy( entry->e_jobtitle );

    entry->e_jobtitle = jobtitle;
}

void entry_set_workphone( entry_t *entry, string_t *workphone )
{
    if( entry->e_workphone != NULL )
        string_destroy( entry->e_workphone );

    entry->e_workphone = workphone;
}

void entry_set_workemail( entry_t *entry, string_t *workemail )
{
    if( entry->e_workemail != NULL )
        string_destroy( entry->e_workemail );

    entry->e_workemail = workemail;
}

void entry_set_compaddr( entry_t *entry, address_t *compaddr )
{
    if( entry->e_compaddr != NULL )
        address_destroy( entry->e_compaddr );

    entry->e_compaddr = compaddr;
}

void entry_set_details( entry_t *entry, string_t *details )
{
    if( entry->e_details != NULL )
        string_destroy( entry->e_details );

    entry->e_details = details;
}

string_t *entry_get_firstname( entry_t *entry )
{
    return entry->e_firstname;
}

string_t *entry_get_lastname( entry_t *entry )
{
    return entry->e_lastname;
}

string_t *entry_get_telephone( entry_t *entry )
{
    return entry->e_telephone;
}
string_t *entry_get_cellphone( entry_t *entry )
{
    return entry->e_cellphone;
}

string_t *entry_get_email( entry_t *entry )
{
    return entry->e_email;
}

address_t *entry_get_homeaddr( entry_t *entry )
{
    return entry->e_homeaddr;
}

string_t *entry_get_company( entry_t *entry )
{
    return entry->e_company;
}

string_t *entry_get_jobtitle( entry_t *entry )
{
    return entry->e_jobtitle;
}

string_t *entry_get_workphone( entry_t *entry )
{
    return entry->e_workphone;
}

string_t *entry_get_workemail( entry_t *entry )
{
    return entry->e_workemail;
}

address_t *entry_get_compaddr( entry_t *entry )
{
    return entry->e_compaddr;
}

string_t *entry_get_details( entry_t *entry )
{
    return entry->e_details;
}

void entry_destroy( entry_t *entry )
{
    string_destroy  ( entry->e_firstname );
    string_destroy  ( entry->e_lastname  );
    string_destroy  ( entry->e_telephone );
    string_destroy  ( entry->e_cellphone );
    string_destroy  ( entry->e_email     );
    address_destroy ( entry->e_homeaddr  );
    string_destroy  ( entry->e_company   );
    string_destroy  ( entry->e_jobtitle  );
    string_destroy  ( entry->e_workphone );
    string_destroy  ( entry->e_workemail );
    address_destroy ( entry->e_compaddr  );
    string_destroy  ( entry->e_details   );
    free            ( entry              );
}
