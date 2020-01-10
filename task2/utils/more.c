#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <termio.h>

int global_fd_in;

int x_size, y_size;
long long file_len = 0;

//fileIO & display.
char readOneCharacter(int fd);
void writeOneCharacter(int fd, char ch);
void printOneScreen(int fd, int line);

//terminal changed singal handle.  
void updateTerminalSize();
void updateScreen(int signal);

//seek as operation of 'B', 'F', ' ', '\n'.
double contentStatus();
void backScreen(int fd);
void backSeek(int fd, int line);
void moveBack(int fd, int line);
void moveForward(int fd, int line);

int getch(void);

int main(int argc, char *argv[]) {
  if (argc > 2) {
		printf("\n\e[1merror: more: usage is false, right usage is more [FILE].\e[0m\n");
    exit(1);
	}
  int fd;
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
    printf("\n\e[1merror: more: open error: %s!\e[0m\n", strerror(errno));
  }
  global_fd_in = fd;
  lseek(fd, 0, SEEK_SET);
  file_len = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);

  updateTerminalSize();
  printOneScreen(fd, y_size - 1);
  printf("--MORE--");
  signal(SIGWINCH, updateScreen);
  while (1) {
    switch (getch()) {
      case 'b':
        printf("\n");
        moveBack(fd, y_size - 1);
        printf("--BACK--(%.2lf%%)", contentStatus()*100);
        break;
      case 'f':
        printf("\n");
        moveForward(fd, y_size - 1);
        printf("--FORWARD--(%.2lf%%)", contentStatus()*100);
        break;
      case '\n':
        printf("\n");
        backSeek(fd, y_size - 2);
        moveForward(fd, y_size - 1);
        printf("--NEXT LINE--(%.2lf%%)", contentStatus()*100);
        break;
      case ' ':
        printf("\n");
        backSeek(fd, y_size - 2);
        moveForward(fd, y_size - 1);
        printf("--NEXT LINE--(%.2lf%%)", contentStatus()*100);
        break;
      case 'q':
        exit(0);
      default:
        break;
    }
  }
	return 0;
}

void printOneScreen(int fd, int line) {
  char t;
  while (line) {
    if ((t = readOneCharacter(fd)) == '\n') line--;
    writeOneCharacter(STDOUT_FILENO, t);
  }
}
char readOneCharacter(int fd) {
  char buf[2];
  ssize_t size;
	if ((size = read(fd, buf, 1)) < 0) { //illegal read operations existed.
		printf("\n\e[1merror: more: read error: %s!\e[0m\n", strerror(errno));
  	exit(1);
  }
  return buf[0];
}
void writeOneCharacter(int fd, char ch) {
  char buf[2];
  buf[0] = ch;
  if (write(fd, buf, 1) != 1) {
    printf("\n\e[1merror: more: write error: %s!\e[0m\n", strerror(errno));
    exit(1);
  }
}

void updateScreen(int signal) {
  backScreen(global_fd_in);
  updateTerminalSize();
  printf("\n");
  printOneScreen(global_fd_in, y_size - 1);
}
void updateTerminalSize() {
  struct winsize size;
  ioctl(STDIN_FILENO, TIOCGWINSZ, &size);
  x_size = size.ws_col;
  y_size = size.ws_row;
}

double contentStatus() {
  return (double)(lseek(global_fd_in, 0, SEEK_CUR))/(double)(file_len);
}
void backSeek(int fd, int line) {
  while (line) {
    if (lseek(fd, -2, SEEK_CUR) == -1) {
      lseek(fd, 0, SEEK_SET);
      break;
    }
    if (readOneCharacter(fd) == '\n') line--;
  }
}
void backScreen(int fd) {
  backSeek(fd, (y_size - 1) * 2 + 1);
}
void moveBack(int fd, int line) {
  backSeek(fd, line * 2);
  printOneScreen(fd, line);
}
void moveForward(int fd, int line) {
  printOneScreen(fd, line);
}

/* reads from keypress, doesn't echo */
int getch(void) {
  struct termios oldattr, newattr;
  int ch;
  tcgetattr( STDIN_FILENO, &oldattr );
  newattr = oldattr;
  newattr.c_lflag &= ~( ICANON | ECHO );
  tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
  ch = getchar();
  tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
  return ch;
}