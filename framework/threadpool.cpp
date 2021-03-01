#include "threadpool.h"

ThreadPool::ThreadPool(const std::string& namePrefix, int32_t numThreads)
: mNamePrefix(namePrefix)
, mNumThreads(numThreads)
{

}
ThreadPool::~ThreadPool()
{
    printf("~ThreadPool start\n");
    {
        std::lock_guard<std::mutex> lock(mMutex);
        printf("~ThreadPool mStoped true\n");
        mStoped = true;
        mCondition.notify_all();
    }

    for (int i = 0; i < mThreads.size(); ++i) {
        printf("~ThreadPool Join :%d\n", i);
        mThreads[i]->Join();
        delete mThreads[i];
    }

    mThreads.clear();
}

void ThreadPool::StartWorkers()
{
    for (int i = 0; i < mNumThreads; i++) {
        mThreads.push_back(new WorkerThread(this, mNamePrefix + "_" + std::to_string(i)));
    }
}

void ThreadPool::Schedule(std::function<void()> callback)
{
    std::unique_lock<std::mutex> lock(mMutex);
    mTasks.push(std::move(callback));
    mCondition.notify_one();
}

void ThreadPool::WaitAllTasks()
{
    std::unique_lock<std::mutex> lock(mMutex);
    if (mTasks.empty())
        return;
    
    mTaskEmptyCondition.wait(lock);
}

void ThreadPool::RunWorker()
{
    printf("ThreadPool::RunWorker start \n");
    std::unique_lock<std::mutex> lock(mMutex);
    while (true)
    {
        if (!mTasks.empty()) {
            std::function<void()> task = std::move(mTasks.front());
            mTasks.pop();
            lock.unlock();
            task();
            lock.lock();
        } else {
            mTaskEmptyCondition.notify_all();
            if (mStoped) {
                break;
            } else {
                mCondition.wait(lock);
            }
        }
    }
    printf("ThreadPool::RunWorker end \n");
}
//--------------------------------------------------------
//
// WorkerThread
//
//--------------------------------------------------------

ThreadPool::WorkerThread::WorkerThread(ThreadPool* pool, const std::string& name_prefix)
: mPool(pool)
, mName(name_prefix)
{
    mThread = std::thread(ThreadBody, this);
}

ThreadPool::WorkerThread::~WorkerThread()
{

}

void ThreadPool::WorkerThread::Join()
{
    mThread.join();
}

void* ThreadPool::WorkerThread::ThreadBody(void* arg)
{
    auto thread = reinterpret_cast<WorkerThread*>(arg);

    thread->mPool->RunWorker();
    return nullptr;
}
