#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <string>
#include <stdint.h>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <queue>

#define GUARDED_BY(x) __attribute__((guarded_by(x)))

class ThreadPool
{
public:
    ThreadPool(const std::string& namePrefix, int32_t numThreads);
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ~ThreadPool();

    void StartWorkers();
    void Schedule(std::function<void()> callback);
    void WaitAllTasks();
private:
    class WorkerThread;
    void RunWorker();

    std::vector<WorkerThread*> mThreads;
    int mNumThreads = 1;
    std::string mNamePrefix;

    std::mutex mMutex;
    std::condition_variable mCondition;
    bool mStoped GUARDED_BY(mMutex) = false;
    std::queue<std::function<void()>> mTasks GUARDED_BY(mMutex);

    std::condition_variable mTaskEmptyCondition;
};

class ThreadPool::WorkerThread {
public:
    // Creates and starts a thread that runs pool->RunWorker().
    WorkerThread(ThreadPool* pool, const std::string& name_prefix);

    // REQUIRES: Join() must have been called.
    ~WorkerThread();

    // Joins with the running thread.
    void Join();

private:
    static void* ThreadBody(void* arg);

    ThreadPool* mPool;
    std::string mName;
    std::thread mThread;
};

#endif
