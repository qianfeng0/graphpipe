
#include <unistd.h>
#include <atomic>

#include "gmock/gmock.h"

#include "threadpool.h"

// void func1(std::atomic<int32_t> &v) {
//     v++;
//     usleep(1 * 1000);
// }

// void func2(std::atomic<int32_t> &v) {
//     v++;
//     usleep(2 * 1000);
// }

void func3(std::atomic<int32_t> *v) {
    (*v)++;
    usleep(3 * 1000);
}

void func4(int32_t *v) {
    *v = *v + 1;
    printf("func4 start v:%d, %p\n", *v, v);
    usleep(1 * 100 * 1000);
}

TEST(thread_pool, test0)
{
    std::atomic<int32_t> atomic_cnt(0);
    int32_t cnt = 0;

    ThreadPool pool("pool_test", 4);
    pool.StartWorkers();
    auto f1 = std::bind(func3, &atomic_cnt);

    auto f2 = [&cnt]() {
        cnt++;
        // usleep(1 * 1000 * 1000);
        usleep(10);
    };
    // std::function<void()> f2 = std::bind(func2, cnt);
    // std::function<void()> f3 = std::bind(func3, cnt);

    for (int i = 0; i < 20; i++) {
        pool.Schedule(f2);
        pool.Schedule(f2);
        pool.Schedule(f2);
    }
    // usleep(1000 * 1000);
    pool.WaitAllTasks();
    printf("atomic_cnt: %d, cnt: %d\n", atomic_cnt.load(), cnt);
}