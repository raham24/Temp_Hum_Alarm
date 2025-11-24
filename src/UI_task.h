// #ifndef UI_TASK_H
// #define UI_TASK_H
// #include "task.h"

// class UI_task:public Task {
    
//     //using Task::Task;

//     public:
//     UI_task(const char* task_name, uint32_t stack_size, UBaseType_t priority):Task(task_name, stack_size, priority) {
//         isConstruct = true;
//         Serial.println("UI Task");

//     }
    
//     virtual void run() {
//         delay(1000);
//         Serial.println("Hello from UI task");
//         delay(1000);

//     }

//     bool isConstructed() {
//         return isConstruct;
//     }

//     private:
//     bool isConstruct;

// };



// #endif