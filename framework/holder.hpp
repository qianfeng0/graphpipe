#ifndef _HOLDER_H_
#define _HOLDER_H_

#include <stdio.h>
#include <memory>

#include "type_util.hpp"
#include "type_map.h"

template<typename T>
class Holder;
template<typename T>
class ForeignHolder;

class HolderBase
{
public:
    HolderBase() {}
    HolderBase(const HolderBase&) = delete;
    HolderBase& operator=(const HolderBase&) = delete;
    virtual ~HolderBase(){};

    template<typename T>
    void SetHolderTypeId() {
        mTypeId = GetTypeHash<T>();
    }
    size_t GetHolderTypeId() {
        return mTypeId;
    }
    template <typename T>
    bool HolderIsType() const{
        return mTypeId == GetTypeHash<T>();
    };

    virtual size_t GetTypeId() const = 0;
    virtual const std::string DebugTypeName() const = 0;

    template <typename T>
    Holder<T>* As();

    // Same as non-const As() function.
    template <typename T>
    const Holder<T>* As() const;

private:
    size_t mTypeId;
};

template <typename T>
Holder<T>* HolderBase::As()
{
    if (HolderIsType<Holder<T>>() || HolderIsType<ForeignHolder<T>>()) {
        return static_cast<Holder<T>* >(this);
    }
    return nullptr;
}

template <typename T>
const Holder<T>* HolderBase::As() const
{
    if (HolderIsType<Holder<T>>()) {
        return static_cast<Holder<T>* >(this);
    }
    return nullptr;
}

template<typename T>
class Holder : public HolderBase
{
public:
    explicit Holder(const T* ptr) : mPtr(ptr) {
        SetHolderTypeId<Holder>();
    }
    ~Holder() override { delete_helper(); }
    const T& Data() const {
        // HolderSupport<T>::EnsureStaticInit();
        return *mPtr;
    }
    size_t GetTypeId() const final { return GetTypeHash<T>(); }
    const std::string DebugTypeName() const final {
        return MediaPipeTypeStringOrDemangled<T>();
    }

    template <typename U = T>
    std::unique_ptr<T> Release(
        typename std::enable_if<!std::is_array<U>::value>::type* = 0) {
        // Since C++ doesn't allow virtual, templated functions, check holder
        // type here to make sure it's not upcasted from a ForeignHolder.
        if (!HolderIsType<Holder<T>>()) {
            // return InternalError(
            //     "Foreign holder can't release data ptr without ownership.");
            printf("Foreign holder can't release data ptr without ownership.");
            return std::unique_ptr<T>();
        }
        // Casts away constness to make the data mutable after the release.
        std::unique_ptr<T> data_ptr(const_cast<T*>(mPtr));
        mPtr = nullptr;
        return std::move(data_ptr);
    }

protected:
    const T* mPtr;

private:
    // Call delete[] if T is an array, delete otherwise.
    template <typename U = T>
    inline void delete_helper(
        typename std::enable_if<!std::is_array<U>::value>::type* = 0) {
        printf("delete_helper 1 \n");
        delete mPtr;
    }

    // template <typename U = T>
    // inline void delete_helper(
    //     typename std::enable_if<std::is_array<U>::value>::type* = 0) {
    //     // Casts ptr_ from const Type(*)[] or const Type(*)[N] to const Type*.
    //     // Deleting a pointer to incomplete type (Type(*)[]) causes compile error.
    //     printf("delete_helper 2 \n");
    //     delete[] reinterpret_cast<const typename std::remove_extent<T>::type*>(
    //         mPtr);
    // }
};

// Like Holder, but does not own its data.
template <typename T>
class ForeignHolder : public Holder<T> {
public:
    explicit ForeignHolder(const T* ptr) : Holder<T>(ptr) {
        // Distinguishes between Holder and ForeignHolder since Consume() treats
        // them differently.
        this->template SetHolderTypeId<ForeignHolder>();
        // SetHolderTypeId<ForeignHolder>();
    }
    ~ForeignHolder() override {
        // Null out ptr_ so it doesn't get deleted by ~Holder.
        this->mPtr = nullptr;
    }
    // Foreign holder can't release data pointer without ownership.
    // ::mediapipe::StatusOr<std::unique_ptr<T>> Release() {
    //     return ::mediapipe::InternalError(
    //         "Foreign holder can't release data ptr without ownership.");
    // }
    std::unique_ptr<T> Release() {
        // return ::mediapipe::InternalError(
        //     "Foreign holder can't release data ptr without ownership.");
        printf("Foreign holder can't release data ptr without ownership.");
        return std::unique_ptr<T>();
    }

};

#endif