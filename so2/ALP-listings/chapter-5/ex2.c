#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

int binary_semaphore_wait (int semid) {
  struct sembuf operations[1];
  /* Use the first (and only) semaphore.  */
  operations[0].sem_num = 0;
  /* Decrement by 1.  */
  operations[0].sem_op = -1;
  /* Permit undo'ing.  */
  operations[0].sem_flg = SEM_UNDO;

  return semop (semid, operations, 1);
}

int binary_semaphore_post (int semid) {
  struct sembuf operations[1];
  /* Use the first (and only) semaphore.  */
  operations[0].sem_num = 0;
  /* Increment by 1.  */
  operations[0].sem_op = 1;
  /* Permit undo'ing.  */
  operations[0].sem_flg = SEM_UNDO;

  return semop (semid, operations, 1);
}

int binary_semaphore_allocation (key_t key, int sem_flags) {
  return semget (key, 1, sem_flags);
}

int binary_semaphore_deallocate (int semid) {
  union semun ignored_argument;
  return semctl (semid, 1, IPC_RMID, ignored_argument);
}

int binary_semaphore_initialize (int semid) {
  union semun argument;
  unsigned short values[1];
  values[0] = 1;
  argument.array = values;
  return semctl (semid, 0, SETALL, argument);
}

int main () {
  int semaphore_id;
  int segment_id;
  char* shared_memory;
  struct shmid_ds shmbuffer;
  int segment_size, i, j;
  pid_t child_pid;

  segment_id = shmget (IPC_PRIVATE, getpagesize(),
           IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  semaphore_id = binary_semaphore_allocation(IPC_PRIVATE, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);

  binary_semaphore_initialize(semaphore_id);

  shared_memory = (char*) shmat (segment_id, 0, 0);

  sprintf (shared_memory, "%d", 100);
  printf("%s\n", shared_memory);

  child_pid = fork ();
  if (child_pid != 0) {
    for (i = 0; i < 1000000; ++i) {
      binary_semaphore_wait(semaphore_id);
      sprintf (shared_memory, "%d", atoi(shared_memory) + 1);
      sprintf (shared_memory, "%d", atoi(shared_memory) - 1);
      printf("%s\n", shared_memory);
      binary_semaphore_post(semaphore_id);
    }

  }
  else {
    for (i = 0; i < 1000000; ++i) {
      binary_semaphore_wait(semaphore_id);
      sprintf (shared_memory, "%d", atoi(shared_memory) + 1);
      sprintf (shared_memory, "%d", atoi(shared_memory) - 1);
      printf("%s\n", shared_memory);
      binary_semaphore_post(semaphore_id);
    }
    return 0;
  }


  printf("\nValor final: %s\n", shared_memory);

  shmdt (shared_memory);

  shmctl (segment_id, IPC_RMID, 0);

  binary_semaphore_deallocate(semaphore_id);

  return 0;
}
