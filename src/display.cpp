#include "display.h"

void display(String str) {

    Serial.println(str);

}
bool isDisplayInit = false;

void initSerial(long baud){
    if (!isDisplayInit) {
        Serial.begin(baud);
        isDisplayInit = true;
    } 
}

bool isSerialInit(){
    return isDisplayInit;
}