#include <customer.h>

customer_t *customer_create( void )
{
    customer_t *customer;

    if( ( customer = malloc( sizeof( customer_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    memset( customer, 0, sizeof( customer_t ) );

    return customer;
}

customer_t *customer_duplicate( const customer_t *customer )
{
    customer_t *duplicate;

    if( ( duplicate = malloc( sizeof( customer_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    if( customer_add_all( duplicate, ( customer_t* )customer ) == -1 )
    {
        customer_destroy( duplicate, 1 );
        errno = ENOMEM;

        return NULL;
    }

    return duplicate;
}

customer_t *customer_search( customer_t *customer, const char *firstname, const char *lastname )
{
    entry_node_t *it, *tmp;
    customer_t *retval;

    if( ( retval = customer_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = customer->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_firstname->s_ptr, firstname ) == 0
                && strcmp( it->n_entry->e_lastname->s_ptr, lastname ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                customer_destroy( retval, 0 );

                errno = ENOMEM;
                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                tmp->n_prev = retval->a_head;
                retval->a_head->n_next = tmp;
                retval->a_tail = tmp;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

entry_node_t *customer_get( const customer_t *customer, unsigned index )
{
    entry_node_t *it;
    unsigned i;

    it = customer->a_head;
    i = 0;

    while( it != NULL && i < index )
    {
        it = it->n_next;
        i++;
    }

    if( it == NULL )
    {
        errno = EINVAL;
        return NULL;
    }

    return it;
}

void customer_write( FILE *file, customer_t *customer )
{
    entry_node_t *it;
    unsigned count;

    it = customer->a_head;
    count = 0;

    while( it != NULL )
    {
        it = it->n_next;
        count++;
    }

    fwrite( &count, sizeof( count ), 1, file );

    it = customer->a_head;

    while( it != NULL )
    {
        entry_write( file, it->n_entry );
        it = it->n_next;
    }
}

customer_t *customer_read( FILE *file )
{
    customer_t *customer;
    entry_t *entry;
    unsigned count;

    fread( &count, sizeof( count ), 1, file );

    customer = customer_create( );

    while( count > 0 )
    {
        entry = entry_read( file );

        if( customer_add( customer, entry ) == -1 )
        {
            customer_destroy( customer, 1 );
            errno = ENOMEM;

            return NULL;
        }

        count--;
    }

    return customer;
}

int customer_add( customer_t *customer, entry_t *entry )
{
    entry_node_t *node;

    if( ( node = malloc( sizeof( entry_node_t ) ) ) == NULL )
    {
        errno = ENOMEM;
        return -1;
    }

    if( ( node->n_entry = entry_duplicate( entry ) ) == NULL )
    {
        errno = ENOMEM;
        free( node );
    
        return -1;
    }
    

    node->n_prev = node->n_next = NULL;
    /* node->n_entry = entry; */

    if( customer->a_head == NULL )
    {
        customer->a_head = node;
    }
    else if( customer->a_tail == NULL )
    {
        customer->a_tail = node;
        customer->a_head->n_next = customer->a_tail;
        customer->a_tail->n_prev = customer->a_head;
    }
    else
    {
        node->n_prev = customer->a_tail;
        customer->a_tail->n_next = node;
        customer->a_tail = node;
    }

    return 0;
}

int customer_add_all( customer_t *customer, customer_t *some_customer )
{
    entry_node_t *it;

    it = some_customer->a_head;

    while( it != NULL )
    {
        if( customer_add( customer, it->n_entry ) == -1 )
        {
            errno = ENOMEM;
            return -1;
        }

        it = it->n_next;
    }

    return 0;
}

int customer_add_many( customer_t *customer, int count, ... )
{
    va_list ap;

    va_start( ap, count );

    while( count > 0 )
    {
        if( customer_add( customer, va_arg( ap, entry_t* ) ) == -1 )
        {
            customer_destroy( customer, 1 );
            errno = ENOMEM;

            return -1;
        }

        count--;
    }

    va_end( ap );

    return 0;
}

customer_t *customer_find_by_firstname( const customer_t *customer, const char *firstname )
{
    entry_node_t *it, *tmp;
    customer_t *retval;

    if( ( retval = customer_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = customer->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_firstname->s_ptr, firstname ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                customer_destroy( retval, 0 );
                errno = ENOMEM;

                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                retval->a_tail = tmp;
                retval->a_head->n_next = retval->a_tail;
                retval->a_tail->n_prev = retval->a_head;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

customer_t *customer_find_by_lastname( const customer_t *customer, const char *lastname )
{
    entry_node_t *it, *tmp;
    customer_t *retval;

    if( ( retval = customer_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = customer->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_lastname->s_ptr, lastname ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                customer_destroy( retval, 0 );
                errno = ENOMEM;

                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                retval->a_tail = tmp;
                retval->a_head->n_next = retval->a_tail;
                retval->a_tail->n_prev = retval->a_head;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

customer_t *customer_find_by_company( const customer_t *customer, const char *company )
{
    entry_node_t *it, *tmp;
    customer_t *retval;

    if( ( retval = customer_create( ) ) == NULL )
    {
        errno = ENOMEM;
        return NULL;
    }

    it = customer->a_head;

    while( it != NULL )
    {
        if( strcmp( it->n_entry->e_company->s_ptr, company ) == 0 )
        {
            if( ( tmp = malloc( sizeof( entry_node_t ) ) ) == NULL )
            {
                customer_destroy( retval, 0 );
                errno = ENOMEM;

                return NULL;
            }

            tmp->n_prev = tmp->n_next = NULL;
            tmp->n_entry = it->n_entry;

            if( retval->a_head == NULL )
            {
                retval->a_head = tmp;
            }
            else if( retval->a_tail == NULL )
            {
                retval->a_tail = tmp;
                retval->a_head->n_next = retval->a_tail;
                retval->a_tail->n_prev = retval->a_head;
            }
            else
            {
                tmp->n_prev = retval->a_tail;
                retval->a_tail->n_next = tmp;
                retval->a_tail = tmp;
            }
        }

        it = it->n_next;
    }

    return retval;
}

entry_t *customer_remove( customer_t *customer, entry_node_t *entry_node )
{
    entry_t *retval;

    if( customer->a_head == entry_node )
    {
        retval = customer->a_head->n_entry;
        customer->a_head = customer->a_head->n_next;

        if( customer->a_head != NULL )
        {
            customer->a_head->n_prev = NULL;
        }
    }
    else if( customer->a_tail == entry_node )
    {
        retval = customer->a_tail->n_entry;
        customer->a_tail = customer->a_tail->n_prev;

        if( customer->a_tail != NULL )
        {
            customer->a_tail->n_next = NULL;
        }
    }
    else
    {
        retval = entry_node->n_entry;
        entry_node->n_prev->n_next = entry_node->n_next;
        entry_node->n_next->n_prev = entry_node->n_prev;
    }

    free( entry_node );

    return retval;
}

customer_t *customer_remove_all( customer_t *customer, customer_t *some_customer )
{
    entry_node_t *it1, *it2;

    it1 = some_customer->a_head;

    while( it1 != NULL )
    {
        it2 = customer->a_head;

        while( it2 != NULL )
        {
            if( it1->n_entry == it2->n_entry )
            {
                customer_remove( customer, it2 );
            }

            it2 = it2->n_next;
        }

        it1 = it1->n_next;
    }

    return some_customer;
}

void customer_destroy( customer_t *customer, int all )
{
    entry_node_t *it, *next;

    it = customer->a_head;

    while( it != NULL )
    {
        next = it->n_next;

        if( all )
        {
            entry_destroy( it->n_entry );
        }

        free( it );
        it = next;
    }

    free( customer );
}
