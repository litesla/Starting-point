/*************************************************************************
	> File Name: k.c
	> Author: x
	> Mail: x.com
	> Created Time: 2018年09月23日 星期日 16时36分46秒
 ************************************************************************/

#include "../common/common.h"
#include "linklist_1.h"
#include "hash.h"

typedef struct argv_lh {
    LinkList *l;
    HashTable *h;
} argv_lh;

void *func(void *argv) {
    LinkList *l = ((argv_lh *)argv)->l;
    HashTable *h = ((argv_lh *)argv)->h;
    LinkNode *p = l->head.next;
    while (1) {
        output(l);
        if (p == NULL) {
            p = l->head.next;
            sleep(5);
            continue;
        }

        LinkNode *del_node = p;
        int con_fd = soc_con(p->IP, p->port);
        if (con_fd < 0) {
            p = p->next;
            del_hash(h, del_node->IP);
            del(l, del_node);
            sleep(5);
            continue;
        }

        char IP[100];
        int port, numbytes;
        char buff[BUFSIZ] = {0};
        strcpy(IP, p->IP); 
        port = p->port;
        while((numbytes = recv(con_fd, buff, BUFSIZ, 0)) > 0) { // 接收数据
            char filename[200] = {0}, data[BUFSIZ] = {0}, dirname[100] = "log\0", temp_name[100] = {0};
            int t = buff[0] - '0', k1 = strlen(IP), k2 = strlen(dirname), k3 = strlen(IP) + strlen(dirname);
            mkdir(dirname, 0775); // 同linux mkdir建立目录后的权限一致
            dirname[k2] = '/';
            strncpy(dirname + 1 + k2, IP, k1);
            mkdir(dirname, 0775); // 同linux mkdir建立目录后的权限一致
            
            strncpy(filename, dirname, k3);
            filename[k3] = '/';
            
            strncpy(temp_name, buff + 1, t);
            strncpy(filename + 1 + k3, buff + 1, t);
            strncpy(data, buff + 1 + t, strlen(buff + 1 + t));
            FILE *fd;
            fd = fopen(filename, "a+");
            fwrite(data, sizeof(char), strlen(data), fd);
            printf("收到客户 %s 的日志：%s\n", IP, temp_name);
            fclose(fd);
            memset(buff, 0, sizeof(buff));
        }
        printf("访客\"%s %d\"退出\n", IP, port);
        p = p->next;
        close(con_fd);
        sleep(5);
    }
    return NULL;
}

void func_alert(char *buff, char *IP) {
    char data[BUFSIZ] = {0};
    mkdir("log", 0775);
    strncpy(data, IP, strlen(IP));
    data[strlen(data)] = ':';
    data[strlen(data)] = '\n';
    strncpy(data + strlen(data), buff + 1, strlen(buff + 1));
    FILE *fd;
    fd = fopen("log/alert.log", "a+");
    fwrite(data, sizeof(char), strlen(data), fd);
    printf("收到警报信息！！！\n%s\n", data);
    fclose(fd);
    return ; 
}

int main(int argc, char *argv[]) {
    int fd, new_fd, struct_len, numbytes;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    char buff[BUFSIZ]; // 缓冲区
    char server_port[100], temp_ins[100], client_port[100];

    int ret_g = get("../common/common.conf", "port", server_port);
    ret_g = get("../common/common.conf", "ins", temp_ins);
    ret_g = get("../common/common.conf", "client_port", client_port);

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
    
    int MAX_N = atoi(temp_ins);
    LinkList *l[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        l[i] = init(i);
    }
    for (int i = 0; i < 10; i++) {
        char temp_IP[100] = {0}, temp_port[100] = {0};
        get_IP("ip", i, temp_IP);
        get_IP("port", i, temp_port);
        insert(l[i % MAX_N], temp_IP, atoi(temp_port));
    }

    HashTable *h = init_hashtable(10000);
    for (int i = 0; i < 10; i++) {
        char temp_IP[100] = {0};
        get_IP("ip", i, temp_IP);
        insert_hash(h, temp_IP);
    }
    
    pthread_t t[MAX_N];
    for (int i = 0; i < MAX_N; i++) {
        argv_lh *temp_lh = (argv_lh *)malloc(sizeof(argv_lh));
        temp_lh->l = l[i];
        temp_lh->h = h;
        pthread_create(&t[i], NULL, func, (void *)temp_lh);
    }

    char IP[100];
    int port;
    int num = 0;
    while (1) {
        memset(buff, 0, sizeof(buff));
        new_fd = accept(fd, (struct sockaddr *)&client_addr, &struct_len);
        strcpy(IP, inet_ntoa(client_addr.sin_addr)); 
        port = htons(client_addr.sin_port);
        
        numbytes = recv(new_fd, buff, BUFSIZ, 0);
        if (buff[0] == '1') {
            func_alert(buff, IP);
        }
        numbytes = send(new_fd, "欢迎,祝您愉快!\n", 21, 0);
        close(new_fd); 
        if (search_hash(h, IP) == 1) continue;
        printf("出现访客\n");
        printf("%s %d\n", IP, port);

        num += 1;
        printf("第 %d 位新访客加入队列\n", num); 
        insert(l[num % MAX_N], IP, atoi(client_port));
        insert_hash(h, IP);
    }

    return 0;
}

