/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
// #include <sys/un.h>
#include <unistd.h>

/* Write TEXT to the socket given by file descriptor SOCKET_FD.  */

void write_text (int socket_fd, const char* text)
{
  /* Write the number of bytes in the string, including
     NUL-termination.  */
  int length = strlen (text) + 1;
  write (socket_fd, &length, sizeof (length));
  /* Write the string.  */
  write (socket_fd, text, length);
}

int main (int argc, char* const argv[])
{
  const char* const message = argv[3];
  int socket_fd;
  struct sockaddr_in name;
  struct hostent* hostinfo;
  int port = atoi(argv[2]);

  /* Create the socket.  */
  socket_fd = socket (PF_INET, SOCK_STREAM, 0);

  if (socket_fd < 1) {
    printf("Erro ao criar socket\n");
    return 1;
  }

  /* Store the server's name in the socket address.  */
  name.sin_family = AF_INET;

  hostinfo = gethostbyname (argv[1]);
  if (hostinfo == NULL) {
    printf("Errou! host não encontrado! \n");
    return 1;
  }
  else
    name.sin_addr = *((struct in_addr *) hostinfo->h_addr);
  /* Web servers use port 80.  */
  name.sin_port = htons (port);

  // strcpy (name.sin_path, socket_name);

  /* Connect the socket.  */
  connect (socket_fd, (struct sockaddr *) &name, sizeof (struct sockaddr_in));
  /* Write the text on the command line to the socket.  */
  write_text (socket_fd, message);
  close (socket_fd);
  return 0;
}
