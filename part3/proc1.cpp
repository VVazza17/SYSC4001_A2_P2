#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static volatile sig_atomic_t kr=1;
void hs(int){ kr=0; }

int main(){
    std::signal(SIGINT,hs);
    std::signal(SIGTERM,hs);

    pid_t pid=fork();
    if(pid<0) return 1;
    if(pid==0){
        char prog[]="./proc2";
        char* const argv[]={prog,nullptr};
        execv(prog,argv);
        _exit(127);
    }

    long long counter=0,cycle=0;
    while(kr){
        bool m=(counter%3==0);
        if(m) std::cout<<"Cycle number: "<<cycle<<" - "<<counter<<" is a multiple of 3\n";
        else std::cout<<"Cycle number: "<<cycle<<"\n";
        ++counter; ++cycle;

        int st=0;
        pid_t r=waitpid(pid,&st,WNOHANG);
        if(r==pid) break;

        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}
