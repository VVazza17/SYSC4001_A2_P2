#pragma once
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

struct Shared { int multiple; int counter; };

inline void P(int semid){ sembuf op{0,-1,0}; semop(semid,&op,1); }
inline void V(int semid){ sembuf op{0,1,0};  semop(semid,&op,1); }
