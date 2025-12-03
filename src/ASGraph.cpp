#include "ASGraph.h"
#include <stack>
#include <iostream>

ASNode* ASGraph::getNode(uint32_t asn){
    auto n = nodes.find(asn);
    if(n != nodes.end()){
        return n->second.get();
    }
    else{
        nodes[asn] = std::make_unique<ASNode>(asn);
        return nodes[asn].get();
    }
}

void ASGraph::add_peer(uint32_t asn1, uint32_t asn2){
    ASNode* a = getNode(asn1);
    ASNode* b = getNode(asn2);
    a->peers.push_back(b);
    b->peers.push_back(a);
    
    
}

void ASGraph::add_pc(uint32_t provider, uint32_t customer){
    ASNode* p = getNode(provider);
    ASNode* c = getNode(customer);
    p->customers.push_back(c);
    c->providers.push_back(p);
}

bool ASGraph::checkCycles(){
    for(auto& pair : nodes){
        (pair.second)->state = 0;
    }
    for(auto& pair : nodes){
        if(cycleHelper(pair.second.get())){
            return true;
        }
    }
    return false;
}
bool ASGraph::cycleHelper(ASNode *node){
    if(node->state == 1){
        return true;
    }
    if(node->state == 0){
        node->state = 1;
        for(ASNode* n : node->customers){
            if(cycleHelper(n)){
                return true;
            }
        }
    }
    node->state = 2;
    return false;
}