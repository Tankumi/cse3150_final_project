#include "BGP.h"
#include "Announcement.h"
#include <stdexcept>
#include <iostream>


//Adds origin announcement
void BGP::newAnnouncement(const Announcement& a) {
    local_rib[a.prefix] = a;
}


//Triggers update, best path for each Prefix gets put in local_rib, rest are deleted for space savings
void BGP::update(){
    
    for(const auto& it : receivedQueue){
        Announcement queueBest = best(it.first);
        if(!local_rib.count(it.first)) {
            local_rib[it.first] = queueBest;
        }
        else if(queueBest.relationship < local_rib[it.first].relationship){
            local_rib[it.first] = queueBest;
        }
        else if(queueBest.relationship == local_rib[it.first].relationship && queueBest.ASPath.size() < local_rib[it.first].ASPath.size()){
            local_rib[it.first] = queueBest;
        }
        else if(queueBest.relationship == local_rib[it.first].relationship && queueBest.ASPath.size() == local_rib[it.first].ASPath.size() && queueBest.nextHop < local_rib[it.first].nextHop){
            local_rib[it.first] = queueBest;
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
        if(a.relationship < bestAnn.relationship){
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

std::unordered_map<Prefix, Announcement>& BGP::getLocalRib(){
    return local_rib;
}

void BGP::receive(const Announcement& a){
    receivedQueue[a.prefix].push_back(a.prepend(owner->asn));
}
void BGP::sendPeers(){
    for(ASNode* p : owner->peers){
        for(const auto& kv : local_rib){
            Announcement a = kv.second;
            a.nextHop = owner->asn;
            a.relationship = 1;
            p->policy->receive(a);
        }
    }
}
void BGP::sendProviders(){
    for(ASNode* p : owner->providers){
        for(const auto& kv : local_rib){
            Announcement a = kv.second;
            a.nextHop = owner->asn;
            a.relationship = 0;
            p->policy->receive(a);
            
        }
    }
}
void BGP::sendCustomers(){
    for(ASNode* p : owner->customers){
        if(p->asn == 51935){
            std::cout << "eeeeeeeeeeeeeeeeeeeee" << std::endl;
            std::cout << "localrib: " << local_rib.size() << std::endl;
        }
        for(const auto& kv : local_rib){
            if(p->asn == 51935){
                std::cout << "eeeeeeeeeeeeeeeeeeeee" << std::endl;
            }
            Announcement a = kv.second;
            a.nextHop = owner->asn;
            a.relationship = 2;
            p->policy->receive(a);
            
        }
    }
}

Announcement BGP::getAnnouncement(const Prefix& pre){
    return local_rib[pre];
}