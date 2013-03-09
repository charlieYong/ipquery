#include <stdio.h>
#include "ip.h"

int main(int argc, char** argv) {
    char* datafile = "/Users/charlie/code/github/ipquery/data/qqwry.dat";
    if (load_ip_data_file(datafile)) {
        printf("error\n");
        return -1;
    }

    char* ip = "119.254.241.68";
    printf("%s:%s\n", ip, query(ip));
    release();
    return 0;
}
