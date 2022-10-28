#include "EventType.h"

struct UpdateInfo {
    EventType type;
    UpdateInfo(EventType type) : type(type) {}
    virtual ~UpdateInfo() = default; // hack to make sure dynamic_cast works
};

template<class T>
struct UpdateValueInfo : public UpdateInfo {
    T value;
    UpdateValueInfo(EventType type, T value) : UpdateInfo(type), value(value) {}
};