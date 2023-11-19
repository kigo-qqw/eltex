#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef int status_t;
enum { BUFFER_SIZE = 1024 };
enum { BUFFER_2_GET_FROM_STDIO_SIZE = 1024 };
typedef struct user_s {
  char *username;
  char *password;
  int uid;
  int gid;
  char *gecos;
  char *home_directory;
  char *shell;
} user_t;

static char *programm_name;
static char const *in_passwd_filename = "/etc/passwd";
static char const *out_passwd_filename = "/etc/passwd";

struct option options[] = {{"help", no_argument, NULL, 'h'},
                           {"f-in", required_argument, NULL, 'f'},
                           {"f-out", required_argument, NULL, 'o'},
                           {NULL, 0, NULL, 0}};

[[noreturn]] static void usage(status_t status) {
  FILE *usage_out = (status == EXIT_SUCCESS) ? stdout : stderr;

  fprintf(usage_out, "Usage: %s [options] <login> <new login>\n",
          programm_name);
  puts("    --f-in=<IN>    passwd input file (/etc/passwd by default)");
  puts("    --f-out=<OUT>  passwd output file (/etc/passwd by default)");
  exit(status);
}

static void
parse_stdin_input(char *username_buffer, size_t username_buffer_size,
                  char *new_username_buffer, size_t new_username_buffer_size,
                  char const **username, char const **new_username) {

  char *result = fgets(username_buffer, username_buffer_size, stdin);
  if (!result)
    exit(EXIT_FAILURE);
  else
    *username = result;

  result = fgets(new_username_buffer, new_username_buffer_size, stdin);
  if (!result)
    exit(EXIT_FAILURE);
  else
    *new_username = result;

  username_buffer[strcspn(username_buffer, "\n")] = 0;
  new_username_buffer[strcspn(new_username_buffer, "\n")] = 0;
}

static void print(user_t *user) {
  printf("username       : %s\n", user->username);
  printf("password       : %s\n", user->password);
  printf("uid            : %d\n", user->uid);
  printf("gid            : %d\n", user->gid);
  printf("gecos          : %s\n", user->gecos);
  printf("home directory : %s\n", user->home_directory);
  printf("shell          : %s\n", user->shell);
}

static user_t parse_user(char *buffer) {
  user_t user;
  char *it = buffer, *uid_str, *gid_str;

  user.username = strsep(&it, ":");
  user.password = strsep(&it, ":");
  uid_str = strsep(&it, ":");
  gid_str = strsep(&it, ":");
  user.gecos = strsep(&it, ":");
  user.home_directory = strsep(&it, ":");
  user.shell = strsep(&it, ":");
  user.shell[strcspn(user.shell, "\n")] = 0;

  char *uid_str_end, *gid_str_end;
  user.uid = strtol(uid_str, &uid_str_end, 10);
  user.gid = strtol(gid_str, &gid_str_end, 10);

  return user;
}

static int write_user(user_t *user, char *buffer) {
  sprintf(buffer, "%s:%s:%d:%d:%s:%s:%s\n", user->username, user->password,
          user->uid, user->gid, user->gecos, user->home_directory, user->shell);
  return 0;
}

int main(int argc, char **argv) {
  programm_name = argv[0];
  char const *username_arg;
  char const *new_username_arg;

  int c, option_index = 0;
  while ((c = getopt_long(argc, argv, "hfo:", options, &option_index)) != -1) {
    switch (c) {
    case 'u':
    case 'h':
      usage(EXIT_SUCCESS);
      break;

    case 'f':
      in_passwd_filename = optarg;
      break;
    case 'o':
      out_passwd_filename = optarg;
      break;
    }
  }

  int positional_args_amount = argc - optind;
  if (positional_args_amount == 0) {
    char username_buffer[BUFFER_2_GET_FROM_STDIO_SIZE];
    char new_username_buffer[BUFFER_2_GET_FROM_STDIO_SIZE];

    parse_stdin_input(username_buffer, BUFFER_2_GET_FROM_STDIO_SIZE,
                      new_username_buffer, BUFFER_2_GET_FROM_STDIO_SIZE,
                      &username_arg, &new_username_arg);
  } else if (positional_args_amount == 2) {
    username_arg = argv[optind++];
    new_username_arg = argv[optind++];
  } else {
    usage(EXIT_FAILURE);
  }

  FILE *passwd = fopen(in_passwd_filename, "r");

  if (!passwd) {
    fprintf(stderr, "Cannot open %s\n", in_passwd_filename);
    exit(EXIT_FAILURE);
  }

  int i = 0;
  char line_buffer[BUFFER_SIZE][BUFFER_SIZE];

  while (fgets(line_buffer[i], BUFFER_SIZE, passwd)) {
    char buffer[BUFFER_SIZE];
    strncpy(buffer, line_buffer[i], BUFFER_SIZE);

    user_t user = parse_user(buffer);

    if (strcmp(user.username, username_arg) == 0) {
      print(&user);

      user.username = (char *)new_username_arg;
      write_user(&user, line_buffer[i]);
    }
    i++;
  }

  fclose(passwd);

  passwd = fopen(out_passwd_filename, "w");

  for (int it = 0; it < i; ++it) {
    fputs(line_buffer[it], passwd);
  }

  fclose(passwd);

  return EXIT_SUCCESS;
}