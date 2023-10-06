#include "macros.h"
typedef enum state {
    START = 0,
    FLAG_RCV = 1,
    A_RCV = 2,
    C_RCV = 3,
    BCC_OK = 4,
    STATE_STOP = 5
} state;

typedef struct state_machine {
    state current_state;
    unsigned char adressByte;
} state_machine;


//void transition(state_machine* st, unsigned char* frame, int len);

void transition(state_machine* st, unsigned char* frame, int len){

    unsigned char A, C;


    for (int i = 0; i < len; i++){
        char trans = frame[i];
        
        switch(st->current_state){

            case START:
                if (trans == FLAG){
                    st->current_state = FLAG_RCV;
                    continue;
                }
                continue;
            case FLAG_RCV:
                if (trans == FLAG) continue;;
                if (trans == st->adressByte){
                    st->current_state = A_RCV;
                    continue;
                }
                st->current_state = START;
                continue;
            case A_RCV:
                if (trans == FLAG){
                    st->current_state = FLAG_RCV;
                    continue;
                }
                else if (trans == C_SET){
                    st->current_state = C_RCV;
                    continue;
                }
                st->current_state = START;
                continue;

            case C_RCV:
                if (trans == FLAG){
                    st->current_state = START;
                    continue;
                }
                else if (trans == (A_SENDER^C_SET)){
                    st->current_state = BCC_OK;
                    continue;
                }
                st->current_state = START;
                continue;
            
            case BCC_OK:
                if (trans == FLAG){
                    printf("Right condition!");
                    st->current_state = STATE_STOP;
                    continue;
                }
                printf("Didn't work :(\n");
                st->current_state = START;
                continue;

            default:
                continue;

        }
    }
}