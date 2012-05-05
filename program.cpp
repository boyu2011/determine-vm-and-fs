//============================================================================
// Name        : program.cpp
// Author      : Bo Yu
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;
#include <sys/shm.h>	// shared memory api
#include <sys/ipc.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>		// mkstemp
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define PAGE_NUM 10000
#define PAGE_SIZE 1024*4
#define PAGE_WRITE_NUM 50
#define TRANSFER_BYTE_1 10000
#define TRANSFER_BYTE_2 20000
#define TRANSFER_BYTE_3 30000
#define TRANSFER_BYTE_4 40000
#define TRANSFER_BYTE_5 50000
#define TRANSFER_BYTE_6 60000

static struct timeval s_tvStart;

void StartTimer()
{
	gettimeofday ( &s_tvStart, NULL );
}

double GetTimer()
{
	struct timeval tvFinish;
	long sec, usec;

	gettimeofday ( &tvFinish, NULL );
	sec = tvFinish.tv_sec - s_tvStart.tv_sec;
	usec = tvFinish.tv_usec - s_tvStart.tv_usec;

	return ( 1e+6*sec + usec );	// usec
}

int g_pageTable [PAGE_NUM][1024];	// using integer array element simulates page.

void DetermineVM ()
{
	// Banner
	printf ( "************\n" );
	printf ( "Determine VM\n" );
	printf ( "************\n\n" );
	
	double dTime;
	for ( int i = 0; i < PAGE_NUM; i++ )
	{
		StartTimer();
		g_pageTable [i][1024] ++;
		dTime = GetTimer();

		if ( dTime > 100 ) // assume that page fault occured.
		{
			printf ( "--------------------------------------\n" );
			printf ( "[PAGE FAULT] Access page [%d][1024] Time = %e usec.\n", i, dTime );
			
			// Trace back  the pages, access them, and detect the page fault.
			for ( int j = i; j > 0; j-- )
			{	
				StartTimer();
				g_pageTable [j][1024] ++;
				dTime = GetTimer();
				if ( dTime > 100 )
				{
					printf ( "[SCAN BACK] Access page [%d][1024] Time = %e usec.\n", j, dTime );
				}
			}
		} // Endof if		
	} // End of for
}

void DetermineDiskBandwidth ()
{
	printf ( "**********************\n" );
	printf ( "DetermineDiskBandwidth\n" );
	printf ( "**********************\n\n" );

	// 1. Create temp file.
	int fd;
	char c = 'y';
	fd = open ( "temp_file_disk_bandwidth", O_RDWR|O_CREAT );

	// 2. Write file and measure the time.
	StartTimer();
	for ( double i = 0; i < TRANSFER_BYTE_1; i++ )
		write ( fd, &c, 1 );
	double dTime = GetTimer();	// usec
	dTime = 1e-6*dTime;	// sec
	printf ( "Disk Bandwidth = %e (byte/sec)\n", TRANSFER_BYTE_1 / dTime );

	// 3. Close file.
	close ( fd );
}

void DetermineSeekTime ()
{
	printf ( "*****************\n" );
	printf ( "DetermineSeekTime\n" );
	printf ( "*****************\n" );
	printf ( "Plese wait...\n" );

	//
	// Experimental 1.
	//

	// 1. Create temp file.
	int fd;
	char c = 'y';
	double dTime;
	fd = open ( "temp_file_seek_1", O_RDWR|O_CREAT );
	// 2. Write file.
	for ( double i = 0; i < TRANSFER_BYTE_1; i++ )
		write ( fd, &c, 1 );

	printf ( "File size = %d\n", TRANSFER_BYTE_1 );
	// 3. Measure time for seek 1.
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_1 / 8, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_1 / 8, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_1 / 4, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_1 / 4, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_1 / 3, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_1 / 3, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_1 / 2, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_1 / 2, dTime );

	// 4. Close file.
	close ( fd );

	//
	// Experimental 2.
	//

	// 1. Create temp file.
	fd = open ( "temp_file_seek_2", O_RDWR|O_CREAT );
	// 2. Write file.
	for ( double i = 0; i < TRANSFER_BYTE_2; i++ )
		write ( fd, &c, 1 );

	printf ( "\nFile size = %d\n", TRANSFER_BYTE_2 );
	// 3. Measure time for seek 2.
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_2 / 8, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_2 / 8, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_2 / 4, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_2 / 4, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_2 / 3, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_2 / 3, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_2 / 2, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_2 / 2, dTime );
	// 4. Close file.
	close ( fd );

	//
	// Experimental 3.
	//

	// 1. Create temp file.
	fd = open ( "temp_file_seek_3", O_RDWR|O_CREAT );
	// 2. Write file.
	for ( double i = 0; i < TRANSFER_BYTE_3; i++ )
		write ( fd, &c, 1 );

	printf ( "\nFile size = %d\n", TRANSFER_BYTE_3 );
	// 3. Measure time for seek 1.
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_3 / 8, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_3 / 8, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_3 / 4, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_3 / 4, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_3 / 3, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_3 / 3, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_3 / 2, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_3 / 2, dTime );

	// 4. Close file.
	close ( fd );

	//
	// Experimental 4.
	//

	// 1. Create temp file.
	fd = open ( "temp_file_seek_4", O_RDWR|O_CREAT );
	// 2. Write file.
	for ( double i = 0; i < TRANSFER_BYTE_4; i++ )
		write ( fd, &c, 1 );

	printf ( "\nFile size = %d\n", TRANSFER_BYTE_4 );
	// 3. Measure time for seek 4.
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_4 / 8, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_4 / 8, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_4 / 4, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_4 / 4, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_4 / 3, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_4 / 3, dTime );
	StartTimer();
	lseek ( fd, TRANSFER_BYTE_4 / 2, SEEK_SET );
	dTime = GetTimer();	// usec
	printf ( "Seek that locate at %d byte, time = %e (usec)\n", TRANSFER_BYTE_4 / 2, dTime );
	// 4. Close file.
	close ( fd );

}


int main() {

	DetermineVM();

	DetermineDiskBandwidth ();

	DetermineSeekTime ();

	printf ( "End of Main().\n" );
	cout << "main()";
	return 0;
}
