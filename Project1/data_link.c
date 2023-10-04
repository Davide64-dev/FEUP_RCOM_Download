#include "data_link.h"
#include "statemachine.h"

volatile int STOP = FALSE;

struct linkLayer* llOpenTransmiter(char* port){
    struct linkLayer* ll;
    ll = (struct linkLayer*)malloc(sizeof(struct linkLayer));
    int fd = open(port, O_RDWR | O_NOCTTY);
    strcpy(ll->port, port);

    unsigned char buf[SET_SIZE] = {FLAG, A_SENDER, C_SET, A_SENDER ^ C_SET, '\n'};
    int bytes = write(fd, buf, SET_SIZE);

    int bytes1 = read(fd, buf, SET_SIZE);
    buf[bytes1] = '\0';
    if (buf[0] == 'z')
    STOP = TRUE;

    printf("%d\n", bytes);
    return ll;
}

struct linkLayer* llOpenReceiver(char* port){
    struct linkLayer* ll;
    ll = (struct linkLayer*)malloc(sizeof(struct linkLayer));
    int fd = open(port, O_RDWR | O_NOCTTY);
    strcpy(ll->port, port);
    unsigned char buf[SET_SIZE];

    state_machine* st = (state_machine*)malloc(sizeof(state_machine));
    st->adressByte = A_SENDER;

    while (STOP == FALSE)
    {
        st->current_state = START;
        int bytes = read(fd, buf, SET_SIZE);
        buf[bytes] = '\0';
        //printf(":%s:%d\n", buf, bytes);
        transition(st, buf, 4);
        char A = st->current_state;
        printf("The final state is: %u\n", A);
        if (st->current_state == STATE_STOP || st->current_state == 4){
            printf("Right state! Sending back the response\n");
            unsigned char buf[SET_SIZE] = {FLAG, A_RECEIVER, C_UA, A_RECEIVER ^ C_UA, '\n'};
            int bytes = write(fd, buf, SET_SIZE);
        }
        if (buf[0] == 'z')
            STOP = TRUE;
    }

    free(st);
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