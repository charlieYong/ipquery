#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include "src/ip.h"

long long ustime() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec*1000000 + tv.tv_usec;
}

int main(int argc, char** argv) {
    if (2 > argc) {
        printf("%s ip [ip2 ip3 ..]\n", argv[0]);
        return -1;
    }
    char* datafile = "data/qqwry.dat";
    if (load_ip_data_file(datafile)) {
        printf("error\n");
        return -1;
    }
    int i = 1;
    long long start;
    for (; i<argc; i++) {
        start = ustime();
        printf("%s -> %s ", argv[i], query(argv[i]));
        printf("time: %.6f seconds\n", (double)((ustime() - start) / 1000000));
    }
    release();
    return 0;
}
