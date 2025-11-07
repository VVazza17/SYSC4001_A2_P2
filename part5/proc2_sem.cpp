#include "common.hpp"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>

static volatile sig_atomic_t running=1;
void handle(int){ running=0; }

int main(int argc,char** argv){
    std::signal(SIGINT,handle);
    std::signal(SIGTERM,handle);
    if(argc<3) return 1;

    int shmid=std::stoi(argv[1]);
    int semid=std::stoi(argv[2]);
    Shared* s=(Shared*)shmat(shmid,nullptr,0);
    if(s==(void*)-1) return 1;

    while(running){
        P(semid);
        int c=s->counter;
        int m=s->multiple;
        bool start=(c>100);
        bool stop =(c>500);
        V(semid);

        if(stop) break;

        if(start){
            P(semid);
            c=s->counter; m=s->multiple;
            if(c % m == 0) std::cout<<"[P2] counter="<<c<<" multiple="<<m<<std::endl;
            V(semid);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }

    shmdt(s);
    return 0;
}
