## UniqueLab 2019 Autumn Assignment 4

### 0x01 descriptions

using **semaphore, shared memory, socket and other IPC methods** to deal with the **synchronization problem** between **multi-processes**. using **multi-threads** to deal with **concurrent problems**.

### 0x02 requirements

- sub task 1: read data from the device `/dev/quantum_reader_0` which module-inserted from `quantum.ko`. ensure there is only one process reading from the device at any time in case of device crash (maybe there are many processes waiting).

- sub task 2: read date from the device `/dev/quantum_reader_1` and will get the structure below:

  ```c++
  struct read_data {
    int book_id; 		//book id of this read.
    int book_size; 	//the total size of the book.
    int data_offset; 	//the offset of this read data in the book.
    int data_size; 	//the data size of this read.
    char data[0]; 	//indefinited size array, whose size is data_size byte.
  }
  ```

  if the all received or stored data is beyond 85%, write all this data into a file which stands for the book. the device can be read by multi-processes at a time, what we should deal with is that the data memory can't be read or write at a time.

- sub task 3: use multi-threads to cut some pieces of data from other *books* and save at a *report*. send all the *reports* by TCP socket.

### 0x03 references

Modern Operating Systems (4th Edition)

### 0x04 solutions

for the convenience of semaphore, `./semaphore.h` put some usual functions in a package:

```c++
/* create semephore, verbose = 0 won't show details */
int semaCreate(int *sem_id, key_t key, int verbose);
/* use P method */
int semaP(int sem_id);
/* use V method */
int semaV(int sem_id);
/* delete semephore */
int semaDelete(int sem_id);
// in semaphore methods, if something failed, return -1, but it won't exit instantly.
```

and for shared memory just see `./sharedmemory.h` :

```c++
/* create shared memory, void **shared_memory is the head address */
void shmCreate(int *shm_id, void **shared_memory, key_t key, size_t size);
/* attach the shared memory to the process and get the head address */
void *shmAttach(int shm_id);
/* deatch the shared memory */
void shmDelete(int shm_id, void *shared_memory);
// in shared memory methods, memory allocation failing will lead to exit instantly.
```

for background process you can see `./bgprocess.h` :

```c++
/* when fork a process, parents process will add pid into list */
int backgroundProcessAdd(pid_t pid);
/* when all works finished, clear all processes in list which maybe waiting at background */ 
void backgroundProcessClear();
/* signal functions for parents process */
void backgroundProcessSignal(int signal);
/* added these in the front of main() */
if (signal(SIGCHLD, backgroundProcessSignal) == SIG_ERR) perror("signal handle");
memset(background_process, 0, sizeof(pid_t));
```

especially for sub-task-2, use background process to deal with the reading dynamically, and for sub-task-3, use enough threads to accelerate and use condition variable at the end of main thread in order to wait for all child threads' exit.