#ifndef _TYPE_UTIL_H_
#define _TYPE_UTIL_H_

#include <stdint.h>
#include <stddef.h>

#define HAS_RTTI    (0)

#if (HAS_RTTI == 0)
class TypeInfo
{
public:
    size_t HashCode() const { return reinterpret_cast<size_t>(this); }
    const char* Name() const { return "<unknown>"; }
    bool operator==(const TypeInfo &other) const { return &other == this; };
    bool operator<(const TypeInfo &other) const { return &other < this; };
    template<typename T>
    static const TypeInfo& Get() {
        static TypeInfo* static_type = new TypeInfo;
        return *static_type;
    }

private:
    TypeInfo() {}
    TypeInfo(const TypeInfo&) = delete;
};
#else 

#include <typeinfo>
class TypeInfo
{
public:
    size_t HashCode() const { return mInfo.hash_code(); }
    const char* Name() const { return mInfo.name(); }
    bool operator==(const TypeInfo &other) const { return mInfo == other.mInfo; };
    bool operator<(const TypeInfo &other) const { return mInfo.before(other.mInfo); };
    template<typename T>
    static const TypeInfo& Get() {
        static TypeInfo* static_type = new TypeInfo(typeid(T));
        return *static_type;
    }
private:
    TypeInfo(const std::type_info& info):mInfo(info) {};
    TypeInfo(const TypeInfo&) = delete;
private:
    const std::type_info& mInfo;
};

#endif

template <typename T>
const TypeInfo& TypeId() {
    return TypeInfo::Get<T>();
}

template <typename T>
size_t GetTypeHash() {
  return TypeId<T>().HashCode();
}

#endif