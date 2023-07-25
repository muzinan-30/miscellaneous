#include<iostream>
#include<unistd.h>
#include<cstdio>
#include<cstdlib>
#include<sys/types.h>
#include<sys/wait.h>
//pcb的本质是一个task_struct pid（进程标识符，父进程标识符，用户标识符，组标识符）进程组，进程环境，进程的运行状态等 
using namespace std;
//fork() 子进程创建成功时 父进程的fork返回0， 子进程的fork不返回
//fork创建失败时 父进程返回－1
int main(){
    pid_t p1=getpid();
    pid_t p2=getppid();
    pid_t pid,w,p;
    int state;
    int i;
    // for(i=0;i<5;i++){
    //     if((pid=fork())==0) break;
    // }
    pid_t child1=fork();
    if(child1==-1){
        perror("创建子进程失败");
        exit(1);
    }
    else if(child1==0){ //子进程沉睡
        sleep(5);
    }else if(child1>0){
        // sleep(i);
        // cout<<"子进程pid"<<getpid()<<"ppid"<<getppid()<<endl;
        // execl("/bin/ls","-a","-l","PCB.cc",NULL);
        // perror("error exec\n");
        p=child1;//获取第一个子进程的id
        //父进程继续创建子进程 保证是父进程创建的子进程 (pid＝fork())==0
        for(int i=0;i<3;i++){
            if((pid=fork())==0){
                break;
            }
        }

        if(pid==-1){ //创建失败
            perror("fork error");
            exit(2);
        }else if(pid==0){
            sleep(1);
            cout<<"子进程的pid:"<<getpid()<<endl;
            exit(0);
        }else if(pid>0)
        {
            w=waitpid(p,NULL,0);
            if(w==p){
                cout<<"进程已经退出:"<<w<<endl;
            }
            else{
                cout<<"waitpid error"<<endl;
            }
            w=waitpid(pid,NULL,WNOHANG);
            if(w==pid){
                cout<<"进程已经退出:"<<w<<endl;
            }
            else{
                cout<<"waitpid error"<<endl;
            }
        }
        // w=wait(&state); //不需要记录子进程退出时的状态
        // cout<<"子进程的状态为"<<WIFEXITED(state)<<endl;
        
    }

    
    return 0;
}
