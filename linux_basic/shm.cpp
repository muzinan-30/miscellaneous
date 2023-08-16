#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>

struct st_pid{
    int pid;
    char name[51];
};

int main(int argc,char* argv[]){
    int shmid;
    if((shmid=shmget(0x5005,sizeof(struct st_pid),0666|IPC_CREAT))<0){
        printf("SHGET failed.\n");
    }

    struct st_pid *stpid=0;
    if((stpid=(struct st_pid*)shmat(shmid,0,0))==(void*)-1){
        perror("shmat");
        printf("shmat failed.\n");
        return -1;
    }

    stpid->pid=getpid();
    strcpy(stpid->name,"1234");
    shmdt(stpid);
    // shmctl(shmid,0,NULL);
    return 0;
}
