#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include "src/ip.h"

void check(char* ip) {
    printf("check: %s -> %s\n", ip, query((const char*)ip));
}

void check_all_ip(char* file) {
    int fd = open(file, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "open datafile error:%s\n", strerror(errno));
        return;
    }
    struct stat st;
    fstat(fd, &st);
    char* data = (char*)mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == data) {
        fprintf(stderr, "mmap error:%s\n", strerror(errno));
        close(fd);
        return;
    }
    uint32_t start = ((uint32_t*)data)[0], end = ((uint32_t*)data)[1];
    printf("start=%d, end=%d\n", start, end);
    uint32_t p = start, ip = 0;
    while (p < end) {
        ip = ((uint32_t*)(data+p))[0];
        ++ip;
        check(inet_ntoa(*(struct in_addr*)&ip));
        p += 7;
    }
    munmap((void*)data, st.st_size);
    close(fd);
}

int main(int argc, char** argv) {
    char* datafile = "data/qqwry.dat";
    load_ip_data_file(datafile);
    check_all_ip(datafile);
    release();
    return 0;
}
