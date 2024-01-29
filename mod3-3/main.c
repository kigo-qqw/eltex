#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

static void __attribute__((__noreturn__)) usage(const char *program_name) {
  fputs("Delete shared memory for filepath and proj_id\n\n", stdout);
  fprintf(stdout, "Usage: %s <pathname> <proj_id>\n", program_name);
  exit(EXIT_SUCCESS);
}

int main(int argc, char **argv) {
  if (argc == 3) {
    const char *filepath = argv[1];
    const char *proj_id_as_str = argv[2];

    int proj_id = (int)strtoll(proj_id_as_str, NULL, 10);

    key_t key = ftok(filepath, proj_id);
    if (key < 0) {
      perror("Can't generate key");
      exit(EXIT_FAILURE);
    }

    int shmid = shmget(key, 3 * sizeof(int), 0);
    if (shmid < 0) {
      perror("Can't find shared memory");
      exit(EXIT_FAILURE);
    }

    int err = shmctl(shmid, IPC_RMID, NULL);
    if (err < 0) {
      perror("Can't delete shared memory");
      exit(EXIT_FAILURE);
    }
  } else {
    usage(argv[0]);
  }
  return 0;
}
