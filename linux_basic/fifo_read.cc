#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>

int main(int argc,char* argv[]){
    if(argc<2){
        printf("./a.out fifoname!\n");
        exit(1);
    }

    int ret=access(argv[1],F_OK);
    if(ret==-1){
        int r=mkfifo(argv[1],0664);
        if(r==-1){
            perror("mkfifo failed!\n");
            exit(1);
        }
        else
        {
            printf("fifo create success!\n");
        }
    }
    //打开文件
    int fd=open(argv[1],O_RDONLY);
    if(fd==-1){
        perror("open");
        exit(1);
    }
    while(1){
        char buffe[1024]={0};
        read(fd,buffe,strlen(buffe));
        printf("buf=%s",buffe);
    }
    close(fd);
    return 0;
}
