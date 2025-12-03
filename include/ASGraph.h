#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "ASNode.h"

class ASGraph{
public:
    std::unordered_map<uint32_t, std::unique_ptr<ASNode>> nodes;
    ASGraph() = default;
    ASNode* getNode(uint32_t asn);
    void add_peer(uint32_t asn1, uint32_t asn2);
    void add_pc(uint32_t provider, uint32_t customer);
    bool checkCycles();
    bool cycleHelper(ASNode* node);

};