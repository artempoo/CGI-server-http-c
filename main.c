#include <stdio.h>
#include <string.h>

typedef struct {
    char* REQUEST_METHOD;
    char* QUERY_STRING;
    char* SCRIPT_NAME;
    char* DOCUMENT_ROOT;
} Headers;

int main(int argc, char* argv[], char* envp[]) {
    Headers env = {"", "", "", ""};
    const char* REQUEST_METHOD_NAME = "REQUEST_METHOD=";
    const char* QUERY_STRING_NAME = "QUERY_STRING=";
    const char* SCRIPT_NAME = "SCRIPT_NAME=";
    const char* DOCUMENT_ROOT = "DOCUMENT_ROOT=";

    int index_envp = 0;
    while (envp[index_envp] != NULL) {
        if (strncmp(REQUEST_METHOD_NAME, envp[index_envp], strlen(REQUEST_METHOD_NAME)) == 0) {
            env.REQUEST_METHOD = strchr(envp[index_envp], '=') + 1;
        } else if (strncmp(QUERY_STRING_NAME, envp[index_envp], strlen(QUERY_STRING_NAME)) == 0) {
            env.QUERY_STRING = strchr(envp[index_envp], '=') + 1;
        } else if (strncmp(SCRIPT_NAME, envp[index_envp], strlen(SCRIPT_NAME)) == 0) {
            env.SCRIPT_NAME = strchr(envp[index_envp], '=') + 1;
        } else if (strncmp(DOCUMENT_ROOT, envp[index_envp], strlen(DOCUMENT_ROOT)) == 0) {
            env.DOCUMENT_ROOT = strchr(envp[index_envp], '=') + 1;
        }
        index_envp++;
    }

    if (strcmp(env.REQUEST_METHOD, "GET") == 0) {
        if (strcmp(env.SCRIPT_NAME, "/") == 0) {
            FILE* fptr;
            int c;
            char* file_path = strcat(env.DOCUMENT_ROOT, "/pages/index.html");

            fptr = fopen(file_path, "r");
            if (fptr == NULL) {
                perror("Файл не найден");
                return 1;
            }

            printf("HTTP/1.1 200 OK\r\n");
            printf("Content-Type: text/html\r\n");
            printf("Server: ponyx-server\r\n\r\n");

            fflush(stdout);

            while ((c = fgetc(fptr)) != EOF) {
                putchar(c);
            }

            fclose(fptr);
        } else {
            FILE* fptr;
            int c;
            char* file_path = strcat(env.DOCUMENT_ROOT, "/pages/404.html");

            fptr = fopen(file_path, "r");

            if (fptr == NULL) {
                perror("Файл не найден");
                return 1;
            }

            printf("HTTP/1.1 404 Not Found\r\n");
            printf("Content-Type: text/html\r\n");
            printf("Server: ponyx-server\r\n\r\n");

            fflush(stdout);

            while ((c = fgetc(fptr)) != EOF) {
                putchar(c);
            }

            fclose(fptr);
        }
    }

    return 0;
}
