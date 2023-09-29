#include "statemachine.h"
#include "macros.h"


void transition(state_machine* st, unsigned char trans){

    unsigned char A, C;

    switch(st->current_state){

        case START:
            if (trans == FLAG){
                st->current_state = FLAG_RCV;
                A = trans;
            }
            break;
        case FLAG_RCV:
            if (trans == FLAG) break;
            if (trans == st->adressByte){
                st->current_state = A_RCV;
                C = trans;
                break;
            }
            st->current_state = START;
            break;
        case A_RCV:
            if (trans == FLAG){
                st->current_state = FLAG_RCV;
                break;
            }
            if (trans == 10){ // não está correto, alterar quando perceber o significado daquela transição
                st->current_state = C_RCV;
                break;
            }
            st->current_state = START;
            break;

        case C_RCV:
            if (trans == FLAG){
                st->current_state = START;
                break;
            }
            else if (trans == A^C){
                st->current_state = BCC_OK;
                break;
            }
            st->current_state = START;
            break;
        
        case BCC_OK:
          if (trans == FLAG){
              st->current_state = STOP;
              break;
          }
          st->current_state = START;
          break;

        default:
            break;

    }
}