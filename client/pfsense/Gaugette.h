
class Gaugette
{
    int fd;
public:
    Gaugette(const char *port);
    void open(const char *port);
    void close();
    bool set(int device, int value);
    bool zero(int device);
    bool send(char cmd, int device, int value);
};
