#include "statemachine.h"
#include "macros.h"


void transition(state_machine* st, unsigned char trans){

    unsigned char A, C;

    switch(st->current_state){

        case START:
            if (trans == FLAG){
                st->current_state = FLAG_RCV;
                A = trans;
                return A;
            }
            return -1;
        case FLAG_RCV:
            if (trans == FLAG) return -1;
            if (trans == st->adressByte){
                st->current_state = A_RCV;
                C = trans;
                return C;
            }
            st->current_state = START;
            return -1;
        case A_RCV:
            if (trans == FLAG){
                st->current_state = FLAG_RCV;
            }
            if (trans == 10){ // não está correto, alterar quando perceber o significado daquela transição
                st->current_state = C_RCV;
            }
            st->current_state = START;
            return -1;

        case C_RCV:
            if (trans == FLAG){
                st->current_state = START;
            }
            else if (trans == A^C){
                st->current_state = BCC_OK;
            }
            st->current_state = START;
            return -1;
        
        case BCC_OK:
          if (trans == FLAG){
              st->current_state = STOP;
          }
          st->current_state = START;
          return -1;

        default:
            return -1;

    }
}