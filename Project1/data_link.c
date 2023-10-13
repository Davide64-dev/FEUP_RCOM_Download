#include "data_link.h"
#include "statemachine.h"

volatile int STOP = FALSE;

struct linkLayer* llOpenTransmiter(char* port){
    struct linkLayer* ll;
    ll = (struct linkLayer*)malloc(sizeof(struct linkLayer));
    int fd = open(port, O_RDWR | O_NOCTTY);
    strcpy(ll->port, port);
    unsigned char buf[SET_SIZE] = {FLAG, A_SENDER, C_SET, A_SENDER ^ C_SET, FLAG};
    int finish = FALSE;

    while(!finish){
        int bytes = write(fd, buf, SET_SIZE);
        int bytes1 = read(fd, buf, SET_SIZE);
        buf[bytes1] = '\0';
        if (buf[0] == 'z')
            STOP = TRUE;
        alarm(3);

        if (bytes1 >= 0){
            alarm(0);
            finish = TRUE;
        }

        printf("%d\n", bytes);
    }
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
        transition(st, buf, bytes);
        char A = st->current_state;
        printf("The final state is: %u\n", A);
        if (st->current_state == STATE_STOP){
            printf("Right state! Sending back the response\n");
            unsigned char buf[SET_SIZE] = {FLAG, A_RECEIVER, C_UA, A_RECEIVER ^ C_UA, FLAG};
            int bytes = write(fd, buf, SET_SIZE);
            STOP = TRUE;
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

unsigned char BCC2(unsigned char* frame, int len){
    unsigned char res = frame[0];
    for (int i = 1; i < len; i++){
        res = res ^frame[i];
    }
    return res;
}

void llwrite(struct linkLayer* li, char* frame, int length){
    char buf[length + 6];
    buf[0] = FLAG;
    buf[1] = A_SENDER;

    if (li->sequenceNumber == 0) buf[2] = 0;
    else buf[2] = 0x40;

    buf[3] = A_SENDER ^buf[2];

    for (int i =4; i<length;i++)
        buf[i] = frame[i-4];

    buf[length + 4] = BCC2(frame, length);

    buf[length + 5] = FLAG;
}


int main(){
    struct linkLayer* fd = llopen("/dev/ttyS10", RECEIVER);
    llwrite(fd, "Hello, World", 14);
    return 0;
}