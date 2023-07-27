#include<cstdio>
#include<cstdlib>
#include<cstring>

#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
/*
inux系统中将管道视为文件，可以通过文件操作接口来进行管道的操作
管道被设计为环形的数据结构，实现管道的循环利用，方便内核对于管道的管理
匿名管道：进程退出就会销毁，使用匿名的管道只能用于有亲缘关系的进程
命名管道：操作和文件操作一样，被具名为一个文件，进程退出后，依旧存在
管道的本质是：修改进程中文件描述符对应file文件的inode指向的索引节点
匿名管道：
1. 管道采用半双工，单向传递，需要关闭不必要通信端口
2. 管道只能进行半双工，全双工需要创建两个管道
3. 只有指向读端的文件描述符打开时，写入数据才有意义，否则写端的进程会收到内核传来的信号
4. 写端关闭读端仍可以读取数据，当管道中全部剩余数据被读取完后，再次read会返回0
5. 进程写端的描述符没有关闭，管道写端的进程也没有向管道中写入新的数据，当进程从管道中读取数据且管道中所有的数据都被读取时，再次read读取会被阻塞，直到写端向管道写入数据，阻塞才会解除
6. 没有进程读端读取，但是一直写，缓存区写满时会阻塞
7. 管道中使用字节流，要求管道两端的进程约定好数据的格式
命名管道
本质和匿名管道一样，都是内核管理的缓存*/
// int main(){
//     int fd[2];
//     int ret=pipe(fd);
//     if(ret==-1){
//         perror("create pipe failed!!!");
//     }
//     //新建子进程
//     pid_t child_process=fork();
//     if(child_process){
//         //fork创建成功时会返回pid，因此如果fork返回值大于零，这就是在父进程中的代码逻辑
//         //先读后写 0对应的是读的文件描述符
//         close(fd[0]);
//         char* p="hello,pipe this is your father!\n";
//         write(fd[1],p,strlen(p)+1);
//         close(fd[1]); //释放内存
//         wait(NULL); //父进程挂起进入阻塞状态，直到子进程进入僵尸态，wait捕获到子进程的退出信息才会转变为运行态
//     }else if(child_process==0){
//         //子进程-读
//         close(fd[1]);
//         char buffer[1024]={0};
//         ret=read(fd[0],buffer,sizeof(buffer));  //read返回接受数据的长度
//         close(fd[0]);
//         write(STDOUT_FILENO,buffer,ret);
//     }
// }

//实现兄弟进程间的管道实现 使用dup2进行重定向 将文件符传递，返回newfd
// int main(){
//     printf("test");
//     int fd[2];
//     int ret=pipe(fd);
//     if(ret==-1){
//         perror("pipe create failed!");
//         exit(1);
//     }

//     int i;
//     pid_t pid,wpid;
//     for(int i=0;i<2;i++){
//         if((pid==fork())==0){
//             break;
//         }
//     }

//     if(2==i){
//         close(fd[0]);
//         close(fd[1]);
//         wpid=wait(NULL);
//         printf("wait child1 success!pid=%d",wpid);
//         pid=wait(NULL);
//         printf("wait child2 success!pid=%d",pid);
//     }else if(i==0) //子进程写
//     {
//         close(fd[0]);
//         dup2(fd[1],STDOUT_FILENO);
//         execlp("ls","ls",NULL);
//     }
//     else if(i==1){
//         close(fd[1]);
//         dup2(fd[0],STDIN_FILENO);
//         execlp("wc","wc","-l",NULL);
//     }
//     return 0;
// }

//popen pipe创建管道，fork创建子进程，execve执行shell命令
// int main(){
//     FILE* r_fd,*w_fd;
//     char buf[100];
//     r_fd=popen("ls","r");
//     w_fd=popen("wc -l","w");
//     while(fgets(buf,sizeof(buf),r_fd)!=NULL){
//         fputs(buf,w_fd);
//     }

//     pclose(r_fd);
//     pclose(w_fd);
//     return 0;
// }




