#include <signal.h>
#include <stdio.h>

static void my_handler(int nsig){ 
  printf("Receive signal %d\n", nsig);
}

int main(void){ 
  for (int i = 0; i < SIGRTMAX; ++i) {
    (void)signal(i, my_handler);
  }
  
  while(1);

  return 0;
}
