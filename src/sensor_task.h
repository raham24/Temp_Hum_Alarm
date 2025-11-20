#ifndef SENSOR_TASK_H
#define SENSOR_TASK_H

#include "display.h"
#include "task.h"

class Sensor:public Task {

    using Task::Task;

    virtual void run() {
        while (!isSerialInit()) {
            delay(100);
        }
        Serial.println("Hello from sensor task");
        delay(1000);

    }

};

#endif