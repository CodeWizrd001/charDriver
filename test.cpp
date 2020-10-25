#include <fcntl.h>
#include <stdio.h>

using namespace std ;

int main()
{
    char buffer[1];
    int fd;
    fd=open("/dev/keylogger ", O_RDWR );
    buffer[0]=0x00;
    write(fd,buffer,1,NULL);
    read(fd,buffer,1,NULL);
    printf("Value : 0x %02x\n",buffer[0]);
    close(fd);
}