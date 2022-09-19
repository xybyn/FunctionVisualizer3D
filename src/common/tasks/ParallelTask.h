//
// Created by pc on 18.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_PARALLELTASK_H
#define FUNCTIONVISUALIZER3D_PARALLELTASK_H
#include <thread>
template<typename T>
class ParallelTask
{
public:
    virtual ~ParallelTask() {}
    bool isInProgress() const
    { return in_progress; }

    bool isDone() const
    { return is_done; }

    float getProgress() const
    { return progress; }

    virtual void run()
    {
        in_progress = true;
        is_done = false;
    }

    T &getResult()
    { return result; }

protected:
    T result;
    bool in_progress = false;
    bool is_done = false;
    float progress = 0.0f;

    void done()
    {
        is_done = true;
        in_progress = false;
        progress = 1.0f;
    }
};


#endif //FUNCTIONVISUALIZER3D_PARALLELTASK_H
