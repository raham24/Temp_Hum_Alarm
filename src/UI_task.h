#ifndef UI_TASK_H
#define UI_TASK_H
#include "task.h"

class UI_task:public Task {
    
    using Task::Task;

    private:
    bool isConstruct;
    
    public:
    UI_task(const char* task_name, uint32_t stack_size, UBaseType_t priority):Task(task_name, stack_size, priority) {
        Serial.begin(9600);
        isConstruct = true;
        Serial.println("UI Task");

 }
    
    virtual void run() {
        while (!isSerialInit) {
            delay(100);
        }
        Serial.println("Hello from UI task");
        delay(1000);

    }

    bool isConstructed() {
        return isConstruct;
    }



};



#endif