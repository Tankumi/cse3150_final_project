#pragma once
#include <unordered_map>
#include "Announcement.h"
#include "Prefix.h"
#include "ASNode.h"

class ASNode;

class Policy{
public:
    ASNode* owner;

    virtual Announcement best(const Prefix& prefix) = 0;
    virtual std::unordered_map<Prefix, Announcement> getLocalRib() = 0;
    virtual Announcement getAnnouncement(const Prefix& pre) = 0;
    virtual void newAnnouncement(const Announcement& a) = 0;
    //virtual void insertLocalRib(const Announcement& a) = 0;
    virtual void receive(const Announcement& a) = 0;
    virtual void update() = 0;
    virtual void sendPeers() = 0;
    virtual void sendProviders() = 0;
    virtual void sendCustomers() = 0;
    virtual ~Policy() = default;


};