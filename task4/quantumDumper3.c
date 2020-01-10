#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BOOK_NUMBER 130

pthread_mutex_t mtx_end;
pthread_cond_t  condition_var = PTHREAD_COND_INITIALIZER;
int finished_threads_count = 0;

pthread_mutex_t mtx_socket;
pthread_mutex_t mtx_file[BOOK_NUMBER];
int socket_fd;

int bookNext(int book_id) {
  return (book_id + 1) % BOOK_NUMBER;
}
long _fileSize(FILE *fp) {
  long last_offset = ftell(fp);
  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);
  fseek(fp, last_offset, SEEK_SET);
  return size;
}
long fileSize(int book_id) {
  FILE *fp = NULL;
  char file_name[25];
  pthread_mutex_lock(&mtx_file[book_id]);
    sprintf(file_name, "./data/book%d.txt", book_id);
    fp = fopen(file_name, "r");
    long size = _fileSize(fp);
    fclose(fp);
  pthread_mutex_unlock(&mtx_file[book_id]);
  return size;
}
void fileWrite(int book_id, const char *buffer) {
  FILE *fp = NULL;
  char file_name[25];
  sprintf(file_name, "./report/book%d.txt", book_id);
  fp = fopen(file_name, "w+");
  fputs(buffer, fp);
  fclose(fp);
}

/* cut part of text from (int)book_id into (char *)buffer, if 
(int)cut_size == 0 , will cut dafult size as 10% of file_size.*/
int cutSentences(int book_id, char *buffer, int cut_size) {
  FILE *fp = NULL;
  char file_name[25];
  sprintf(file_name, "./data/book%d.txt", book_id);
  pthread_mutex_lock(&mtx_file[book_id]);
    fp = fopen(file_name, "r");
    int file_size = _fileSize(fp);
    if (cut_size == 0) cut_size = file_size / 10;
    if (cut_size > file_size) cut_size = file_size;
    fgets(buffer, cut_size, fp);
    fclose(fp);
  pthread_mutex_unlock(&mtx_file[book_id]);
  return cut_size;
}

/* connect with [inet4_address:port] successfully return socket_fd, failed return -1. */
int socketClientCreate(const char *inet4_address, int port) {
  struct sockaddr_in address;
  //build a socket.
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  //name the socket.
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(inet4_address);
  address.sin_port = htons(port);
  int address_length = sizeof(address);
  //connect two sockets.
  if (connect(socket_fd, (struct sockaddr *)&address, address_length) == -1) {
    perror("socket client create");
    return -1;
  }
  return socket_fd;
}
//read and write in socket.
int socketRead(int socket_fd, char *buffer, size_t read_size) {
  return read(socket_fd, buffer, read_size);
}
int sockerWrite(int socket_fd, char *buffer, size_t write_size) {
  return write(socket_fd, buffer, write_size);
}
void socketClientClose(int socket_fd) {
  close(socket_fd);
}

void *fun(void *args) {
  int book_id = *((int *)args);
  long book_need_size = sizeof(char) * fileSize(book_id) / 5;
  char *buffer = (char *)malloc(book_need_size + 5);
  memset(buffer, 0, book_need_size + 5);
  int book_left_size = 0, append_size = 0;
  int i = book_id;
  while (book_left_size = book_need_size
     - (append_size += cutSentences(i, buffer + append_size, book_left_size)))
    i = bookNext(i);
  printf("[%d book done]\n", book_id);
  pthread_mutex_lock(&mtx_socket);
    #ifdef SOCKET_WRITE
    sockerWrite(socket_fd, buffer, book_need_size + 5);
    #else
    fileWrite(book_id, buffer);
    #endif
  pthread_mutex_unlock(&mtx_socket);
  free(buffer);

  pthread_mutex_lock(&mtx_end);
    finished_threads_count++;
    pthread_cond_signal(&condition_var);
  pthread_mutex_unlock(&mtx_end);
  
  pthread_exit(NULL);
}

int main() {
  pthread_mutex_init(&mtx_socket, NULL);
  for (int i = 0; i < BOOK_NUMBER; ++i) pthread_mutex_init(&mtx_file[i], NULL);
  #ifdef SOCKET_WRITE
  socket_fd = socketClientCreate("127.0.0.1", 8080);
  #endif

  int thread_map[BOOK_NUMBER];
  pthread_t tid[BOOK_NUMBER];
  for (int i = 0; i < BOOK_NUMBER; ++i)
  { 
    thread_map[i] = i;
    if (pthread_create(&tid[i], NULL, &fun, &thread_map[i]) != 0) {
      perror("pthread_creat");
      exit(1);
    }
    pthread_join(tid[i], NULL);
  }
  
  pthread_mutex_lock(&mtx_end);
  while (finished_threads_count < BOOK_NUMBER)
    pthread_cond_wait(&condition_var, &mtx_end);
  return 0;
}
