#include <stdio.h>
#include <string.h>

typedef struct {
    char* REQUEST_METHOD;
    char* QUERY_STRING;
    char* SCRIPT_NAME;
    char* DOCUMENT_ROOT;
} Headers;

typedef struct {
    char* url;
    char* file_path;
} TableUrl;

TableUrl search_page(char* cur_path, TableUrl* table, int count) {
    TableUrl result = {.url = "404", .file_path = "/pages/404.html"};

    for (int i = 0; i < count; i++) {
        if (strcmp(table[i].url, cur_path) == 0) {
            result = table[i];
            break;
        }
    }

    return result;
}

int route(Headers env, TableUrl* table, char* cur_path) {
    FILE* fptr;
    int c;

    TableUrl page = search_page(cur_path, table, 3);

    char file_path[512];
    snprintf(file_path, sizeof(file_path), "%s%s", env.DOCUMENT_ROOT, page.file_path);

    fptr = fopen(file_path, "r");
    if (fptr == NULL) {
        printf("HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n");
        perror("Файл не найден");
        return 1;
    }

    if (strcmp(page.url, "404") == 0) {
        printf("HTTP/1.1 404 Not Found\r\n");
        printf("Content-Type: text/html\r\n");
        printf("Server: ponyx-server\r\n\r\n");

    } else {
        printf("HTTP/1.1 200 OK\r\n");
        printf("Content-Type: text/html\r\n");
        printf("Server: ponyx-server\r\n\r\n");
    }

    fflush(stdout);

    while ((c = fgetc(fptr)) != EOF) {
        putchar(c);
    }

    fclose(fptr);

    return 0;
}

int main(int argc, char* argv[], char* envp[]) {
    Headers env = {NULL, NULL, NULL, NULL};

    TableUrl table[3];
    table[0].url = "/";
    table[0].file_path = "/pages/index.html";

    table[1].url = "/contacts";
    table[1].file_path = "/pages/contacts.html";

    table[2].url = "404";
    table[2].file_path = "/pages/404.html";

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

    if (env.REQUEST_METHOD && strcmp(env.REQUEST_METHOD, "GET") == 0) {
        route(env, table, env.SCRIPT_NAME ? env.SCRIPT_NAME : "/");
    }

    return 0;
}
