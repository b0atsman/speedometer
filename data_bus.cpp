#include <iostream>
#include <fstream>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <pthread.h>

namespace data_bus {

    // data buffer
    double *buffer = nullptr;

    // mutex
    pthread_mutex_t *mutex = nullptr;
    pthread_mutexattr_t mutex_attr;

    // file name for key generation
    const char *pathName = "/tmp/speed_data";

    // shared memory key & id
    key_t key;
    int shmid;

    using namespace std;

    /* 
    /  Initialization routine
    */

    int init() 
    {
        if (buffer == nullptr) 
        {
            // checking/creating data file for 'ftok' call
	    fstream dataFile;
	    dataFile.open(pathName, fstream::in | fstream::out | std::fstream::trunc);
	    if (!dataFile) {
   	    	cout << "Can't create " << pathName << endl;
	        return 1;
            }
	    dataFile.close();

	    // creating key for 'shmget' call
            key = ftok(pathName, 'S');
	    if (key == -1) 
            { 
	        perror("ftok");
	        cout << pathName << endl;
	        return 1;
	    }

	    // creating shared memory block
            shmid = shmget(key, sizeof(pthread_mutex_t) + sizeof(double), 0644 | IPC_CREAT); 
	    if (shmid == -1) 
            {
	        perror("shmget");
	        return 1;
    	    }

            // creating shared mutex
	    mutex = (pthread_mutex_t *) shmat(shmid, (void*) 0, 0);
            if (mutex == (void *)(-1)) 
            {
	        perror("shmat");	
	        return 1;
	    }
            
            pthread_mutexattr_init(&mutex_attr);
            pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);

            int res = pthread_mutex_init(mutex, &mutex_attr);
	    if (res) {
                perror("pthread_mutex_init");
                return 1;
            }   	

            // setting buffer pointer to shared memory
            buffer = (double*) (mutex + 1);
        }

        return 0; 
    }

    /*
    / Deinitialization routine
    */

    void deInit() 
    {

	// resetting speed value
        *buffer = 0.0;

        // destroying mutex
        pthread_mutex_destroy(mutex);
	
	// detaching shared memory
        shmdt(mutex);
    
	// resetting pointers
        buffer = nullptr;
        mutex = nullptr;
    }

    /* 
    / Writing value of type 'double'
    */

    void populate(double data)
    {
        if (buffer != nullptr) 
        {
            // exclusively writing to buffer
            pthread_mutex_lock(mutex);
            *buffer = data;
            pthread_mutex_unlock(mutex);

            // monitoring
	    cout << *buffer << endl;
        }
    }
    
    /*
    / Reading value of type 'double'
    */

    double consume() 
    {
	double result;

        if (buffer != nullptr) 
        {
            // exclusively reading from buffer
            pthread_mutex_lock(mutex);
	    result = *buffer;
            pthread_mutex_unlock(mutex);

            return result;
        } 
        else 
        {
            return 0.0; 
        }
    }
}
