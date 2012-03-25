class IfStats
{
    const char *if_name;
    unsigned long bytes_in;
    unsigned long bytes_out;
    double update_time;

 public:
    IfStats(const char *if_name);
    void get_rate(double *in_bps, double *out_bps);
    void read_counters();
};
