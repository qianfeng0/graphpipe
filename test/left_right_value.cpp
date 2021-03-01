#include <stdio.h>
#include <vector>

#include "gmock/gmock.h"

class LRValue
{

public:
    LRValue() {
        val32 = 0;
    }
    LRValue(int32_t val) {
        val32 = val;
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
    }
    ~LRValue() {
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
    }

    LRValue(const LRValue& src) {
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
        val32 = src.val32;
    }
    LRValue& operator=(const LRValue& src) {
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
        val32 = src.val32;
        return *this;
    }

    LRValue(LRValue&& src) {
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
        val32 = src.val32;
        src.val32 = 0;
    }
    LRValue& operator=(LRValue&& src){
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
        val32 = src.val32;
        src.val32 = 0;
        return *this;
    }
    
    void dump() {
        printf("func: %s, this:%p, value:%d\n", __PRETTY_FUNCTION__, this, val32);
    }
public:
    int32_t val32 = 0;
};

// LRValue::LRValue(/* args */)
// {
// }

// LRValue::~LRValue()
// {
// }
int function0(LRValue value) {
    return value.val32;
}

int function1(LRValue &value) {
    return value.val32;
}

int function2(LRValue &&value) {
    return value.val32;
}

LRValue funcCreate() {
    LRValue a = LRValue(10);
    return a;
}

TEST(left_right, test0)
{
    {
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue a = LRValue(10);
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue b = LRValue(20);
        printf("\n======== step line:%d ========\n", __LINE__);
        b = LRValue(20);
        printf("\n======== step line:%d ========\n", __LINE__);
        b = std::move(LRValue(20));
        printf("\n======== step line:%d ========\n", __LINE__);
        b = std::move(a);
        printf("\n======== step line:%d ========\n", __LINE__);
        b = a;
        printf("\n======== step line:%d ========\n", __LINE__);
    }

    {
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue a = LRValue(10);
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue b = LRValue(20);
        printf("\n======== step line:%d ========\n", __LINE__);
        std::vector<LRValue> vec;
        vec.reserve(100);
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.push_back(LRValue(101));        //LRValue::LRValue(int32_t), LRValue::LRValue(LRValue &&), LRValue::~LRValue()
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.emplace_back(LRValue(102));
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.push_back(std::move(LRValue(103)));
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.push_back(103);
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.emplace_back(103);      //将参数传递给构造函
        printf("\n======== step line:%d ========\n", __LINE__);

        printf("\n======== step line:%d ========\n", __LINE__);
        vec.push_back(a);
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.push_back(std::move(a));

        printf("\n======== step line:%d ========\n", __LINE__);
        vec.emplace_back(b);
        printf("\n======== step line:%d ========\n", __LINE__);
        vec.emplace_back(std::move(b));
        printf("\n======== step line:%d ========\n", __LINE__);

    }

    {
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue a = LRValue(10);
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue b = LRValue(20);
        printf("\n======== step line:%d ========\n", __LINE__);
        function0(a);
        printf("\n======== step line:%d ========\n", __LINE__);
        function1(a);
        printf("\n======== step line:%d ========\n", __LINE__);
        function2(LRValue(20));
        printf("\n======== step line:%d ========\n", __LINE__);
    }

    {
        printf("\n======== step line:%d ========\n", __LINE__);
        LRValue a = funcCreate();
        printf("\n======== step line:%d ========\n", __LINE__);
    }
}