#include <algorithm>
#include "Observer.h"
#include <assert.h>

void Subject::attach(Observer *o) {
    assert(o != nullptr);
    observers.push_back(o);
}

void Subject::detach(Observer *o) {
    auto r = std::remove_if(
            observers.begin(),
            observers.end(),
            [o](auto* p) {
                return p == o;
            });
    observers.erase(r, observers.end());
}

void Subject::notify(const UpdateInfo& info) {
    for (auto* o : observers) {
        o->update(info);
    }
}
