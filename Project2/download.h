#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <termios.h>

#define MAX_LENGTH 128
#define USER_REGEX "%*[^/]//%[^:/]"
#define PASSWORD_REGEX "%*[^/]//%*[^:]:%[^@\n$]"
#define HOST_REGEX "%*[^/]//%*[^@]@%[^/]"
#define URL_REGEX "%*[^/]//%*[^/]/%s"
#define USER_DEFAULT "anonymous"
#define PASS_DEFAULT "anonymous"

typedef enum {
    START,
    SINGLE,
    MULTIPLE,
    END
} State;

struct StateMachine{
    State state;
};

struct URL {
    char user[MAX_LENGTH];
    char password[MAX_LENGTH];
    char host[MAX_LENGTH];
    char path[MAX_LENGTH];
    char ip[MAX_LENGTH];
};

int parse(const char *input, struct URL *url);

char* get_ip_addr(const struct URL* url);

int createSocket(char *ip, int port);

int readResponse(const int socket, char* buffer);

int authenticate(const int socket, const char* user, const char* pass);

int changePath(const int socket, const char* path);

int putPASV(const int socket, char *ip, int *port);

char* getFileName(const char* path);

int requestFile(const int socket, const char* path);

int getResource(const int socketA, const int socketB, char *filename);

int closeConnection(const int socketA, const int socketB);



#endif