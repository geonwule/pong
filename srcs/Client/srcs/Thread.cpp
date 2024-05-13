#include "Thread.hpp"

std::thread *Thread::thread_arr[MAX_THREAD] = {nullptr};

void Thread::cleanThread()
{
    std::cout << "cleanThread start..." << std::endl;
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == nullptr)
            continue;
        std::cout << "thread_arr[" << i << "]->join()..." << std::endl;
        // thread_arr[i]->join();
        thread_arr[i]->detach();
        delete thread_arr[i];
        thread_arr[i] = nullptr;
        std::cout << "thread_arr[" << i << "]->join()... done" << std::endl;
    }
    std::cout << "cleanThread over..." << std::endl;
}