#ifndef THREAD_HPP
#define THREAD_HPP

#include <iostream>
#include <thread>
#include <atomic>

#define MAX_THREAD 2

class Thread
{

private:
    static std::thread *thread_arr[MAX_THREAD];

public:
    template <class _Fp, class... _Args>
    static void createThread(_Fp&& __f, _Args&&... __args)
    {
        for (int i = 0; i < MAX_THREAD; i++)
        {
            if (thread_arr[i] == nullptr)
            {
                thread_arr[i] = new std::thread(__f, __args...);
                break;
            }
        }
    }
    static void cleanThread();
};

#endif // THREAD_HPP