#include <regex.h>
#include <stdio.h>

struct URLData {
    char user[128];
    char password[128];
    char host[128];
    char url_path[128];
};

int validate_ftp_url(const char *url);

