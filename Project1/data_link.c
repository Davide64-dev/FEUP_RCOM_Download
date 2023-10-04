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
        if (buf[0] == 'z')
            STOP = TRUE;
    }

    free(st);

    //printf("%s\n", buf);
    return ll;
}

struct linkLayer* llopen(char* port, int mode){
    if (mode == TRANSMITER){
        return llOpenTransmiter(port);
    }

    return llOpenReceiver(port);

}


int main(){
    llopen("/dev/ttyS10", RECEIVER);
    return 0;
}