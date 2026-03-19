#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define LISTEN_BACKLOG 10
#define SOCKET_OPT_VAL 1

int main()
{
  struct sockaddr_in my_addr, client_addr;

  int dock, my_bind, my_listen, my_accept, my_sockopt;
  int socketoptval = SOCKET_OPT_VAL;
  char buffer[1024];
  char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";

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

    memset(&buffer, 0, sizeof(buffer));

    my_accept = accept(dock, (struct sockaddr *)&client_addr, &client_len);
    if (my_accept == -1)
    {
      perror("Accept is failing");
      return 1;
    }

    read_result = read(my_accept, buffer, sizeof(buffer) - 1);

    my_write = write(my_accept, response, strlen(response));
    close(my_accept);

    printf("Heres the program result:\n\n");
    printf("dock: %d\n", dock);
    printf("sockopt: %d\n", my_sockopt);
    printf("bind: %d\n", my_bind);
    printf("listen: %d\n", my_listen);
    printf("accept: %d\n", my_accept);
    printf("read: %ld\n", read_result);
    printf("buffer: %s\n", buffer);
    printf("write: %ld\n", my_write);
  }

  return 0;
}
