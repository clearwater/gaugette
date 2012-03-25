#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "Gaugette.h"

Gaugette::Gaugette(const char *port)
{
    open(port);
}

void Gaugette::open(const char *port)
{
    printf("opening %s\n", port);
    fd = ::open(port,O_RDWR);
}

void Gaugette::close()
{
    ::close(fd);
    fd = -1;
}

bool Gaugette::zero(int device)
{
    return send('z',device,0);
}

bool Gaugette::set(int device, int value)
{
    return send('s',device,value);
}

bool Gaugette::send(char cmd, int device, int value)
{
    char buf[100];
    sprintf(buf, "%c %d %d\n", cmd, device, value);
    int len = strlen(buf);
    return len == write(this->fd, buf, len);
}
