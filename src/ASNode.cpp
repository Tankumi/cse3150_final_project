#include "ASNode.h"
//#include "Policy.h"
#include <algorithm>
#include <iostream>


void ASNode::print_relationships() const {
    std::cout << "AS " << asn << ":\n";

    std::cout << "  Providers: ";
    for (auto* p : providers)
        std::cout << p->asn << " ";
    std::cout << "\n";

    std::cout << "  Customers: ";
    for (auto* c : customers)
        std::cout << c->asn << " ";
    std::cout << "\n";

    std::cout << "  Peers: ";
    for (auto* peer : peers)
        std::cout << peer->asn << " ";
    std::cout << "\n";
}