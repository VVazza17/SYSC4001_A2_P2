#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>

static volatile sig_atomic_t running = 1;
void handle(int){ running = 0; }

int main(){
    std::signal(SIGINT, handle);
    std::signal(SIGTERM, handle);

    pid_t pid = fork();
    if(pid < 0) return 1;

    if(pid == 0){
        long long counter = 0;
        long long cycle = 0;
        while(running){
            std::cout << "[Process 2] Cycle: " << cycle << " | Counter: " << counter << std::endl;
            counter++;
            cycle++;
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        return 0;
    }

    long long counter = 0;
    long long cycle = 0;
    while(running){
        std::cout << "[Process 1] Cycle: " << cycle << " | Counter: " << counter << std::endl;
        counter++;
        cycle++;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    return 0;
}
