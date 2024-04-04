#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "common.h"

enum { REPL_NUMBER = 3 };

NORETURN static void usage(const c8 *const program_name, i32 status) {
  FILE *const out = status ? stdout : stderr;
  fprintf(out, "Usage: %s <IP address>", program_name);
  exit(status);
}

i32 main(i32 argc, c8 **argv) {
  if (argc != 2) {
    usage(argv[0], EXIT_FAILURE);
  }
  i32 socket_fd = socket(PF_INET, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    perror(NULL);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  if (inet_aton(argv[1], &server_address.sin_addr) == 0) {
    printf("Invalid IP address\n"); /* TODO: */
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  i32 connect_status = connect(socket_fd, (struct sockaddr *)&server_address,
                               sizeof(server_address));
  if (connect_status < 0) {
    perror(NULL);
    close(socket_fd);
    exit(EXIT_FAILURE);
  }

  c8 input_buffer[BUFFER_SIZE];
  c8 buffer[BUFFER_SIZE];
  for (i32 i = 0; i < REPL_NUMBER; i++) {
    printf("> ");
    fflush(stdin);
    fgets(input_buffer, BUFFER_SIZE, stdin);

    ssize written = write(socket_fd, input_buffer, strlen(input_buffer) + 1);
    if (written < 0) {
      perror("Can't write\n");
      close(socket_fd);
      exit(EXIT_FAILURE);
    }

    ssize number_read = read(socket_fd, buffer, BUFFER_SIZE - 1);
    if (number_read < 0) {
      perror("Can't read\n");
      close(socket_fd);
      exit(EXIT_FAILURE);
    }

    printf("%s", buffer);
  }
  close(socket_fd);
}
