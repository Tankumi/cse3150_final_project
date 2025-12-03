#pragma once
#include <cstdint>
#include <string>
#include <array>



class Prefix{
public:
    bool type; //0 for v4, 1 for v6
    uint8_t length;
    std::array<uint8_t, 16> bytes;

    Prefix(): type(0), length(0){};
    Prefix(const std::string& p);
    Prefix(const Prefix& other){
        bytes = other.bytes;
        type = other.type;
        length = other.length;
    }
    Prefix(Prefix&& other) = default;
    Prefix& operator=(const Prefix&) = default;
    Prefix& operator=(Prefix&&) = default;
    bool operator==(const Prefix& other) const;
    ~Prefix() = default;
    std::string ipv4ToString() const;
    std::string ipv6ToString() const;
    std::string toString() const;
};


namespace std {
    template<>
    struct hash<Prefix> {
        std::size_t operator()(const Prefix& p) const {
            // simple hash combining type, length, and first few bytes
            std::size_t h = std::hash<int>()(p.type);
            h ^= std::hash<int>()(p.length) << 1;
            for (int i = 0; i < 4; ++i) {  // combine first 4 bytes
                h ^= std::hash<uint8_t>()(p.bytes[i]) << (i * 2);
            }
            return h;
        }
    };
}