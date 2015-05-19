#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define BUF_LEN 10
int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, n = 0;
    struct sockaddr_in serv_addr;
    char rec_buffer[BUF_LEN];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, 0, sizeof(serv_addr));
    memset(rec_buffer, 0, sizeof(rec_buffer));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    int cnt = 0;
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

        char filename[100]; sprintf(filename, "%d.txt", cnt);
        FILE *fp = fopen(filename, "w");
        while ( (n = read(connfd, rec_buffer, sizeof(rec_buffer)-1) ) > 0)
        {
            printf("receice %d byte\n", n);
            rec_buffer[n] = 0;
            for (int i = 0; i < n; i++)
                if (rec_buffer[i] == EOF)
                    rec_buffer[i] = 0;
            fprintf(fp, "%s", rec_buffer);
            if(fputs(rec_buffer, stdout) == EOF)
            {
                printf("\n Error : Fputs error\n");
            }
            putchar('\n');
        }
        printf("saving file to %s\n", filename);
        cnt++;
        fclose(fp);

        close(connfd);
        sleep(1);
     }
}
