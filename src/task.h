// #ifndef TASK_H
// #define TASK_H

// #include <Arduino.h>

// class Task {
//     public:
//     Task(const char* task_name, uint32_t stack_size, UBaseType_t priority) {
//         Serial.println("task constructor");
//         taskHandle = NULL;
//         bool res = create(task_name, stack_size, priority);

//     }

//     static void vTaskCode( void * pvParameters ) {
//         Serial.println("task code");
//         Task* task = (Task*) pvParameters;
//         task->run();
//         vTaskDelete(NULL);
//     }

//     virtual void run()=0; //cannot use task without inherited - pure virtual function
    
//     bool create(const char* task_name, uint32_t stack_size, UBaseType_t priority) {
//         Serial.println("create");
//         return xTaskCreate( vTaskCode, task_name, stack_size, (void *)this, tskIDLE_PRIORITY, &taskHandle );
//     }

//     TaskHandle_t getTaskHandle() {
//         return taskHandle;
//     }
    
//     private:
//     TaskHandle_t taskHandle;
// };

// #endif