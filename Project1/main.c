#include "application_layer.h"

int main(){
    applicationLayer("/dev/ttyS10", RECEIVER, 3, 3, 3, "penguin.gif");
    
    return 0;
}
