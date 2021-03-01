#include "stdio.h"
#include "stdlib.h"
// #include "gmock/gmock.h"

#include <iostream>
#include "gtest/gtest.h"
#include "type_util.hpp"

int Add(int a, int b)
{
    return a+b;
}

TEST(testCase, test0)
{
    EXPECT_EQ(14, Add(4, 10));
}

// TEST(testCase, test1)
// {
//     EXPECT_EQ(6, Add(5, 7));
// }

// TEST(testCase, test2)
// {
//     EXPECT_EQ(28, Add(8, 10)) << "bugs";
// }

int* func()
{
    int a = 0;
    int *p = &a;
    return p;
}

int main(int argc, char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    //test 
    // void *p = malloc(7);
    // int a[10];
    // int i = 100;
    // a[i] = 123;

    testing::InitGoogleTest(&argc, argv);
    //LOG(FATAL)<<"FATAL";
    return RUN_ALL_TESTS();

    // Use after free
    // {
    //     int *array = new int[100];
    //     delete [] array;
    //     array[10];
    // }

    // Heap buffer overflow
    // int *array = new int[100];
    // array[0] = 0;
    // int res = array[argc + 100];  // BOOM
    // delete [] array;

    //Stack buffer overflow
    // int stack_array[100];
    // stack_array[1] = 0;
    // int t = stack_array[2 + 100];  // BOOM
    return 0;
}
