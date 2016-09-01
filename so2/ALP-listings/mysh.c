#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/utsname.h>
#include <errno.h>

extern int errno;

char *replace_str(char *str, char *orig, char *rep);
char** str_split(char* str, char* separator);
void keep_runing();
int spawn (char* program, char** arg_list);
int spawn_pipe (int in, int out, char* program, char** arg_list);
int fork_pipes (char *cmd);

int signal_sent = 0;



int main ()
{

  struct passwd *passwd;
  struct sigaction sa;
  char cmd[100],*directory, host[100], *dir = NULL, *root_path, *path, **arg_list, *cd;

  memset (&sa, 0, sizeof (sa));
  sa.sa_handler = &keep_runing;
  sigaction (SIGINT, &sa, NULL);
  sigaction (SIGTSTP, &sa, NULL);


  // Pega o usuário atual
  passwd = getpwuid ( getuid());

  // Pega o Home directory do usuário atual
  root_path = passwd->pw_dir;

  // pega o nome do computador
  gethostname(host, 99);


  while(1) {
    dir = getcwd(dir, 100);
    path = replace_str(dir, root_path, "~");
    printf("[MySh] %s@%s:%s$ ", passwd->pw_name, host, path);


    fgets(cmd, 100, stdin);

    if ( feof(stdin) ) {
      break;
    }


    if (strstr(cmd, "exit") != 0) break;

    if (strstr(cmd, "cd") != 0) {
      arg_list = str_split(cmd, " ");
      if (arg_list[1] == NULL)
        directory = root_path;
      else
        directory = replace_str(arg_list[1],"~", root_path);

      if(chdir(directory) != 0) {
        fprintf (stderr, "Error: no such file or directory \n");
      }

    } else {
      if (signal_sent) {
        printf("\n");
        signal_sent = 0;
      } else {
        fork_pipes(cmd);
      }
    }
  }

  return 0;
}

int spawn (char* program, char** arg_list)
{
  pid_t child_pid;

  child_pid = fork ();
  if (child_pid != 0) {
    return child_pid;
  } else {
    execvp (program, arg_list);

    fprintf (stderr, "Error: no such file or directory \n");
    return 0;
  }
}

int spawn_pipe (int in, int out, char* program, char** args)
{
  pid_t pid;

  pid = fork ();

  if (pid == 0) {

    if (in != 0) {
      dup2 (in, 0);
      close (in);
    }

    if (out != 1) {
      dup2 (out, 1);
      close (out);
    }

    return execvp (program, args);
  }

  return pid;
}

int fork_pipes (char *cmd)
{
  int i = 0, n = 0, child_status = 0;
  pid_t pid;
  int in, fd [2], std_in, std_out;
  char **arg_list, aux[20], **pipe_cmd;

  std_in = dup(0);
  std_out = dup(1);
  in = 0;
  arg_list = str_split(cmd, "|");


  while(arg_list[n] != NULL) {
    n++;
  }

  for (i = 0; i < n - 1; i++) {

    memset(aux, '\0', sizeof(aux));
    strcpy(aux, arg_list[i]);

    pipe_cmd = str_split(aux, " ");

    pipe (fd);

    pid = spawn_pipe (in, fd [1], pipe_cmd[0], pipe_cmd);

    waitpid(pid, &child_status, 0);

    close (fd [1]);
    in = fd [0];

    if (child_status != 0)  {
      close(in);
      break;
    }

  }

  if (child_status == 0) {
    if (in != 0) {
      dup2 (in, 0);
    }

    memset(aux, '\0', sizeof(aux));
    strcpy(aux, arg_list[i]);
    pipe_cmd = str_split(aux, " ");

    spawn (pipe_cmd[0], pipe_cmd);
    wait(NULL);
  } else {
    fprintf (stderr, "Error: no such file or directory \n");
  }

  dup2(std_in, 0);
  dup2(std_out, 1);

  return 0;
}

char *replace_str(char *str, char *orig, char *rep) {
  static char buffer[4096];
  char *p;
  if(!(p = strstr(str, orig)))
    return str;

  strncpy(buffer, str, p-str);
  buffer[p-str] = '\0';

  sprintf(buffer+(p-str), "%s%s", rep, p+strlen(orig));

  return buffer;
}

char** str_split(char* str, char* separator) {
  char ** res  = NULL;
  // remove o \n no fim da string
  char *  p    = strtok (str, "\n");
  int n_spaces = 0, i;

  p = strtok (str, separator);
  while (p) {
    res = realloc (res, sizeof (char*) * ++n_spaces);

    if (res == NULL)
      exit (-1);

    res[n_spaces-1] = p;
    p = strtok (NULL, separator);
  }

  res = realloc (res, sizeof (char*) * (n_spaces+1));
  res[n_spaces] = 0;

  return res;
}

void keep_runing() {
  signal_sent = 1;
}
