/* x.c - program pomocniczy x, okrojona wersja polecenia sleep */
/* wykorzystywana do testów */
#include <stdlib.h>
#include <unistd.h>

int main( int argc, char *argv[] )
{
	if( argc < 2 )
		return 1;
	sleep( atoi( argv[1] ) );
	return 0;
}
