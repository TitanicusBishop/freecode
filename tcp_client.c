// skynet_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COMMAND_LEN 256
#define MAX_ARGS 10
#define MAX_ARG_LEN 50
#define BUFFER_SIZE 1024

// ---------- Cross-platform includes & macros ----------
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <direct.h>   // _mkdir, _chdir
    #pragma comment(lib, "ws2_32.lib")
    typedef SOCKET sock_t;
    #define MKDIR(path)     _mkdir(path)
    #define CHDIR(path)     _chdir(path)
    #define CLOSESOCK(s)    closesocket(s)
    #define SOCK_ERR        SOCKET_ERROR
    #define SOCK_INV        INVALID_SOCKET
#else
    #include <dirent.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <sys/stat.h>   // mkdir
    typedef int sock_t;
    #define MKDIR(path)     mkdir(path, 0777)
    #define CHDIR(path)     chdir(path)
    #define CLOSESOCK(s)    close(s)
    #define SOCK_ERR        (-1)
    #define SOCK_INV        (-1)
#endif

typedef enum { MODE_LOCAL, MODE_REMOTE } Mode;
static Mode currentMode = MODE_LOCAL;
static sock_t remoteSock = SOCK_INV;

// ---------- Small utilities ----------
static void displayPrompt(void) {
    printf("\nSkynet(%s) > ", currentMode == MODE_LOCAL ? "Local" : "Remote");
}

// Read “all available” data from the socket until a short timeout.
// This avoids truncation when server sends multiple chunks (e.g., ls/view).
static int recv_all_with_timeout(sock_t s, char **out, int timeout_ms) {
    // We'll grow this buffer as needed
    size_t cap = 4096, len = 0;
    char *buf = (char*)malloc(cap);
    if (!buf) return -1;

    // Set up select() timeout loop
    for (;;) {
        // First, if nothing yet read, we still attempt at least one recv with select
        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(s, &rfds);

        struct timeval tv;
        tv.tv_sec  = timeout_ms / 1000;
        tv.tv_usec = (timeout_ms % 1000) * 1000;

        int ready = select((int)(s + 1), &rfds, NULL, NULL, &tv);
        if (ready <= 0) break; // timeout or error → stop reading

        // Socket ready to read
        char chunk[BUFFER_SIZE];
        int n = recv(s, chunk, sizeof(chunk), 0);
        if (n <= 0) break; // connection closed or error → stop

        // Ensure capacity
        if (len + (size_t)n + 1 > cap) {
            size_t newcap = cap * 2;
            while (len + (size_t)n + 1 > newcap) newcap *= 2;
            char *nb = (char*)realloc(buf, newcap);
            if (!nb) { free(buf); return -1; }
            buf = nb; cap = newcap;
        }

        memcpy(buf + len, chunk, (size_t)n);
        len += (size_t)n;

        // If we got less than a full chunk, the server may be done (heuristic)
        if (n < (int)sizeof(chunk)) {
            // do one more quick poll in case a tiny trailer arrives
            FD_ZERO(&rfds);
            FD_SET(s, &rfds);
            tv.tv_sec = 0; tv.tv_usec = 150 * 1000; // 150ms
            ready = select((int)(s + 1), &rfds, NULL, NULL, &tv);
            if (ready <= 0) break;
        }
    }

    buf[len] = '\0';
    *out = buf;
    return (int)len;
}

static void sendToRemote(const char *fullCmd) {
    if (remoteSock == SOCK_INV) {
        printf("No remote connection.\n");
        return;
    }

    // Send the command exactly as typed
    if (send(remoteSock, fullCmd, (int)strlen(fullCmd), 0) == SOCK_ERR) {
        printf("Send failed. Disconnecting.\n");
        CLOSESOCK(remoteSock);
        remoteSock = SOCK_INV;
        currentMode = MODE_LOCAL;
        return;
    }

    // Read back all available data for this command
    char *response = NULL;
    int got = recv_all_with_timeout(remoteSock, &response, 400); // ~400ms idle window
    if (got <= 0) {
        printf("Lost connection to server.\n");
        if (response) free(response);
        CLOSESOCK(remoteSock);
        remoteSock = SOCK_INV;
        currentMode = MODE_LOCAL;
        return;
    }
    printf("%s", response);
    free(response);
}

