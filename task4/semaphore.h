#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <errno.h>
#ifdef _SEM_SEMUN_UNDEFINED
union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */
};
#endif
int semaInit(int sem_id) {
  union semun sem_union;
  sem_union.val = 1;
  if (semctl(sem_id, 0, SETVAL, sem_union) == -1) {
    perror("open semaphore");
    return -1;
  }
  return 0;
}
int semaCreate(int *sem_id, key_t key, int verbose) {
  int nsems = 1; // number of semaphore in semaphore set.
  *sem_id = semget(key, nsems, 0666 | IPC_CREAT);
  if (*sem_id < 0) {
    perror("create semaphore");
    return -1;
  } else {
    if (verbose) printf("created semaphore id = %d\n", *sem_id);
    semaInit(*sem_id);
  }
}
int semaDelete(int sem_id) {
  union semun sem_union;
  if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1) {
    perror("delete semaphore");
    return -1;
  }
  return 0;
}
int semaP(int sem_id) {
  struct sembuf sem_buf;
  sem_buf.sem_num = 0;
  sem_buf.sem_op = -1; // pause to wait for resrc.
  sem_buf.sem_flg = SEM_UNDO;
  while (1) {
    if (semop(sem_id, &sem_buf, 1) == -1) {
      if (errno == EINTR) continue;
      perror("semaphore P");
      return -1;
    } else {
      break;
    }
  }
  return 0;
}
int semaV(int sem_id) {
  struct sembuf sem_buf;
  sem_buf.sem_num = 0;
  sem_buf.sem_op = 1; // release the resrc.
  sem_buf.sem_flg = SEM_UNDO; 
  while (1) {
    if (semop(sem_id, &sem_buf, 1) == -1) {
      if (errno == EINTR) continue;
      perror("semaphore V");
      return -1;
    } else {
      break;
    }
  }
  return 0;
}
#endif