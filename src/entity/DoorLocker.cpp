#include "DoorLocker.h"

namespace SuperPacMan {
    DoorLocker::DoorLocker(int id) : id_(id), isLocked_(false)
    {}

    void DoorLocker::lock(const Key &key) {
        if (!isLocked_ && key.getId() == id_)
            isLocked_ = true;
    }

    void DoorLocker::unlock(const Key &key) {
        if (isLocked_ && key.getId() == id_)
            isLocked_ = false;
    }

    bool DoorLocker::isLocked() const {
        return isLocked_;
    }

    int DoorLocker::getId() const {
        return id_;
    }
}
