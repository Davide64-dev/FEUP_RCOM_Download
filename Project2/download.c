#include "download.h"

int validate_ftp_url(const char *url) {
    const char *pattern = "^ftp://(?:[a-zA-Z0-9]+:[a-zA-Z0-9]+@)?[a-zA-Z0-9.-]+(?:/[a-zA-Z0-9./_-]+)?$";

    regex_t regex;
    int ret = regcomp(&regex, pattern, REG_EXTENDED);
    if (ret != 0) {
        fprintf(stderr, "Error compiling regex\n");
        return -1;
    }

    ret = regexec(&regex, url, 0, NULL, 0);
    regfree(&regex);

    if (ret == 0) {
        printf("Valid FTP URL\n");
        return 1;
    } else if (ret == REG_NOMATCH) {
        printf("Invalid FTP URL\n");
        return 0;
    } else {
        fprintf(stderr, "Error executing regex\n");
        return -1;
    }
}

int main() {
    const char *url = "ftp://user:password@host/example/path";
    validate_ftp_url(url);

    return 0;
}