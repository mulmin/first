#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server-ip> <port>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    char message[BUFFER_SIZE];

    // 1. 소켓 생성
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return EXIT_FAILURE;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // 2. 서버 주소 설정
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        return EXIT_FAILURE;
    }

    // 3. 서버에 연결
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        return EXIT_FAILURE;
    }

    printf("Connected to the server %s:%d. Type your message:\n", server_ip, port);

    while (1) {
        printf("You: ");
        fgets(message, BUFFER_SIZE, stdin);
        send(sock, message, strlen(message), 0);

        int bytes_received = read(sock, buffer, BUFFER_SIZE);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("Echo: %s\n", buffer);
        }

        memset(buffer, 0, BUFFER_SIZE); // 버퍼 초기화
        if (strncmp(message, "exit", 4) == 0) { // 'exit' 입력 시 종료
            break;
        }
    }

    // 4. 소켓 종료
    close(sock);
    return 0;
}


