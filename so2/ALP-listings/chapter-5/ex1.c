#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

int main () {
  int segment_id;
  char* shared_memory;
  struct shmid_ds shmbuffer;
  int segment_size, i, j;
  pid_t child_pid;

  segment_id = shmget (IPC_PRIVATE, getpagesize(),
           IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  shared_memory = (char*) shmat (segment_id, 0, 0);

  sprintf (shared_memory, "%d", 100);
  printf("%s\n", shared_memory);

  child_pid = fork ();
  if (child_pid != 0) {
    for (i = 0; i < 1000000; ++i) {
      sprintf (shared_memory, "%d", atoi(shared_memory) + 1);
      sprintf (shared_memory, "%d", atoi(shared_memory) - 1);
      printf("%s\n", shared_memory);
    }

  }
  else {
    for (i = 0; i < 1000000; ++i) {
      sprintf (shared_memory, "%d", atoi(shared_memory) + 1);
      sprintf (shared_memory, "%d", atoi(shared_memory) - 1);
      printf("%s\n", shared_memory);
    }
    return 0;
  }


  printf("\nValor final: %s\n", shared_memory);

  shmdt (shared_memory);

  shmctl (segment_id, IPC_RMID, 0);

  return 0;
}
