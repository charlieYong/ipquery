#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <iconv.h>

#include "ip.h"

#define IP_SIZE 4
#define INDEX_SIZE 7
#define MAX_IP_INFO_LEN 2048

typedef struct _ipdata {
    uint32_t indexstart;
    uint32_t indexend;
    uint32_t total_index_num;
    unsigned char* data;
    size_t size;
} _ipdata;

static _ipdata ipdata;
static unsigned char result[MAX_IP_INFO_LEN];

int load_ip_data_file(char* datafile) {
    int fd = open(datafile, O_RDONLY);
    if (-1 == fd) {
        fprintf(stderr, "IpQuery open file failed:%s\n", strerror(errno));
        abort();
    }
    struct stat st;
    if (fstat(fd, &st) < 0)
        return -1;
    memset(&ipdata, 0, sizeof(ipdata));
    memset(result, 0, MAX_IP_INFO_LEN);
    void* addr = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr)
        return -1;
    ipdata.data = (unsigned char*)addr;
    ipdata.size = st.st_size;
    ipdata.indexstart = ((uint32_t*)addr)[0];
    ipdata.indexend = ((uint32_t*)addr)[1];
    ipdata.total_index_num = (ipdata.indexend - ipdata.indexstart) / INDEX_SIZE;
    close(fd);
    return 0;
}

void release(void) {
    if (NULL != ipdata.data) {
        munmap((void*)ipdata.data, ipdata.size);
        ipdata.data = NULL;
    }
}

uint32_t ip2long(char* ip) {
    struct in_addr addr;
    if (inet_aton(ip, &addr) <= 0)
        return 0;
    return htonl(addr.s_addr);
}

static uint32_t get_data_offset(uint32_t offset) {
    unsigned char buf[4] = {0};
    memcpy(buf, ipdata.data+offset, 3);
    return *(uint32_t*)buf;
}

static uint32_t get_endip_by_offset(uint32_t offset) {
    uint32_t data_offset = get_data_offset(offset);
    return ((uint32_t *)(ipdata.data+data_offset))[0];
}

static uint32_t get_single_location(uint32_t offset, char* buf, int buflen, uint32_t* next_offset) {
    uint32_t _offset = offset;
    while (_offset > 0 && 2 == ipdata.data[_offset]) {
        *next_offset = _offset + 4;
        _offset = get_data_offset(_offset + 1);
    }
    if (_offset <= 0)
        return 0;
    char* ptr = (char*)(ipdata.data+_offset);
    char* ptrend = strchr(ptr, '\0');
    if (NULL == ptrend)
        return 0;
    uint32_t len = ptrend - ptr;
    if (offset == _offset)
        *next_offset = _offset + len + 1;
    if (len >= buflen)
        return 0;
    memcpy(buf, ptr, len);
    return len;
}

static char* get_ip_info(uint32_t offset) {
    uint32_t next_offset = 0, len = 0;
    uint32_t data_offset = get_data_offset(offset) + 4;
    while (1 == ipdata.data[data_offset]) {
        data_offset = get_data_offset(data_offset + 1);    
    }
    len = get_single_location(data_offset, result, MAX_IP_INFO_LEN, &next_offset);
    result[len++] = ' ';
    len += get_single_location(next_offset, result+len, MAX_IP_INFO_LEN-len, &next_offset);
    return result;
}

char* query(char* ip) {
    if (NULL == ip || NULL == ipdata.data)
        return "";
    uint32_t iplong = ip2long(ip);
    if (0 == iplong)
        return "";
    memset(result, 0, MAX_IP_INFO_LEN);
    uint32_t offset, brk, low, mid, high, indexip, endip;
    low = 0;
    high = ipdata.total_index_num - 1;
    while (low <= high) {
        mid = (low + high) / 2;
        offset = ipdata.indexstart + mid * INDEX_SIZE;
        indexip = ((uint32_t *)(ipdata.data+offset))[0];
        if (iplong == indexip) {
            return get_ip_info(offset+IP_SIZE);
        } else if (iplong < indexip) {
            high = mid -1;
        } else {
            if (get_endip_by_offset(offset+IP_SIZE) >= iplong)
                return get_ip_info(offset+IP_SIZE);
            low = mid + 1;
        }
    }
    return "";
}
