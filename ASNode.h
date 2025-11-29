#pragma once
#include <cstdint>
#include <vector>
#include <memory>


class ASNode{
public:
    uint32_t asn;
    std::vector<ASNode*> providers;
    std::vector<ASNode*> customers;
    std::vector<ASNode*> peers;


    //policy stuff implement later

    int rank;
    explicit ASNode(uint32_t n): asn(n), rank(-1){};

    void print_relationships() const;
};
