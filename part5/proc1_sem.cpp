#include "common.hpp"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

union semun { int val; struct semid_ds* buf; unsigned short* array; struct seminfo* __buf; };

static volatile sig_atomic_t running=1;
void handle(int){ running=0; }

int main(){
    std::signal(SIGINT,handle);
    std::signal(SIGTERM,handle);

    int shmid=shmget(IPC_PRIVATE,sizeof(Shared),IPC_CREAT|0600);
    if(shmid<0) return 1;
    Shared* s=(Shared*)shmat(shmid,nullptr,0);
    if(s==(void*)-1) return 1;

    int semid=semget(IPC_PRIVATE,1,IPC_CREAT|0600);
    if(semid<0) return 1;
    semun arg; arg.val=1; semctl(semid,0,SETVAL,arg);

    P(semid); s->multiple=3; s->counter=0; V(semid);

    pid_t pid=fork();
    if(pid<0) return 1;
    if(pid==0){
        char prog[]="./proc2_sem";
        std::string a=std::to_string(shmid), b=std::to_string(semid);
        char* argv2[4]; argv2[0]=prog; argv2[1]=(char*)a.c_str(); argv2[2]=(char*)b.c_str(); argv2[3]=nullptr;
        execv(prog,argv2);
        _exit(127);
    }

    while(running){
        P(semid);
        int c=s->counter;
        bool m=(c % s->multiple == 0);
        if(m) std::cout<<"[P1] counter="<<c<<" multiple="<<s->multiple<<std::endl;
        s->counter=c+1;
        bool done = (s->counter>500);
        V(semid);
        if(done) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int st=0; waitpid(pid,&st,0);
    shmdt(s);
    shmctl(shmid,IPC_RMID,nullptr);
    semctl(semid,0,IPC_RMID);
    return 0;
}
