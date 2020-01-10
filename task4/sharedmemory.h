#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

void shmCreate(int *shm_id, void **shared_memory, key_t key, size_t size) {
  //*shared_memory = (void *)0;
  *shm_id = shmget(key, size, 0666 | IPC_CREAT);
  if (*shm_id == -1) {
    perror("shmget");
    exit(EXIT_FAILURE);
  }
  *shared_memory = shmat(*shm_id, (void *)0, 0);
  if (*shared_memory == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }
  memset(*shared_memory, 0, size);
}
void *shmAttach(int shm_id) {
  void *shared_memory = shmat(shm_id, (void *)0, 0);
  if (shared_memory == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }
  return shared_memory;
}
void shmDelete(int shm_id, void *shared_memory) {
  if (shmdt(shared_memory) == -1) {
    perror("shmdt");
    exit(EXIT_FAILURE);
  }
  if (shmctl(shm_id, IPC_RMID, 0) == -1) {
    perror("shmctl(IPC_RMID)");
    exit(EXIT_FAILURE);
  }
}
#endif