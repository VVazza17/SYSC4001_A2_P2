#include "common.hpp"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>

static volatile sig_atomic_t running = 1;
void handle(int){ running = 0; }

int main(int argc, char** argv){
    std::signal(SIGINT, handle);
    std::signal(SIGTERM, handle);
    if (argc < 2) return 1;

    int shmid = std::stoi(argv[1]);
    Shared* s = (Shared*)shmat(shmid, nullptr, 0);
    if (s == (void*)-1) return 1;

    while (running && s->counter <= 100) std::this_thread::sleep_for(std::chrono::milliseconds(50));

    while (running) {
        int c = s->counter;
        if (c > 500) break;
        if (c % s->multiple == 0) std::cout << "[P2] counter=" << c << " multiple=" << s->multiple << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }

    shmdt(s);
    return 0;
}
