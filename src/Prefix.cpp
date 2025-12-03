#include "Prefix.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <functional>

static std::vector<std::string> split(const std::string& s, char c) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, c))
        out.push_back(item);

    return out;
}

Prefix::Prefix(const std::string& p){
    auto pos = p.find('/');
    if (pos == std::string::npos)
        throw std::invalid_argument("Invalid prefix" + p);
    length = std::stoi(p.substr(pos+1));
    std::string first = p.substr(0, pos);
    if(first.find('.') != std::string::npos){
        type = 0;
        auto parts = split(first, '.');
        if(parts.size() != 4){
            throw std::runtime_error("invalid address");
        }
        for(int i = 0; i < 4; i++){
            int num = std::stoi(parts[i]);
            if(num > 255 || num < 0){
                throw std::runtime_error("invalid address");
            }
            bytes[i] = static_cast<uint8_t>(num);
        }

    }
    if(first.find(':') != std::string::npos){
        type = 1;
        auto parts = split(first, ':');
        if(parts.size() != 8){
            throw std::runtime_error("invalid address");
        }
        for(int i = 0; i < 8; i++){
            int val = std::stoi(parts[i], nullptr, 16);
            bytes[2*i] = (val >> 8) & 0xFF;
            bytes[2*i+1] = val & 0xFF;
        }
        
    }
    
}

bool Prefix::operator==(const Prefix& other) const {
    return type == other.type &&
           length == other.length &&
           bytes == other.bytes;
}


std::string Prefix::ipv4ToString() const {
    std::ostringstream oss;
    oss << (int)bytes[0] << "."
        << (int)bytes[1] << "."
        << (int)bytes[2] << "."
        << (int)bytes[3];
    return oss.str();
}
std::string Prefix::ipv6ToString() const {
    std::ostringstream oss;
    for(int i = 0; i < 16; i += 2) {
        int value = (bytes[i] << 8) | bytes[i+1]; // combine high and low byte
        oss << std::hex << std::setw(4) << std::setfill('0') << value;
        if(i < 14) oss << ":"; // add ':' between groups
    }
    return oss.str();
}


std::string Prefix::toString() const {
    if (type == 0)
        return ipv4ToString() + "/" + std::to_string(length);
    return ipv6ToString() + "/" + std::to_string(length);
}

