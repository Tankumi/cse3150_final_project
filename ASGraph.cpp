#include "ASGraph.h"
#include <stack>
#include <iostream>

ASGraph* ASGraph::getNode(uint32_t asn){
    auto n = nodes.find(asn)
    if(n != nodes.end()){
        return n->second.get();
    }
    else{
        nodes[asn] = std::make_unique<ASNode>(asn);
        return nodes[asn].get();
    }
}