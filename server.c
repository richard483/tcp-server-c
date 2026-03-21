#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define LISTEN_BACKLOG 10
#define SOCKET_OPT_VAL 1

int main()
{
  printf("Program is starting!\n");
  struct sockaddr_in my_addr, client_addr;

  int dock, my_bind, my_listen, my_accept, my_sockopt;
  int socketoptval = SOCKET_OPT_VAL;

  socklen_t client_len;
  ssize_t read_result, my_write;

  memset(&my_addr, 0, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(8080);
  my_addr.sin_addr.s_addr = INADDR_ANY;

  dock = socket(AF_INET, SOCK_STREAM, 0);
  if (dock == -1)
  {
    perror("Socket is failing\n");
    return 1;
  }

  my_sockopt = setsockopt(dock, SOL_SOCKET, SO_REUSEADDR, &socketoptval, sizeof(socketoptval));
  if (my_sockopt == -1)
  {
    perror("Sockopt is failing");
    return 1;
  }

  my_bind = bind(dock, (struct sockaddr *)&my_addr, sizeof(my_addr));
  if (my_bind == -1)
  {
    perror("Bind is failing");
    return 1;
  }

  my_listen = listen(dock, LISTEN_BACKLOG);
  if (my_listen == -1)
  {
    perror("Listen is failing");
    return 1;
  }

  while (1)
  {
    memset(&client_addr, 0, sizeof(client_addr));
    client_len = sizeof(client_addr);

    my_accept = accept(dock, (struct sockaddr *)&client_addr, &client_len);
    if (my_accept == -1)
    {
      perror("Accept is failing");
      return 1;
    }

    pid_t pid = fork();

    if (pid == -1)
    {
      printf("Fork is failing\n");
      return 1;
    }
    else if (pid == 0)
    {
      char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
      char buffer[1024];
      memset(&buffer, 0, sizeof(buffer));

      close(dock);

      read_result = read(my_accept, buffer, sizeof(buffer) - 1);
      if (read_result == -1)
      {
        printf("Read is failing\n");
        return 1;
      }

      my_write = write(my_accept, response, strlen(response));
      if (my_write == -1)
      {
        printf("Write is failing\n");
        return 1;
      }

      printf("Done child process\n");
      close(my_accept);
      return 0;
    }

    close(my_accept);
    waitpid(-1, NULL, WNOHANG);
  }

  return 0;
}
