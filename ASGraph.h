#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>

class ASGraph{
public:
    std::unordered_map<uint32_t, std::unique_ptr<ASNode>> nodes;
    ASGraph() = default;
    ASGraph* getNode(uint32_t asn);

};