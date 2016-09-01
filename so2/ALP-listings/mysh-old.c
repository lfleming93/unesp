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
char** str_split(char* str);
void keep_runing();
int spawn (char* program, char** arg_list);

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


    if (strstr(cmd, "exit") != 0) break;

    arg_list = str_split(cmd);
    if (strstr(arg_list[0], "cd") != 0) {
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
        spawn (arg_list[0], arg_list);

        wait(NULL);
      }
    }
  }

  return 0;
}

int spawn (char* program, char** arg_list)
{
  pid_t child_pid;

  /* Duplicate this process.  */
  child_pid = fork ();
  if (child_pid != 0) {
    return child_pid;
  } else {
    /* Now execute PROGRAM, searching for it in the path.  */
    execvp (program, arg_list);
    /* The execvp function returns only if an error occurs.  */
    fprintf (stderr, "Error: no such file or directory \n");
    return 0;
  }
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

char** str_split(char* str) {
  char ** res  = NULL;
  // remove o \n no fim da string
  char *  p    = strtok (str, "\n");
  int n_spaces = 0, i;

  p = strtok (str, " ");
  while (p) {
    res = realloc (res, sizeof (char*) * ++n_spaces);

    if (res == NULL)
      exit (-1);

    res[n_spaces-1] = p;
    p = strtok (NULL, " ");
  }

  res = realloc (res, sizeof (char*) * (n_spaces+1));
  res[n_spaces] = 0;

  free (res);

  return res;
}


void keep_runing() {
  signal_sent = 1;
}
