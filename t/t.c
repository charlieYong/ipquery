#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "src/ip.h"

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
    for (; i<argc; i++) {
        printf("%s -> %s\n", argv[i], query(argv[i]));
    }
    release();
    return 0;
}
