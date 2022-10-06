//
// Created by pc on 18.09.22.
//

#ifndef FUNCTIONVISUALIZER3D_PARALLELTASK_H
#define FUNCTIONVISUALIZER3D_PARALLELTASK_H
#include <thread>
#include <functional>
template<typename T>
class Event
{
public:
	void add(std::function<void(T)> function)
	{
		functions.push_back(function);
	}
	void run(T a)
	{
		for (int i = 0; i < functions.size(); i++)
		{
			functions[i](a);
		}
	}
private:
	std::vector<std::function<void(T)>> functions;
};

template<typename T>
class ParallelTask
{
public:
	virtual ~ParallelTask()
	{
		t->join();
		delete t;
	}
	bool isInProgress() const
	{
		return in_progress;
	}

	bool isDone() const
	{
		return is_done;
	}

	float getProgress() const
	{
		return progress;
	}
	void update()
	{

		if (isInProgress())
		{
			onProcessEvent.run(progress);
		}
		if (isDone())
		{
			onProcessEvent.run(1.0f);
			onDoneEvent.run(ref(result));
		}
	}
	virtual void run()
	{
		in_progress = true;
		is_done = false;

		t = new std::thread(&ParallelTask::process, this);

	}

	virtual void process()
	{
	}
	T& getResult()
	{
		return result;
	}

	Event<T&> onDoneEvent;
	Event<float> onProcessEvent;
protected:
	std::thread* t = nullptr;
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
