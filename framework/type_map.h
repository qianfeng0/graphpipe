#ifndef _TYPE_MAP_
#define _TYPE_MAP_

#include <string>
#include <map>
#include <mutex>

#include "type_util.hpp"

struct MediaPipeTypeData {
    size_t type_id;
    std::string type_string;
    // SerializeFn serialize_fn;
    // DeserializeFn deserialize_fn;
};


class PacketTypeIdToMediaPipeTypeData {
public:
    static void RegisterTypeData(const MediaPipeTypeData &data){
        mMap[data.type_id] = data;
    };

    static MediaPipeTypeData* GetValue(size_t typeId) {
        // TODO: Uses gtl::NoDestructor for the thread-safe one-time
        // initialization if gtl::NoDestructor will be open sourced by ABSL.
        // static MapName* instance = new MapName();
        // return instance;
        auto it = mMap.find(typeId);
        if (it == mMap.end()) {
            return nullptr;
        }
        return (it == mMap.end()) ? nullptr : &it->second;
    }
private:
    static std::map<size_t, MediaPipeTypeData> mMap;
};
std::map<size_t, MediaPipeTypeData> PacketTypeIdToMediaPipeTypeData::mMap;

inline const std::string* MediaPipeTypeStringFromTypeId(const size_t type_id) {
  const MediaPipeTypeData* value =
      PacketTypeIdToMediaPipeTypeData::GetValue(type_id);
  return (value) ? &value->type_string : nullptr;
}

template <typename T>
void SetMediaPipeType(const std::string& type_str) {
    MediaPipeTypeData data = {GetTypeHash<T>(), type_str};
    PacketTypeIdToMediaPipeTypeData::RegisterTypeData(data);
}

template <typename T>
inline const std::string* MediaPipeTypeString() {
  return MediaPipeTypeStringFromTypeId(GetTypeHash<T>());
}

template <typename T>
const std::string MediaPipeTypeStringOrDemangled() {
  const std::string* type_string = MediaPipeTypeString<T>();
  if (type_string) {
    return *type_string;
  } else {
    // return ::mediapipe::Demangle(tool::TypeId<T>().name());
    return TypeId<T>().Name();
  }
}

#endif