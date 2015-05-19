#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_LEN 1024
int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char send_buffer[BUF_LEN];
    struct sockaddr_in serv_addr;

    if(argc != 3)
    {
        printf("\n Usage: %s <ip> <filename> \n",argv[0]);
        return 1;
    }

    memset(send_buffer, 0,sizeof(send_buffer));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    FILE *fp = fopen(argv[2], "r");
    char c;
    int len = 0;
    while (true) {
        c = fgetc(fp);
        send_buffer[len++] = c;
        if (BUF_LEN - 1 == len)
            write(sockfd, send_buffer, strlen(send_buffer)), len = 0;
        if (EOF == c) break;
    }
    if (len) write(sockfd, send_buffer, strlen(send_buffer)), len = 0;

    return 0;
}
