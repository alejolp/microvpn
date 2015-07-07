#include <iostream>
#include <cstring>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

using namespace std;

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h> 
#include <linux/if.h>
#include <linux/if_tun.h>


int tap_alloc(char *dev)
{
    struct ifreq ifr;
    int fd, err;

    if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
       return -1;

    memset(&ifr, 0, sizeof(ifr));

    /* Flags: IFF_TUN   - TUN device (no Ethernet headers) 
     *        IFF_TAP   - TAP device  
     *
     *        IFF_NO_PI - Do not provide packet information  
     */ 
    ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
    if( *dev )
       strncpy(ifr.ifr_name, dev, IFNAMSIZ);

    if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
       close(fd);
       return err;
    }
    strcpy(dev, ifr.ifr_name);
    return fd;
}              


int main (int argc, char** argv) {
    int dev;
    char devname[IFNAMSIZ+1] = "";

    dev = tap_alloc(devname);
    if (dev < 1) {
        std::cerr << "Error " << errno << " " << strerror(errno) << std::endl;
        exit(1);
    }

    return 0;
}
