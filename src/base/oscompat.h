/**
 * \file oscompat.h
 * Provides system independent operating system functionality. 
 */
#ifndef _OSCOMPATABILITY_H
#define _OSCOMPATABILITY_H


#include "basetypes.h"


#ifdef WIN32
#include <Windows.h>
#else
#include <sys/time.h>
#include <ctime>
#endif

#define MICROS_PER_SECOND 10e6

/**
 * Returns the current time since January 1, 1970 in microseconds.   
 * \returns The number of microseconds.
 */
uint64 getMicros();

#endif