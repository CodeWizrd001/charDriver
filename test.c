#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    char buffer[100] ;
    char rBuff[100] ;
    int fd;
    fd=open("/dev/keylogger", O_RDWR);
    /*write(fd,buffer,strlen(buffer));
    read(fd,rBuff,10);
    printf("Value : %s \n",rBuff);
    write(fd,buffer,strlen(buffer));
    read(fd,rBuff,10);
    printf("Value : %s \n",rBuff);
    read(fd,rBuff,10);
    printf("Value : %s \n",rBuff);
    read(fd,rBuff,10);
    printf("Value : %s \n",rBuff);*/
    while(1) 
    {
        scanf("%s",buffer) ;
        write(fd,buffer,strlen(buffer));
        read(fd,rBuff,5);
    }
    close(fd);
}