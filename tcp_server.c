// skynet_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
    #define CLOSESOCK closesocket
    typedef int socklen_t;
#else
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #define CLOSESOCK close
#endif

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int sock) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes = recv(sock, buffer, sizeof(buffer)-1, 0);
        if (bytes <= 0) break;
        buffer[bytes] = '\0';

        char cmd[BUFFER_SIZE] = {0};
        char arg[BUFFER_SIZE] = {0};
        sscanf(buffer, "%s %s", cmd, arg);

        if (strcmp(cmd, "ls") == 0) {
            DIR *d = opendir(".");
            if (!d) {
                send(sock, "Error opening directory.\n", 25, 0);
                continue;
            }
            struct dirent *entry;
            char out[BUFFER_SIZE];
            while ((entry = readdir(d)) != NULL) {
                snprintf(out, sizeof(out), "%s\n", entry->d_name);
                send(sock, out, strlen(out), 0);
            }
            closedir(d);
        }
        else if (strcmp(cmd, "create") == 0 && strlen(arg) > 0) {
            FILE *fp = fopen(arg, "w");
            if (fp) { fclose(fp); send(sock, "File created.\n", 14, 0); }
            else send(sock, "Error creating file.\n", 22, 0);
        }
        else if (strcmp(cmd, "delete") == 0 && strlen(arg) > 0) {
            if (remove(arg) == 0) send(sock, "File deleted.\n", 14, 0);
            else send(sock, "Error deleting file.\n", 22, 0);
        }
        else if (strcmp(cmd, "view") == 0 && strlen(arg) > 0) {
            FILE *fp = fopen(arg, "r");
            if (!fp) {
                send(sock, "File not found.\n", 16, 0);
            } else {
                char line[BUFFER_SIZE];
                while (fgets(line, sizeof(line), fp)) {
                    send(sock, line, strlen(line), 0);
                }
                fclose(fp);
            }
        }
        else if (strcmp(cmd, "exit") == 0) {
            break;
        }
        else {
            send(sock, "Unknown or invalid command.\n", 28, 0);
        }
    }
    CLOSESOCK(sock);
}

int main() {
#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock.\n");
        return 1;
    }
#endif

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        return 1;
    }

    printf("Skynet server listening on port %d...\n", PORT);

    while (1) {
        int client = accept(server_fd, NULL, NULL);
        if (client >= 0) {
            printf("Client connected!\n");
            handle_client(client);
            printf("Client disconnected.\n");
        }
    }

    CLOSESOCK(server_fd);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}