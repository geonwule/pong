#ifndef THREAD_HPP
#define THREAD_HPP

#include <iostream>
#include <thread>

#define MAX_THREAD 4

class Thread
{

private:
    static std::thread *thread_arr[MAX_THREAD];

public:
    template <class _Fp, class... _Args>
    static int createThread(_Fp&& __f, _Args&&... __args)
    {
        for (int i = 0; i < MAX_THREAD; i++)
        {
            if (thread_arr[i] == nullptr)
            {
                thread_arr[i] = new std::thread(__f, __args...);
                return EXIT_SUCCESS;
            }
        }
        return EXIT_FAILURE;
    }
    static void cleanThread();
};

#endif // THREAD_HPP