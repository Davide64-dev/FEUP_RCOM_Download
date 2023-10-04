#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>


struct linkLayer {
    char port[20]; /* Dispositivo /dev/ttySx, x = 0, 1 */
    int baudRate;  /* Velocidade de transmissão */
    unsigned int sequenceNumber; /* Número de sequência da trama: 0, 1 */
    unsigned int timeout; /* Valor do temporizador: 1 s */
    unsigned int numTransmissions; /* Número de tentativas em caso de falha */
    char frame[MAX_SIZE]; /* Trama */
};

struct linkLayer* llopen(char* port, int mode);

struct linkLayer* llOpenTransmiter(char* port);

struct linkLayer* llOpenReceiver(char* port);

void llwrite(struct linkLayer* li);

void llread(struct linkLayer* li);

void llclose(struct linkLayer* li);
