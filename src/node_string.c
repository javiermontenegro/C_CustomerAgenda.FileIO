#include <node_string.h>

string_t *string_create( const char *s )
{
    string_t *str;

    if( ( str = malloc( sizeof( string_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    if( ( str->s_ptr = malloc( strlen( s ) + 1 ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    strcpy( str->s_ptr, s );
    str->s_len = strlen( s );

    return str;
}

string_t *string_duplicate( const string_t *str )
{
    return string_create( str->s_ptr );
}

void string_print( FILE *file, string_t *str )
{
   fputs( str->s_ptr, file );
}

string_t *string_scan( FILE *file )
{
    unsigned long long capacity;
    string_t *str;
    char *tmp;
    int chr;

    if( ( str = malloc( sizeof( string_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    capacity = 1;

    if( ( str->s_ptr = malloc( sizeof( capacity * sizeof( char ) ) ) ) == 0 )
    {
        errno = ENOMEM;
        free( str );

        return NULL;
    }

    str->s_len = 0;

    while( ( chr = fgetc( file ) ) != '\n' && chr != EOF )
    {
        if( str->s_len >= capacity )
        {
            tmp = realloc( str->s_ptr, 2 * capacity * sizeof( char ) );

            if( tmp == NULL )
            {
                string_destroy( str );

                errno = ENOMEM;
                return NULL;
            }

            str->s_ptr = tmp;
            capacity *= 2;
        }

        str->s_ptr[ str->s_len ] = chr;
        str->s_len++;
    }

    str->s_ptr[ str->s_len ] = '\0';
    tmp = realloc( str->s_ptr, strlen( str->s_ptr ) + 1 );

    if( tmp == NULL )
    {
        free( str->s_ptr );
        free( str );

        errno = ENOMEM;
        return NULL;
    }

    str->s_ptr = tmp;

    return str;
}

void string_write( FILE *file, string_t *str )
{
    fwrite( &str->s_len, sizeof( str->s_len ), 1, file );
    fwrite( str->s_ptr, sizeof( char ), str->s_len, file );
}

string_t *string_read( FILE *file )
{
    string_t *str;
    unsigned long long length;
    char *ptr;

    fread( &length, sizeof( length ), 1, file );

    if( ( ptr = malloc( length + 1 ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    fread( ptr, sizeof( char ), length, file );
    ptr[ length ] = '\0';
    str = string_create( ptr );

    free( ptr );

    return str;
}

void string_destroy( string_t *str )
{
    free( str->s_ptr );
    free( str        );
}
