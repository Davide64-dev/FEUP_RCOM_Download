#include "application_layer.h"

int main(){
    applicationLayer("/dev/ttyS11", TRANSMITER, 9600, 3, 3, "alanturing.gif");
    
    return 0;
}
