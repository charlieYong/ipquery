#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include "src/ip.h"

#define TIMES 5000

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
    int i = 1, j = 0;
    long long start, end;
    for (; i<argc; i++) {
        printf("%s -> %s ", argv[i], query(argv[i]));
        start = ustime();
        for (j=0; j < TIMES; j++) {
            query(argv[i]);
        }
        end = ustime();
        printf("(%d times in %.6f seconds, %.6f microseconds per query)\n", j, (float)(end-start)/1000000, (float)(end-start)/TIMES);
    }
    release();
    return 0;
}
