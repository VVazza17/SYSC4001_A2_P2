#include "common.hpp"
#include <chrono>
#include <csignal>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static volatile sig_atomic_t running = 1;
void handle(int){ running = 0; }

int main(){
    std::signal(SIGINT, handle);
    std::signal(SIGTERM, handle);

    int shmid = shmget(IPC_PRIVATE, sizeof(Shared), IPC_CREAT | 0600);
    if (shmid < 0) return 1;
    Shared* s = (Shared*)shmat(shmid, nullptr, 0);
    if (s == (void*)-1) return 1;
    s->multiple = 3;
    s->counter = 0;

    pid_t pid = fork();
    if (pid < 0) return 1;
    if (pid == 0) {
        char prog[] = "./proc2_shm";
        std::string id = std::to_string(shmid);
        char* argv2[3]; argv2[0] = prog; argv2[1] = (char*)id.c_str(); argv2[2] = nullptr;
        execv(prog, argv2);
        _exit(127);
    }

    while (running) {
        int c = s->counter;
        if (c % s->multiple == 0) std::cout << "[P1] counter=" << c << " multiple=" << s->multiple << std::endl;
        s->counter = c + 1;
        if (s->counter > 500) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    int st = 0; waitpid(pid, &st, 0);
    shmdt(s);
    shmctl(shmid, IPC_RMID, nullptr);
    return 0;
}