// ---------- LOCAL COMMANDS ----------
static void executeLocalCommand(char *command, char *args[]) {
    if (strcmp(command, "help") == 0) {
        printf("Commands:\n");
        printf("  help, exit\n");
        printf("  mode local\n");
        printf("  mode remote <ip> <port>\n");
        printf("  ls, cd <dir>, mkdir <dir>, create <file>, delete <file>, view <file>\n");
        // Local-only stubs you might add later:
        // printf("  put <file>, append <file>\n");
        return;
    }
    if (strcmp(command, "exit") == 0) {
        if (remoteSock != SOCK_INV) CLOSESOCK(remoteSock);
#ifdef _WIN32
        WSACleanup();
#endif
        exit(0);
    }
    if (strcmp(command, "ls") == 0) {
#ifdef _WIN32
        // Minimal Windows fallback without dirent (if compiling with MSVC)
        system("dir");
#else
        DIR *dir = opendir(".");
        if (!dir) { perror("ls"); return; }
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) puts(entry->d_name);
        closedir(dir);
#endif
        return;
    }
    if (strcmp(command, "cd") == 0) {
        if (!args[0]) { printf("Usage: cd <dir>\n"); return; }
        if (CHDIR(args[0]) != 0) perror("cd");
        return;
    }
    if (strcmp(command, "mkdir") == 0) {
        if (!args[0]) { printf("Usage: mkdir <dir>\n"); return; }
        if (MKDIR(args[0]) != 0) perror("mkdir");
        else puts("Directory created.");
        return;
    }
    if (strcmp(command, "create") == 0) {
        if (!args[0]) { printf("Usage: create <file>\n"); return; }
        FILE *fp = fopen(args[0], "w");
        if (!fp) perror("create");
        else { fclose(fp); puts("File created."); }
        return;
    }
    if (strcmp(command, "delete") == 0) {
        if (!args[0]) { printf("Usage: delete <file>\n"); return; }
        if (remove(args[0]) == 0) puts("File deleted.");
        else perror("delete");
        return;
    }
    if (strcmp(command, "view") == 0) {
        if (!args[0]) { printf("Usage: view <file>\n"); return; }
        FILE *fp = fopen(args[0], "r");
        if (!fp) { perror("view"); return; }
        char buf[BUFFER_SIZE];
        while (fgets(buf, sizeof(buf), fp)) fputs(buf, stdout);
        fclose(fp);
        return;
    }

    printf("Unknown local command. Type 'help'.\n");
}

// ---------- ROUTER ----------
static void executeCommand(char *command, char *args[], char *fullCmd) {
    if (strcmp(command, "mode") == 0) {
        if (!args[0]) {
            printf("Current mode: %s\n", currentMode == MODE_LOCAL ? "local" : "remote");
            return;
        }
        if (strcmp(args[0], "local") == 0) {
            if (remoteSock != SOCK_INV) { CLOSESOCK(remoteSock); remoteSock = SOCK_INV; }
            currentMode = MODE_LOCAL;
            puts("Switched to local mode.");
            return;
        }
        if (strcmp(args[0], "remote") == 0) {
            if (!args[1] || !args[2]) { printf("Usage: mode remote <ip> <port>\n"); return; }

            // Create socket
            sock_t sockfd =
#ifdef _WIN32
                socket(AF_INET, SOCK_STREAM, 0);
#else
                socket(AF_INET, SOCK_STREAM, 0);
#endif
            if (sockfd == SOCK_INV) {
                printf("socket() failed\n");
                return;
            }

            struct sockaddr_in server;
            memset(&server, 0, sizeof(server));
            server.sin_family = AF_INET;
            server.sin_port = htons((unsigned short)atoi(args[2]));
            if (inet_pton(AF_INET, args[1], &server.sin_addr) != 1) {
                printf("Invalid IP address.\n");
                CLOSESOCK(sockfd);
                return;
            }

            if (connect(sockfd, (struct sockaddr*)&server, sizeof(server)) == SOCK_ERR) {
#ifdef _WIN32
                printf("connect() failed. WSA error: %d\n", WSAGetLastError());
#else
                perror("connect");
#endif
                CLOSESOCK(sockfd);
                return;
            }

            remoteSock = sockfd;
            currentMode = MODE_REMOTE;
            printf("Connected to %s:%s\n", args[1], args[2]);
            return;
        }

        printf("Usage: mode [local | remote <ip> <port>]\n");
        return;
    }

    if (currentMode == MODE_LOCAL)
        executeLocalCommand(command, args);
    else
        sendToRemote(fullCmd);
}

// ---------- PARSER ----------
static int parseCommand(char *input, char *command, char *args[], char *fullCmd) {
    strcpy(fullCmd, input);

    // Strip trailing newlines
    size_t L = strlen(input);
    while (L && (input[L-1] == '\n' || input[L-1] == '\r')) input[--L] = '\0';

    char *token = strtok(input, " \t");
    if (!token) return 0;
    strncpy(command, token, MAX_ARG_LEN-1); command[MAX_ARG_LEN-1] = '\0';

    int count = 0;
    while ((token = strtok(NULL, " \t")) != NULL && count < MAX_ARGS-1) {
        args[count++] = token;
    }
    args[count] = NULL;
    return count;
}

int main(void) {
    printf("-- Skynet Client Shell --\n");

#ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        printf("Failed to initialize Winsock. Error: %d\n", WSAGetLastError());
        return 1;
    }
#endif

    char input[MAX_COMMAND_LEN], command[MAX_ARG_LEN], *args[MAX_ARGS], fullCmd[MAX_COMMAND_LEN];

    for (;;) {
        displayPrompt();
        if (!fgets(input, sizeof(input), stdin)) break;
        parseCommand(input, command, args, fullCmd);
        executeCommand(command, args, fullCmd);
    }

    if (remoteSock != SOCK_INV) CLOSESOCK(remoteSock);
#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}