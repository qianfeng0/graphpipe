
#include "gmock/gmock.h"

#include <vector>
#include <map>

#include "type_util.hpp"
#include "holder.hpp"
#include "packet.h"
#include "type_map.h"

struct stData{
    int* ptr;
    int32_t size;
};

void initData(stData *data) {
    data->ptr = new int[10];
    data->size = 10;
}

void freeData(stData *data) {
    if (data) {
        delete[] data->ptr;
        data->size = 0;
    }
}

class DataDeleter {
public:
    // void operator()(void* x) const {
    //     stData* data = static_cast<stData*>(x);
    //     if (data) {
    //         freeData(data);
    //         delete data;
    //     }
    // }
    void operator()(stData* x) const {
        // stData* data = static_cast<stData*>(x);
        stData* data = x;
        if (data) {
            freeData(data);
            delete data;
        }
    }
};

TEST(packet, type_util)
{
    EXPECT_EQ(TypeInfo::Get<int32_t>, TypeInfo::Get<int32_t>);

    const TypeInfo& type32 = TypeInfo::Get<int32_t>();
    // auto type32_1 = TypeId<int32_t>();
    EXPECT_NE(TypeInfo::Get<int64_t>, TypeInfo::Get<int32_t>);
    EXPECT_NE(TypeInfo::Get<std::vector<int>>, TypeInfo::Get<int32_t>);

    printf("HashCode: %zu, name: %s\n", TypeId<int32_t>().HashCode(), TypeId<int32_t>().Name());
    printf("HashCode: %zu, name: %s\n", TypeId<int32_t>().HashCode(), TypeId<int32_t>().Name());
    printf("HashCode: %zu, name: %s\n", TypeId<int64_t>().HashCode(), TypeId<int64_t>().Name());
    printf("HashCode: %zu, name: %s\n", TypeId<std::vector<int32_t>>().HashCode(), TypeId<std::vector<int32_t>>().Name());
    printf("HashCode: %zu, name: %s\n", TypeId<std::vector<int64_t>>().HashCode(), TypeId<std::vector<int64_t>>().Name());
}

TEST(packet, holder)
{
    {
        Holder<int32_t> hold1(new int32_t(100));
        EXPECT_EQ(hold1.Data(), 100);

        HolderBase *baseHolder = &hold1;
        Holder<int32_t> *phold = dynamic_cast<Holder<int32_t>*>(baseHolder);
        EXPECT_NE(phold, nullptr);
        const int32_t &a = baseHolder->As<int32_t>()->Data();

        std::unique_ptr<int32_t> ptr = baseHolder->As<int32_t>()->Release();
        EXPECT_EQ(*ptr, 100);
    }

    {
        int32_t val32 = 12;
        ForeignHolder<int32_t> hold2(&val32);
        EXPECT_EQ(hold2.Data(), 12);
        HolderBase *baseHolder = &hold2;
        const int32_t &b = baseHolder->As<int32_t>()->Data();
        std::unique_ptr<int32_t> ptr = baseHolder->As<int32_t>()->Release();
        EXPECT_EQ(ptr, nullptr);
    }

    {
        std::unique_ptr<stData, DataDeleter> data(new stData());
        initData(data.get());

        auto func_delete = [](stData *data) {
            printf("func_delete %p \n", data);
            if (data) {
                delete[] data->ptr;
                data->size = 0;
                delete data;
            }
        };
        std::unique_ptr<stData, decltype(func_delete)> data2(new stData(), func_delete);
        std::unique_ptr<stData, DataDeleter> data3(new stData());

        Holder<std::unique_ptr<stData, DataDeleter>> hold1(new std::unique_ptr<stData, DataDeleter>(new stData()));
        initData(hold1.As<std::unique_ptr<stData, DataDeleter>>()->Data().get());
        HolderBase *baseHolder = &hold1;
        auto a = baseHolder->As<std::unique_ptr<stData, DataDeleter>>()->Data()->size;
        EXPECT_EQ(a, 10);
        auto ptr = baseHolder->As<std::unique_ptr<stData, DataDeleter>>()->Release();
        EXPECT_NE(ptr.get()->get(), nullptr);
    }

    auto func_delete = [](int* ptr) {
        delete ptr;
    };
    std::unique_ptr<int, decltype(func_delete)> upint(new int, func_delete);
}

TEST(packet, Packet)
{
    SetMediaPipeType<int32_t>("int32_t");
    SetMediaPipeType<int64_t>("int64_t");
    SetMediaPipeType<int16_t>("int16_t");
    SetMediaPipeType<std::unique_ptr<stData, DataDeleter>>("std::unique_ptr<stData, DataDeleter>");
    {
        Packet a;

        Packet b = Adopt<int64_t>(new int64_t);
        b.Get<int64_t>();

        Packet c = Adopt<std::unique_ptr<stData, DataDeleter>>(new std::unique_ptr<stData, DataDeleter>(new stData()));
        const std::unique_ptr<stData, DataDeleter> &ptr = c.Get<std::unique_ptr<stData, DataDeleter>>();
        
    }
}

