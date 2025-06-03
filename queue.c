/*

#define NUM_BUFFERS 128
#define BUFFER_SIZE 132

uint8_t   stream_fifo[NUM_BUFFERS][BUFFER_SIZE];
int16_t   stream_wr_index;
int16_t   stream_rd_index;

void init_queue ()
{
    stream_wr_index = 0;
    stream_rd_index = 0;
}

void write_queue ( uint8_t header, uint8_t * buffer )
{
    // save data into fifo at current write index
    memcpy ( stream_fifo[ stream_wr_index ], buffer, BUFFER_SIZE );

    // increment write index, and wrap if it goes past end of fifo
    stream_wr_index++;
    stream_wr_index %= NUM_BUFFERS;

    // if the write index "catches up" to the read index, we are in an overwrite situation
    // decrement the write index back to where it was
    if ( stream_wr_index == stream_rd_index )
    {
        stream_wr_index--;
        if ( stream_wr_index == -1 )
            stream_wr_index = NUM_BUFFERS-1;
        else
            stream_wr_index;
    }
}

uint8_t * read_queue ( void )
{
     // if there is something queued up, return the pointer and increment the read index
    if ( stream_rd_index != stream_wr_index )
    {
        return ( stream_fifo[ stream_rd_index++ ] );
    }

    // if nothing queued, return a NULL
    return ( NULL );
}
*/
