#include <condition_variable>
#include <mutex>
#include <iostream>
#include <signal.h>

static std::condition_variable condition;
static std::mutex mutex;

namespace kj
{
    class InterruptHandler 
    {
    public:
        static void hookSIGINT() 
        {
            signal(SIGINT, handleUserInterrupt);        
        }

        static void handleUserInterrupt(int signal)
        {
            if (signal == SIGINT) 
            {
                std::cout << "SIGINT trapped ..." << '\n';
                condition.notify_one();
            }
        }

        static void waitForUserInterrupt() 
        {
            std::unique_lock<std::mutex> lock{mutex};
            condition.wait(lock);
            std::cout << "user has signaled to interrup program..." << '\n';
            lock.unlock();
        }
    };
}