/* NOTE:  ALL CODE BELOW BELONGS TO DR. DOHERTY.  WE ARE REUSING IT TO KEEP WITH HIS STANDARDS FOR TIME MEASUREMENT */

/* COMP 137-1 Spring 2018
 * filename: timer.h
 *
 * Functions to get current time.
 *
 * getProcessTime() returns time in seconds since the start of the program.
 * getWorldTime() returns time in seconds since the Epoch (00:00:00 UTC, January 1 1970)
 *
 * getProcessTime() seems to be having problems for some users, particularly OSX users.
 */

#include <time.h>
double getProcessTime()
{
    clock_t t;
    t = clock();
    return ((double)t)/CLOCKS_PER_SEC;
}

#include <sys/time.h>
double getWorldTime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    double s = t.tv_sec + ((double)t.tv_usec)/1000000.0;
    return s;
}