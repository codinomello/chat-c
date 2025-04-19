#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE] = {0};
    
    // Criar socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Erro ao criar socket");
        exit(EXIT_FAILURE);
    }

    // Configurar endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Vincular socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Erro no bind");
        exit(EXIT_FAILURE);
    }

    // Escutar conexões
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Erro no listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor escutando na porta %d...\n", PORT);

    // Aceitar conexões
    int addr_len = sizeof(client_addr);
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr*)&client_addr, (socklen_t*)&addr_len);
        if (new_socket < 0) {
            perror("Erro no accept");
            exit(EXIT_FAILURE);
        }

        printf("Novo cliente conectado!\n");

        // Receber e enviar mensagens
        read(new_socket, buffer, BUFFER_SIZE);
        printf("Mensagem recebida: %s\n", buffer);
        send(new_socket, "Mensagem recebida!", strlen("Mensagem recebida!"), 0);
        
        close(new_socket);
    }

    close(server_fd);
    return 0;
}