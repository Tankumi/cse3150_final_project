#pragma once
#include "Policy.h"
#include "Prefix.h"
#include "Announcement.h"
#include <vector>
#include <string>
#include <unordered_map>

class BGP : public Policy{
public:
    std::unordered_map<Prefix, Announcement> local_rib;
    std::unordered_map<Prefix, std::vector<Announcement>> receivedQueue;
    BGP(ASNode* o) {
        owner = o;
    };
    BGP(const std::unordered_map<Prefix, Announcement>& l, const std::unordered_map<Prefix, std::vector<Announcement>>& q){
        local_rib = l;
        receivedQueue = q;
    }
    void update() override;
    std::unordered_map<Prefix, Announcement> getLocalRib();
    void newAnnouncement(const Announcement& a) override;
    Announcement getAnnouncement(const Prefix& pre) override;
    Announcement best(const Prefix& pre) override;
    void receive(const Announcement& a) override;
    void sendPeers() override;
    void sendProviders() override;
    void sendCustomers() override;
    ~BGP() = default;
};