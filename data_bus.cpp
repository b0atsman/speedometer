#include <iostream>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 

using namespace std;

double *buffer = nullptr;
key_t key;
int shmid;

int init() {
    if (buffer == nullptr) {
        key = ftok("speed", 'S');
	if (key == -1) { 
	    //return 1; -- always indicates error
	}

        shmid = shmget(key, sizeof(double), 0644 | IPC_CREAT); 
	if (shmid == -1) 
	    return 2;

        buffer = (double*) shmat(shmid, (void*) 0, 0);
	if (buffer == (double *)(-1)) 
	    return 3;
    }

    return 0; 
}

void deinit() {
    
    *buffer = 0.0;

    shmdt(buffer);
    
    buffer = nullptr;
}

void populate(double data)
{
    if (buffer != nullptr) {
        *buffer = data;
	cout << *buffer << endl;
    }
}

double consume() 
{
    if (buffer != nullptr) {
      return *buffer;
    } else {
      return 0; 
    }
};
