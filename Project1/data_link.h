#include "macros.h"

struct linkLayer {
    char port[20]; /* Dispositivo /dev/ttySx, x = 0, 1 */
    int baudRate;  /* Velocidade de transmissão */
    unsigned int sequenceNumber; /* Número de sequência da trama: 0, 1 */
    unsigned int timeout; /* Valor do temporizador: 1 s */
    unsigned int numTransmissions; /* Número de tentativas em caso de falha */
    char frame[MAX_SIZE]; /* Trama */
};

void llopen(struct linkLayer* li, char* port);

struct linkLayer* llOpenTransmiter(char* port);

void llwrite(struct linkLayer* li);

void llread(struct linkLayer* li);

void llclose(struct linkLayer* li);
