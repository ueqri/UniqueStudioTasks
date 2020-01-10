#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#define LOW_VERBOSE
#define BGP_FORCE_EXIT
#include "semaphore.h"
#include "sharedmemory.h"
#include "bgprocess.h" //for mutiprocess.
#define BOOK_NUMBER 130
#define SHM_NULL -1

double delim = 0.85;
struct read_data {
  int book_id; //book id of this read.
  int book_size; //the total size of the book.
  int data_offset; //the offset of this read data in the book.
  int data_size; //the data size of this read.
  char data[0]; //indefinited size array, whose size is data_size byte.
};
struct block_info {
  int loded;
  int size;
  int shm_dest_desc;
};
int shm_block[BOOK_NUMBER];
int sema_block[BOOK_NUMBER];
void readRaw(int fd, struct read_data *data) {
  long int size = read(fd, data, 4105);
  if (size < 0) {
		perror("read");
  	exit(EXIT_FAILURE);
	}
}
void writeInOrder(int book_id, unsigned char *begin, int book_size) {
  FILE *fp = NULL;
  char file_name[25];
  sprintf(file_name, "./data/book%d.txt", book_id);
  fp = fopen(file_name, "w+");
  while (book_size--) {
    // fputc(*begin, fp);
    fprintf(fp, "%hhu", *begin);
    begin++;
  }
  fclose(fp);
}
int check(int *shm_memory) {
  int result = 0;
  if (*shm_memory >= BOOK_NUMBER) result = 1; 
  return result;
}
int main() {
  /* initial for multiprocess */
  if (signal(SIGCHLD, backgroundProcessSignal) == SIG_ERR) perror("signal handle");
  memset(background_process, 0, sizeof(pid_t));
  /* open read device */ 
  int fd = open("/dev/quantum_reader_1", O_RDONLY);
  if (fd == -1) {
		perror("open");
    return -1;
	}
  struct read_data *data;
  data = (struct read_data *)malloc(4105);
  
  //use (key_t)2000 to 2999 for semaphore, 
  //use (key_t)1000 to 1999 for shared memory.

  /* supervision for count books between multiprocess */
  int shm_sv, sema_sv;
  int *supervision;
  semaCreate(&sema_sv, (key_t)2200, 0); //2200 for sem_sv.
  shmCreate(&shm_sv, (void *)&supervision, (key_t)1200, sizeof(int)); //1200 for shm_sv.
  /* block struct for each book contains shared memory & semaphore */
  struct block_info *block[BOOK_NUMBER];
  for (int i = 0; i < BOOK_NUMBER; ++i) {
    shmCreate(&shm_block[i], (void *)&block[i], (key_t)(1000 + i), sizeof(struct block_info));
    memset(block[i], 0, sizeof(struct block_info));
    block[i]->shm_dest_desc = SHM_NULL;
    semaCreate(&sema_block[i], (key_t)(2000 + i), 0);
  }

  while (!check(supervision)) {
    pid_t pid = fork();
    if (pid == -1) {
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      /* clear the left process when finished */
      if (check(supervision)) {
        printf("!");
        exit(EXIT_SUCCESS);
      }
      readRaw(fd, data); //read data from device.
      int id = data->book_id;
      semaP(sema_block[id]);
        block[id]->size = data->book_size;
        int loded_size = block[id]->loded, total_size = block[id]->size;
        /* skip the finished, and consider double error value */
        if (loded_size > 0 && block[id]->shm_dest_desc == SHM_NULL) {
          exit(EXIT_SUCCESS); // continue; <-remember!
        }
        unsigned char *book, *book_begin;
        /* attach the shared memory. */
        if (block[id]->shm_dest_desc == SHM_NULL) {
          shmCreate(&block[id]->shm_dest_desc, (void *)&book, (key_t)(1300 + id), total_size);
          memset(book, 0xff, total_size);
        } else {
          book = shmAttach(block[id]->shm_dest_desc);
        }
        book_begin = book;
        book += data->data_offset;
        int valid_count = 0;
        for (int i = 0; i < data->data_size; ++i, ++book) {
          /* count valid text from device */
          if (*book == 0xff) {
            valid_count++;
            *book = data->data[i];
          }
          /* check to write down */
          if ((double)(loded_size + valid_count) >= (delim)*((double)total_size)) {
            writeInOrder(id, book_begin, total_size);
            semaP(sema_sv);
              (*supervision)++;
              printf("\e[32;1mbook %d(%d) completed!\e[0m(%d process inside)\n", id, 
                *supervision, background_process_count);
            semaV(sema_sv);
            /* delete shared memory dynamticly, and detach the link */
            shmDelete(block[id]->shm_dest_desc, book_begin);
            block[id]->shm_dest_desc = SHM_NULL;
            break;
          }
        }
        block[id]->loded += valid_count;
      semaV(sema_block[id]);
      exit(EXIT_SUCCESS);
    } else {
      while (backgroundProcessAdd(pid)) ;
    }
  }
  backgroundProcessClear();
  return 0;
}
