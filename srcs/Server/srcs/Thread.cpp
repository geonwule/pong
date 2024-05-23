#include "Thread.hpp"
#include <sstream>

std::thread *Thread::thread_arr[MAX_THREAD] = {nullptr};

void Thread::joinThread(std::thread *thread)
{
    if (thread == nullptr)
        return;
    thread->join();
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == thread)
        {
            delete thread_arr[i];
            thread_arr[i] = nullptr;
            break;
        }
    }
    std::cout << "[joinThread] over..." << std::endl;
}

void Thread::cleanThread()
{
    std::cout << "[cleanThread] start..." << std::endl;
    for (int i = 0; i < MAX_THREAD; i++)
    {
        if (thread_arr[i] == nullptr)
            continue;
        std::stringstream ss;
        ss << "thread_arr[" << i << "]->join()...\n";
        std::cout << ss.str();
        thread_arr[i]->join();
        // thread_arr[i]->detach();
        delete thread_arr[i];
        thread_arr[i] = nullptr;
        ss.clear();
        ss << "thread_arr[" << i << "]->join()... done\n";
        std::cout << ss.str();
    }
    std::cout << "[cleanThread] over..." << std::endl;
}