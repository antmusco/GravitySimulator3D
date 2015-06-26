/**
 * \file oscompat.cpp
 * Implements functions declared in oscompat.h. 
 */


#include "oscompat.h"
#include "basetypes.h"


#define UNIX_EPOCH_TIME_IN_SECONDS 11644473600LL
#define HUNDRED_NANOS_PER_SECOND 10000000LL


uint64 getMicros() {

// Windows
#ifdef WIN32
 		
 	FILETIME ft;
 	LARGE_INTEGER li;

 	// Get the amount of 100 nano seconds intervals elapsed since 
 	// January 1, 1601 (UTC) and copy it to a LARGE_INTEGER structure.
 	GetSystemTimeAsFileTime(&ft);
 	li.LowPart = ft.dwLowDateTime;
 	li.HighPart = ft.dwHighDateTime;

 	// Get the 64 bit number indicating number of 100 nano seconds.
 	uint64 ret = li.QuadPart;

 	// Convert from file time to UNIX epoch time.
 	ret -= (UNIX_EPOCH_TIME_IN_SECONDS * HUNDRED_NANOS_PER_SECOND);

 	// From 100 nano seconds (10^-7) to 1 microsecond (10^-6) intervals
 	ret /= 10; 

 	return ret;

// Linux
#else

 	// Get time of day and send it to timeval struct.
	struct timeval tv;
	gettimeofday(&tv, NULL);
 
 	// Get the number of whole seconds (10^0) in microseconds (10^-6).
 	uint64 ret = (tv.tv_sec) * MICROS_PER_SECOND;

	// Add the number of microseconds.
	ret += tv.tv_usec;

 	return ret;

#endif

}