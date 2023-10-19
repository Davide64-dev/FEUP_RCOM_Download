#include "application_layer.h"

int main(){
    applicationLayer("/dev/ttyS11", TRANSMITER, 3, 3, 3, "penguin.gif");
    
    return 0;
}
