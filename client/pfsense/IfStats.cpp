#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>
#include <ifaddrs.h>
#include <string.h>
#include <sys/time.h>
#include <stdio.h>
#include "IfStats.h"

IfStats::IfStats(const char *if_name)
{
    this->if_name = if_name;
    read_counters();
}

void IfStats::get_rate(double *in_bps, double *out_bps)
{
    unsigned long old_bytes_in = bytes_in;
    unsigned long old_bytes_out = bytes_out;
    double old_update_time = update_time;
    read_counters();
    double elapsed = (update_time-old_update_time);
    *in_bps = (double)(bytes_in - old_bytes_in)/elapsed;
    *out_bps = (double)(bytes_out - old_bytes_out)/elapsed;
}

void IfStats::read_counters()
{
    struct ifaddrs *ifap;
    struct if_data *ifdata;
    struct timespec timespec;
    clock_gettime(CLOCK_MONOTONIC, &timespec);
    update_time = 
        (double)timespec.tv_sec + 
        (double)timespec.tv_nsec / 1000000000.0;
    int err = getifaddrs(&ifap);
    if (err==0) {
        while (ifap != NULL) {
            if (ifap->ifa_addr->sa_family == AF_LINK &&
                !strcmp(ifap->ifa_name, if_name)) {
                ifdata = (struct if_data*)ifap->ifa_data;
                bytes_in = ifdata->ifi_ibytes;
                bytes_out = ifdata->ifi_obytes;
            }
            ifap = ifap->ifa_next;
        }
    } else {
        printf("Error %d getting interfaces", err);
    }
}
