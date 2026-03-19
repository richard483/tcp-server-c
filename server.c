#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define LISTEN_BACKLOG 10

int main()
{
  int dock, my_bind, my_listen, my_accept;
  char buffer[1024];
  char response[] = "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
  struct sockaddr_in my_addr, client_addr;
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

    printf("Heres the program result:\ndock: %d\nbind: %d\nlisten: %d\naccept: %d\nread: %ld\nbuffer: %s\nwrite: %ld\n", dock, my_bind, my_listen, my_accept, read_result, buffer, my_write);
  }

  return 0;
}
