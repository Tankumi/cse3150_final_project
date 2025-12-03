#include "ROV.h"

void ROV::receive(const Announcement& a) {
    if (a.rov_invalid) {
        return;
    }

    BGP::receive(a);
}