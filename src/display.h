#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

void display(String str);

void initSerial(long baud=9600);

bool isSerialInit();

#endif //DISPLAY_H