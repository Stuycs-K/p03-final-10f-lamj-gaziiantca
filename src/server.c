#include "networking.h"

int main(int argc, char *argv[] ) {int server_fd = setupUDP();
  char buffer[1024];
  struct sockaddr_storage client_addr;
  socklen_t addr_len = sizeof(client_addr);

  printf("UDP Server waiting for packets...\n");

  while (1) {
    int bytes_received = recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *) &client_addr, &addr_len);

    if (bytes_received < 0) {
      perror("recvfrom failed");
      continue;
    }

    buffer[bytes_received] = '\0';
    printf("Received message: %s\n", buffer);

    sendto(server_fd, "Receipt", 7, 0, (struct sockaddr *)&client_addr, addr_len);
  }

  return 0;
}
