#include "data_link.h"


unsigned char* createControlPacket(int C, long FileSize, const char* filename, int* packetSize){
    const int L1 = sizeof(FileSize);
    const int L2 = strlen(filename);
    int finalSize = 1 + 2 + L1 + 2 + L2;
    *packetSize = finalSize;

    unsigned char* ret = (unsigned char*)malloc(finalSize);

    ret[0] = C;
    ret[1] = 0;
    ret[2] = L1;

    for (unsigned char i = 0 ; i < L1 ; i++) {
        ret[2+L1-i] = FileSize | 0x00;
        FileSize >>= 8;
    }

    ret[L1+3] = 1;
    ret[L1+4] = L2;

    for (int i = 0; i < L2; i++){
        ret[L1 + 5 + i] = filename[i];
    }

    for (int i = 0; i < finalSize; i++){
        printf("%d,",ret[i]);
    }
    printf("\n");

    return ret;
}

unsigned char* createDataPacket(const unsigned char* packageContent, int packageContentSize, int* packetSize){
    int L2 = packageContentSize / 256;
    int L1 = packageContentSize % 256;
    *packetSize = 3 + packageContentSize;
    unsigned char* ret = (unsigned char*)malloc(*packetSize);

    ret[0] = 1;
    ret[1] = L2;
    ret[2] = L1;
    for (int i = 0; i < packageContentSize; i++){
        ret[i + 3] = packageContent[i];
    }

    return ret;
}

void applicationLayerTransmiter(struct linkLayer* ll, const char *filename){
    llopen(ll, TRANSMITER);
    FILE* file = fopen(filename, "rb");

    int begin = ftell(file);
    fseek(file, 0, SEEK_END);
    long FileSize = ftell(file);
    fseek(file, begin, SEEK_SET);

    printf("Size of the file: %ld\n", FileSize);

    int controlPacketLen;

    unsigned char* controlPacket = createControlPacket(2, FileSize, filename, &controlPacketLen);

    llwrite(ll, controlPacket, controlPacketLen);

    char* data = (char *)malloc(FileSize);

    fread(data, FileSize, FileSize, file);

    for (int i = 0; i < FileSize; i++){
        printf("%d,", data[i]);
    }
    printf("\n");

    long int RemainingBytes = FileSize;

    while (RemainingBytes >= 0){
        int packageContentSize;
        if (RemainingBytes > 1000) packageContentSize = 1000;
        else packageContentSize = RemainingBytes;

        unsigned char* packageContent = (unsigned char*) malloc(packageContentSize);

        memcpy(packageContent, data, packageContentSize);

        int packageSize = 0;

        unsigned char* package = createDataPacket(package, packageContentSize, &packageSize);

        llwrite(ll, package, 100);

        RemainingBytes -= packageContentSize;
        data += packageContentSize;
        free(packageContent);
    }

    controlPacket = createControlPacket(3, FileSize, filename, &controlPacketLen);

    llwrite(ll, controlPacket, controlPacketLen);

    free(data);


    fclose(file);
}

void applicationLayerReceiver(struct linkLayer* ll){
    printf("cheguei aqui!");
    llopen(ll, RECEIVER);

    char *packet = (char *)malloc(1000);

    llread(ll, packet);

    char* FileName = "teste.gif"; // alterar isto para conseguir o valor certo do package;

    FILE* File = fopen(FileName, "wb+");

    int finish = FALSE;

    unsigned long int practicalSize = 0;

    while (!finish){
        llread(ll, packet);
        if(packet[3] == 3) finish = TRUE;
        else{
            printf("cheguei aqui\n");
            unsigned long int contentSize = packet[1] * 256 + packet[2];
            practicalSize += contentSize;
            char* content = (char *)malloc(contentSize);

            memcpy(content, &packet[3], practicalSize);
            fwrite(content, 1, contentSize, File);
            free(content);
        }
    }

    free(packet);

    fclose(File);
}


void applicationLayer(const char *serialPort, int mode, int baudRate,
                      int nTries, int timeout, const char *filename){
    
    struct linkLayer* ll;
    ll = (struct linkLayer*)malloc(sizeof(struct linkLayer));
    strcpy(ll->port, serialPort);
    ll->baudRate = baudRate;
    ll->timeout = timeout;



    if (mode == RECEIVER){
        printf("receiver\n");
        applicationLayerReceiver(ll);
    }

    if (mode == TRANSMITER){
        printf("transmiter\n");
        applicationLayerTransmiter(ll, filename);
    }

}


int main(){
    applicationLayer("/dev/ttyS11", TRANSMITER, 3, 3, 3, "penguin.gif");
    
    return 0;
}

