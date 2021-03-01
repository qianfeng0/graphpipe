#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>

#include "status.h"
#include "holder.hpp"

// class Packet;
// Packet Create(HolderBase* holder);

class Packet
{
public:
    Packet() = default;
    ~Packet() = default;

    // Copy constructor and assignment operator.
    Packet(const Packet&);
    Packet& operator=(const Packet&);

    // Move constructor and assignment operator that take non-const rvalue
    // reference.
    Packet(Packet&&);
    Packet& operator=(Packet&&);

    bool IsEmpty() const;

    // Returns an error if the packet does not contain data of type T.
    template <typename T>
    StatusCode ValidateAsType() const;

    template <typename T>
    const T& Get() const;


private:
    friend Packet Create(HolderBase* holder);
    std::shared_ptr<HolderBase> mHolder;
};


inline Packet::Packet(const Packet& packet)
    : mHolder(packet.mHolder) {
  printf("Using copy constructor of \n");
}

inline Packet& Packet::operator=(const Packet& packet) {
    printf("Using copy assignment operator of \n");
    if (this != &packet) {
        mHolder = packet.mHolder;
    }
    return *this;
}

inline Packet::Packet(Packet&& packet) {
    printf("Using move constructor of \n");
    mHolder = std::move(packet.mHolder);
}

inline Packet& Packet::operator=(Packet&& packet) {
    printf("Using move assignment operator of \b");
    if (this != &packet) {
        mHolder = std::move(packet.mHolder);
    }
    return *this;
}

inline bool Packet::IsEmpty() const { return mHolder == nullptr; }

template <typename T>
StatusCode Packet::ValidateAsType() const {
//   if (ABSL_PREDICT_FALSE(IsEmpty())) {
//     return ::mediapipe::InternalError(absl::StrCat(
//         "Expected a Packet of type: ", MediaPipeTypeStringOrDemangled<T>(),
//         ", but received an empty Packet."));
//   }
//   if (ABSL_PREDICT_FALSE(holder_->As<T>() == nullptr)) {
//     return ::mediapipe::InvalidArgumentError(absl::StrCat(
//         "The Packet stores \"", holder_->DebugTypeName(), "\", but \"",
//         MediaPipeTypeStringOrDemangled<T>(), "\" was requested."));
//   }
//   return ::mediapipe::OkStatus();
    if (IsEmpty()) {
        printf("Expected a Packet of type:%s, but received an empty Packet.\n",  MediaPipeTypeStringOrDemangled<T>().c_str());
        return StatusCode::kUnknown;
    }
    if (mHolder->As<T>() == nullptr) {
        printf("The Packet stores %s, but %s was requested.\n", mHolder->DebugTypeName().c_str(), MediaPipeTypeStringOrDemangled<T>().c_str());
        return StatusCode::kUnknown;
    }

    return StatusCode::kOk;
}

template <typename T>
inline const T& Packet::Get() const {
    Holder<T>* holder = IsEmpty() ? nullptr : mHolder->As<T>();
    if (holder == nullptr) {
        // Produce a good error message.
        StatusCode status = ValidateAsType<T>();
        // LOG(FATAL) << "Packet::Get() failed: " << status.message();
        printf("Packet::Get() failed:");
    }
    return holder->Data();
}

Packet Create(HolderBase* holder) {
    Packet result;
    result.mHolder.reset(holder);
    return result;
}

template <typename T>
Packet Adopt(const T* ptr) {
    return Create(new Holder<T>(ptr));
}

#endif