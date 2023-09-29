typedef enum state {
    START = 0,
    FLAG_RCV,
    A_RCV,
    C_RCV,
    BCC_OK,
    STOP
} state;

typedef struct state_machine {
    state current_state;
    unsigned char adressByte;
} state_machine;


void transition(state_machine* st, unsigned char trans);