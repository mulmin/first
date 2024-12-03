#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int bytes_read;

    // 1. 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. 소켓 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // 3. 소켓 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 4. 소켓 리스닝
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Echo Server is running on port %d\n", port);

    // 5. 클라이언트 연결 대기
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected.\n");

    // 6. 클라이언트와 데이터 교환
    while ((bytes_read = read(new_socket, buffer, BUFFER_SIZE)) > 0) {
        printf("Received: %s\n", buffer);
        send(new_socket, buffer, bytes_read, 0); // 받은 데이터를 그대로 전송
        memset(buffer, 0, BUFFER_SIZE); // 버퍼 초기화
    }

    if (bytes_read == 0) {
        printf("Client disconnected.\n");
    } else {
        perror("Read error");
    }

    // 7. 소켓 종료
    close(new_socket);
    close(server_fd);
    return 0;
}

