#include "BGP.h"
#include "Announcement.h"
#include <stdexcept>
#include <iostream>


//Adds announcement received to the queue
void BGP::newAnnouncement(const Announcement& a) {
    local_rib[a.prefix] = a;
}


//Triggers update, shortest path for each Prefix gets put in local_rib, rest are deleted for space savings
void BGP::update(){
    
    for(const auto& it : receivedQueue){
        Announcement queueBest = best(it.first);
        if(!local_rib.count(it.first)) {
            local_rib[it.first] = queueBest;
            local_rib[it.first].prepend(owner->asn);
            local_rib[it.first].nextHop = owner->asn;
        }
        else if(queueBest.relationship == -1){
            local_rib[it.first] = queueBest;
            local_rib[it.first].prepend(owner->asn);
            local_rib[it.first].nextHop = owner->asn;
        }
        else if(queueBest.relationship < local_rib[it.first].relationship){
            local_rib[it.first] = queueBest;
            local_rib[it.first].prepend(owner->asn);
            local_rib[it.first].nextHop = owner->asn;
        }
        else if(queueBest.relationship == local_rib[it.first].relationship && queueBest.ASPath.size() < local_rib[it.first].ASPath.size()){
            local_rib[it.first] = queueBest;
            local_rib[it.first].prepend(owner->asn);
            local_rib[it.first].nextHop = owner->asn;
        }
        else if(queueBest.relationship == local_rib[it.first].relationship && queueBest.ASPath.size() == local_rib[it.first].ASPath.size() && queueBest.nextHop < local_rib[it.first].nextHop){
            local_rib[it.first] = queueBest;
            local_rib[it.first].prepend(owner->asn);
            local_rib[it.first].nextHop = owner->asn;
        }
        
    }

    receivedQueue.clear();
}


//Finds the best path for a certain prefix in the queue
Announcement BGP::best(const Prefix& pre) {
    if(receivedQueue.find(pre) == receivedQueue.end() || receivedQueue[pre].empty()) {
        throw std::runtime_error("No announcements for this prefix");
    }

    Announcement bestAnn = receivedQueue[pre][0];
    for(const auto& a : receivedQueue[pre]) {
        if(a.relationship == -1) {
            bestAnn = a;
        }
        else if(a.relationship < bestAnn.relationship){
            bestAnn = a;
        }
        else if(a.relationship == bestAnn.relationship && a.ASPath.size() < bestAnn.ASPath.size()){
            bestAnn = a;
        }
        else if(a.relationship == bestAnn.relationship && a.ASPath.size() == bestAnn.ASPath.size() && a.nextHop < bestAnn.nextHop){
            bestAnn = a;
        }
    }
    return bestAnn;
}

std::unordered_map<Prefix, Announcement> BGP::getLocalRib(){
    return local_rib;
}

void BGP::receive(const Announcement& a){
    receivedQueue[a.prefix].push_back(a);
}
void BGP::sendPeers(){
    for(ASNode* p : owner->peers){
        for(const auto& kv : local_rib){
            Announcement a = kv.second;
            a.nextHop = owner->asn;
            if(a.relationship == -1){
                a.relationship = 1;
                p->policy->receive(a);
            }
            a.relationship = 1;
            p->policy->receive(a.prepend(owner->asn));
        }
    }
}
void BGP::sendProviders(){
    for(ASNode* p : owner->providers){
        for(const auto& kv : local_rib){
            Announcement a = kv.second;
            a.nextHop = owner->asn;
            if(a.relationship == -1){
                a.relationship = 0;
                p->policy->receive(a);
            }
            a.relationship = 0;
            p->policy->receive(a.prepend(owner->asn));
            
        }
    }
}
void BGP::sendCustomers(){
    for(ASNode* p : owner->customers){
        for(const auto& kv : local_rib){
            Announcement a = kv.second;
            a.nextHop = owner->asn;
            if(a.relationship == -1){
                a.relationship = 2;
                p->policy->receive(a);
            }
            a.relationship = 2;
            p->policy->receive(a.prepend(owner->asn));
            
        }
    }
}

Announcement BGP::getAnnouncement(const Prefix& pre){
    return local_rib[pre];
}