#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"
#define semaphore sema
int readFromDevice(int byte_size) {
  int fd;
  if ((fd = open("/dev/quantum_reader_0", O_RDONLY)) == -1) {
		perror("open");
    return -1;
	}
  char buffer[2];
  long int size;
  while (size = read(fd, buffer, 1)) {
    if (write(STDOUT_FILENO, buffer, size) != size) return -1;
    if (!(--byte_size)) break;
  }
  if (size < 0) {
    perror("read");
    return -1;
  }
  close(fd);
  return 0;
}
int main(int argc, char *argv[]) {
  int sem_id;
  semaphoreCreate(&sem_id, (key_t)1234, 1);
  if (argc != 2) {
      printf("arguments lost: using [PROGRAM] [BYTE_SIZE].\n");
      return -1;
  }
  int byte_size = 0;
  for (int i = 0; i < strlen(argv[1]); ++i) {
    if (argv[1][i] <= '9' && argv[1][i] >= '0') {
      byte_size = byte_size * 10 + argv[1][i] - '0';
    } else {
      printf("arguments: bad format, using [PROGRAM] [BYTE_SIZE].\n");
      return -1;
    }
  }
  // printf("%d\n", byte_size);
  // system("ipcs -s");
  semaphoreP(sem_id);
  readFromDevice(byte_size);
  semaphoreV(sem_id);
  return 0;
}