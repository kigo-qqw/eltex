#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>


static void read_from_fd_n_write_to_stdout(int fd) {
    char buf[BUFSIZ];
    ssize_t n;
    while ((n=read(fd, buf, BUFSIZ))>0) {
      write(STDOUT_FILENO,buf,n);
    }
}

int main(int argc, char**argv) {
  if (argc == 1) {
    read_from_fd_n_write_to_stdout(STDIN_FILENO);  
  } else {
    for (int i = 1; i < argc; ++i) {
      int fd = open(argv[i], O_RDONLY, 0);       
      read_from_fd_n_write_to_stdout(fd);
      close(fd);
    }
  }
  return 0;
}
