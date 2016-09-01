/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
// #include <sys/un.h>
#include <unistd.h>


/* Read text from the socket and print it out.  Continue until the
   socket closes.  Return non-zero if the client sent a "quit"
   message, zero otherwise.  */

int server (int client_socket)
{
  while (1) {
    int length;
    char* text;

    /* First, read the length of the text message from the socket.  If
       read returns zero, the client closed the connection.  */
    if (read (client_socket, &length, sizeof (length)) == 0)
      return 0;
    /* Allocate a buffer to hold the text.  */
    text = (char*) malloc (length);
    /* Read the text itself, and print it.  */
    read (client_socket, text, length);
    printf ("%s\n", text);

    /* Free the buffer.  */
    /* If the client sent the message "quit", we're all done.  */
    if (!strcmp (text, "quit")) {
      free (text);
      return 1;
    } else {
      free (text);
    }
  }
}

int main (int argc, char* const argv[])
{
  int socket_fd;
  struct sockaddr_in name;
  int client_sent_quit_message;
  struct hostent* hostinfo;
  int port = atoi(argv[1]);

  /* Create the socket.  */
  socket_fd = socket (PF_INET, SOCK_STREAM, 0);
  /* Indicate this is a server.  */
  name.sin_family = AF_INET;

  name.sin_addr.s_addr = htonl(INADDR_ANY);

  name.sin_port = htons (port);


  bind (socket_fd, (struct sockaddr *) &name, sizeof (struct sockaddr_in));
  /* Listen for connections.  */
  listen (socket_fd, 5);

  /* Repeatedly accept connections, spinning off one server() to deal
     with each client.  Continue until a client sends a "quit" message.  */
  do {
    struct sockaddr_in client_name;
    socklen_t client_name_len;
    int client_socket_fd;
    client_name_len = sizeof(client_name);
    /* Accept a connection.  */
    client_socket_fd = accept (socket_fd, (struct sockaddr *) &client_name, &client_name_len);
    /* Handle the connection.  */
    client_sent_quit_message = server (client_socket_fd);
    /* Close our end of the connection.  */
    close (client_socket_fd);
  }
  while (!client_sent_quit_message);

  /* Remove the socket file.  */
  close (socket_fd);

  return 0;
}
