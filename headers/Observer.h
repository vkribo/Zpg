#pragma once

#include <vector>
#include "EventType.h"
#include "UpdateInfo.h"



struct Observer {
    virtual void update(const UpdateInfo& info) = 0;
};

class Subject {
    std::vector<Observer*> observers;
public:
    void attach(Observer* o);
    void detach(Observer* o);
    void notify(const UpdateInfo& info);
};

