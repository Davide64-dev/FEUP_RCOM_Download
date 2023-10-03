#include "data_link.h"

volatile int STOP = FALSE;

struct linkLayer* llOpenTransmiter(char* port){
    struct linkLayer* ll;
    ll = (struct linkLayer*)malloc(sizeof(struct linkLayer));
    int fd = open(port, O_RDWR | O_NOCTTY);
    strcpy(ll->port, port);

    unsigned char buf[SET_SIZE] = {FLAG, A_SENDER, C_SET, A_SENDER ^ C_SET, '\n'};
    int bytes = write(fd, buf, SET_SIZE);

    printf("%d\n", bytes);
    return ll;
}

struct linkLayer* llOpenReceiver(char* port){
    struct linkLayer* ll;
    ll = (struct linkLayer*)malloc(sizeof(struct linkLayer));
    int fd = open(port, O_RDWR | O_NOCTTY);

    strcpy(ll->port, port);
    unsigned char buf[SET_SIZE];
    while (STOP == FALSE)
    {
        int bytes = read(fd, buf, SET_SIZE);
        buf[bytes] = '\0';
        printf(":%s:%d\n", buf, bytes);
        if (buf[0] == 'z')
            STOP = TRUE;
    }

    printf("%s\n", buf);
    return ll;
}

struct linkLayer* llopen(char* port, int mode){
    if (mode == TRANSMITER){
        return llOpenTransmiter(port);
    }

    return llOpenReceiver(port);

}


int main(){
    llopen("/dev/ttyS11", TRANSMITER);
    return 0;
}