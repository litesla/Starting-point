/*************************************************************************
	> File Name: k.c
	> Author: x
	> Mail: x.com
	> Created Time: 2018年09月23日 星期日 16时36分46秒
 ************************************************************************/

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "common.h"
#include "queue.h"

#define MAX_N 5 

void *func(void *argv) {
    Queue *q = (Queue *)argv;
    while (1) {
        output(q);
        sleep(2);
        pop(q);
        sleep(5);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int fd, new_fd, struct_len, numbytes, i;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char buff[BUFSIZ]; // 缓冲区
    char server_port[100];

    int g = get("common.conf", "port", server_port);

    server_addr.sin_family = AF_INET; // IP地址
    server_addr.sin_port = htons(atoi(server_port)); // 端口
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8); // 地址族协议
    struct_len = sizeof(struct sockaddr_in);

    fd = socket(AF_INET, SOCK_STREAM, 0); // socket
    while(bind(fd, (struct sockaddr *)&server_addr, struct_len) == -1); // bind 绑定
    printf("绑定成功\n");
    while(listen(fd, 10) == -1); // 进入监听状态
    printf("正在监听中......\n");
    Queue *q[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        q[i] = init(1000, i);
    }
    int num = 0, onli;
    pthread_t t[MAX_N];
    char IP[100];
    int port;
    for (int i = 0; i < MAX_N; i++) {
        pthread_create(&t[i], NULL, func, (void *)q[i]);
    }
    while (1) {
        new_fd = accept(fd, (struct sockaddr *)&client_addr, &struct_len);
        printf("出现访客\n");
        numbytes = send(new_fd, "欢迎,祝您愉快!\n", 21, 0);
        strcpy(IP, inet_ntoa(client_addr.sin_addr)); 
        port = htons(client_addr.sin_port);
        
        num += 1;
        push(q[num % MAX_N], IP, port);
        printf("%s %d\n", IP, port);
        //output(q);
      
    }
    return 0;
}

