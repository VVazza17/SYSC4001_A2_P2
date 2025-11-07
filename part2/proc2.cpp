#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>

static volatile sig_atomic_t kr=1;
void hs(int){ kr=0; }

int main(){
    std::signal(SIGINT,hs);
    std::signal(SIGTERM,hs);

    long long counter=0,cycle=0;
    while(kr){
        bool m=(counter%3==0);
        if(m) std::cout<<"Cycle number: "<<cycle<<" - "<<counter<<" is a multiple of 3\n";
        else std::cout<<"Cycle number: "<<cycle<<"\n";
        --counter; ++cycle;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    return 0;
}
