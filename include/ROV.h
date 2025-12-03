#pragma once
#include "Policy.h"
#include "Prefix.h"
#include "Announcement.h"
#include "BGP.h"
#include <vector>
#include <string>
#include <unordered_map>

class ROV : public BGP {
public:
    using BGP::BGP;   // inherit constructor

    void receive(const Announcement& a) override;
    ~ROV() = default;
};