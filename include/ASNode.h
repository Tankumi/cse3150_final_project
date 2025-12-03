#pragma once
#include <cstdint>
#include <vector>
#include <memory>
#include "Policy.h"

class Policy;

class ASNode{
public:
    uint32_t asn;
    std::vector<ASNode*> providers;
    std::vector<ASNode*> customers;
    std::vector<ASNode*> peers;
    int state;


    std::unique_ptr<Policy> policy;

    int rank;
    explicit ASNode(uint32_t n): asn(n), rank(-1), state(0) {};

    void print_relationships() const;
};
