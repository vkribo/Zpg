#include "EventType.h"

struct UpdateInfo {
    EventType type;
    UpdateInfo(EventType type) : type(type) {}
    virtual ~UpdateInfo() = default; // hack to make sure dynamic_cast works
};

template<class T>
struct UpdateValueInfo : public UpdateInfo {
    const T& value;
    UpdateValueInfo(EventType type, const T& value) : UpdateInfo(type), value(value) {}
};
