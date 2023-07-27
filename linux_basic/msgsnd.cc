#include<cstdio>
#include<cstdlib>
#include<sys/types.h>
#include<sys/msg.h>
#include<cstring>
const int MAX_TEXT=512;
//消息队列
//系统中最大的消息队列数 和队列中最大的消息数存在限制 MSGMNI MSGTOL
//msgget() msgsnd() msgrcv() msgctl()
// struct msgbuf{ long int msgtypel anytype data;}
struct my_msg_st{
    long int my_msg_st;
    char anytype[MAX_TEXT];
};
int main(){

    int idx=1;
    int msqid;
    struct my_msg_st data;
    char buf[BUFSIZ];  //设置缓存变量
    int msgqid=msgget((key_t)1000,0664|IPC_CREAT);
    if(msgqid==-1){
        perror("msgget error");
        exit(-1);
    }
    while(idx<5){
        printf("enter some text:");
        fgets(buf,BUFSIZ,stdin);
        data.my_msg_st=rand()%3+1;
        strcpy(data.anytype,buf);
        if(msgsnd(msgqid,(void*)&data,sizeof(data),IPC_NOWAIT)==-1){perror("msgsnd error!");exit(1);};//最后一位标志设置0，调用函数的进程会被挂起，直到消息写入消息队列为止
        idx++;
    }
    return 0;
}
