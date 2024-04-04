#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"

enum { THREAD_COUNT = 2 };

static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *thread_runner(void *data) {
  i32 socket_fd = *(i32 *)data;

  i32 new_socket_fd;
  struct sockaddr_in client_address;
  socklen_t client_addr_len = sizeof(client_address);

  c8 buffer[BUFFER_SIZE];

  for (;;) {
    pthread_mutex_lock(&lock);
    if ((new_socket_fd = accept(socket_fd, (struct sockaddr *)&client_address,
                                &client_addr_len)) < 0) {
      perror(NULL);
      close(socket_fd);
      exit(1);
    }
    pthread_mutex_unlock(&lock);

    ssize n;
    while ((n = read(new_socket_fd, buffer, BUFFER_SIZE - 1)) > 0) {
      if ((n = write(new_socket_fd, buffer, strlen(buffer) + 1)) < 0) {
        perror(NULL);
        close(socket_fd);
        close(new_socket_fd);
        exit(1);
      }
    }
    if (n < 0) {
      perror(NULL);
      close(socket_fd);
      close(new_socket_fd);
      exit(1);
    }
  }
}

i32 main(i32 argc, const c8 *const *const argv) {
  (void)argc;
  (void)argv;

  i32 socket_fd;
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror(NULL);
    exit(1);
  }

  struct sockaddr_in server_addr;

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror(NULL);
    close(socket_fd);
    exit(1);
  }

  if (listen(socket_fd, 5) < 0) {
    perror(NULL);
    close(socket_fd);
    exit(1);
  }

  pthread_t *threads = malloc(sizeof(pthread_t) * THREAD_COUNT);
  for (i32 i = 0; i < THREAD_COUNT; ++i) {
    pthread_create(&threads[i], NULLPTR, thread_runner, &socket_fd);
  }
  for (i32 i = 0; i < THREAD_COUNT; ++i) {
    void *ignore;
    pthread_join(threads[i], &ignore);
  }
  free(threads);

  exit(EXIT_SUCCESS);
}
