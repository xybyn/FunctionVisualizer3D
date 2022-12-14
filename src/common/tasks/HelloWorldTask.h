//
// Created by pc on 18.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_HELLOWORLDTASK_H
#define FUNCTIONVISUALIZER3D_HELLOWORLDTASK_H


#include "ParallelTask.h"
#include <thread>
#include <iostream>
class HelloWorldTask : public ParallelTask<int>
{
public:
    const char* s;
    HelloWorldTask(const char *s) : s(s){}

    ~HelloWorldTask()
    {
        if(t->joinable())
            t->join();
    }

private:
    void process() override
    {
        std::cout<<"hello world task started"<<std::endl;
        std::cout<<s<<std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress = 0.5f;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        progress = 0.75f;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        done();
        std::cout<<"im done!"<<std::endl;
        result = 228;
    }
};


#endif //FUNCTIONVISUALIZER3D_HELLOWORLDTASK_H
