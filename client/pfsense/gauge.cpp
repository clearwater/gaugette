#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "Gaugette.h"
#include "IfStats.h"

int main(int argc, char **argv)
{
    char *sio_name = "/dev/cuaU0";
    char *if_name = "pppoe0";

    if (argc>1) {
        sio_name = argv[1];
    }
    if (argc>2) {
        if_name = argv[2];
    }
    printf("sio port %s\n", sio_name);
    printf("interface %s\n", if_name);

    Gaugette gaugette(sio_name);
    IfStats interface(if_name);

    int steps = 696;
    int fsd = 100000;
    int period = 5000000;
    usleep(5000000);
    gaugette.zero(0);
    gaugette.zero(1);
    while (true) {
        double in_bps, out_bps;
        usleep(period);
        interface.get_rate(&in_bps, &out_bps);
        gaugette.set(0,in_bps / fsd * steps);
        gaugette.set(1,out_bps / fsd * steps);
    }
    return 0;
}


