#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <array>
#include "Prefix.h"

class Announcement{
public:
    uint32_t nextHop;
    std::vector<uint32_t> ASPath;
    int relationship; //1 for peer, 2 if came from a provider, 0 if came from a customer, -1 if origin
    Prefix prefix;
    bool rov_invalid;
    
    Announcement(): nextHop(-1), relationship(-1), rov_invalid(false){};
    Announcement(Prefix& pre, uint32_t n, std::vector<uint32_t> p, int r) : prefix(pre), nextHop(n), ASPath(p), relationship(r), rov_invalid(false){};
    Announcement(Prefix& pre, uint32_t n, std::vector<uint32_t> p, int r, bool i) : prefix(pre), nextHop(n), ASPath(p), relationship(r), rov_invalid(i){};
    Announcement(Prefix& pre, uint32_t n) : prefix(pre), nextHop(n), relationship(-1), ASPath({n}), rov_invalid(false){};
    Announcement(Prefix& pre, uint32_t n, bool i) : prefix(pre), nextHop(n), relationship(-1), ASPath({n}), rov_invalid(i){};
    Announcement(const Announcement& other) = default;
    Announcement(Announcement&& other) = default;
    Announcement& operator=(const Announcement&) = default;
    Announcement& operator=(Announcement&&) = default;
    Announcement prepend(uint32_t asn) const;
    std::string path_string() const;
};