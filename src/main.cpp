#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "ASGraph.h"
#include "ASNode.h"
#include "Announcement.h"
#include "Policy.h"
#include "BGP.h"
#include "Prefix.h"
#include "ROV.h"


using std::cout, std::cin, std::endl;
int main(int argc, char* argv[]){
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <CAIDA> <anns> <rov_asns>\n";
        return 1;
    }
    std::string file_path = argv[1];
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_path << "\n";
        return 1;
    }
    std::string line;
    
    ASGraph g;
    
    while (std::getline(file, line)) {
        if(line.find('#') == std::string::npos){
            std::stringstream ss(line);
            std::string part;
            std::string a, b, c, d;
            std::getline(ss, a, '|');
            std::getline(ss, b, '|');
            std::getline(ss, c, '|');
            std::getline(ss, d, '|');
            if(std::stoi(c) == -1){
                g.add_pc(std::stoi(a), std::stoi(b));
            }
            else if(std::stoi(c) == 0){
                g.add_peer(std::stoi(a), std::stoi(b));
            }
            else if(std::stoi(c) == 1){
                g.add_pc(std::stoi(b), std::stoi(a));
            }
        }
    }

    if(g.checkCycles() == 0){
        cout << "No cycles detected." << endl;
    }else{
        cout << "Cycle detected." << endl;
        return 1;
    }
    cout << g.nodes.size() << endl;
    cout << g.nodes[1]->peers.size() << endl;
    



    for(auto& thing : g.nodes){
        ASNode* temp1 = thing.second.get();
        temp1->policy = std::make_unique<BGP>(temp1);
    }

    std::string file_path3 = argv[3];
    std::ifstream file3(file_path3);
    if (!file3.is_open()) {
        std::cerr << "Error: Could not open file: " << file_path3 << "\n";
        return 1;
    }
    std::string line3;
    
    while (std::getline(file3, line3)){
        ASNode* temp2 = g.getNode(std::stoi(line3));
        temp2->policy = std::make_unique<ROV>(temp2);
    }


    std::string file_path2 = argv[2];
    std::ifstream file2(file_path2);
    if (!file2.is_open()) {
        std::cerr << "Error: Could not open file: " << file_path2 << "\n";
        return 1;
    }
    std::string line2;
    std::getline(file2, line2);
    while (std::getline(file2, line2)) {
        std::stringstream ss(line2);
        std::string aa, bb, cc;
        std::getline(ss, aa, ',');
        std::getline(ss, bb, ',');
        std::getline(ss, cc, ',');
        cout << bb << endl;
        Prefix p = Prefix(bb);
        bool isROV;
        int asnum = stoi(aa);
        if(cc.find("True") != std::string::npos){
            isROV = true;
        }else{
            isROV = false;
        }
        Announcement ann69 = Announcement(p, asnum, isROV);
        g.getNode(asnum)->policy->newAnnouncement(ann69);


    }

    for(auto& a : g.nodes){
            if(a.second->customers.size() == 0){
                a.second->rank = 0;
            }
        }
    bool finished = false;
    int i = 0;
    while(!finished){
        finished = true;
        for(auto& a : g.nodes){
            if(a.second->rank == i){
                for(auto b : a.second->providers){
                    if(b->rank == -1 || b->rank > i+1){
                        b->rank = i+1;
                        finished = false;
                    }
                    
                }
            }
        }
        i++;
    }
    cout << g.getNode(1)->rank << endl;
    cout << g.getNode(149)->rank << endl;
    


    bool finished2 = false;
    int j = 0;
    while(!finished2){
        finished2 = true;
        for(auto& a : g.nodes){
            if(a.second->rank == j){
                a.second->policy->update();
                finished2 = false;
                a.second->policy->sendProviders();
            }
        }
        j++;
    }
    for(auto& a : g.nodes){
        a.second->policy->sendPeers();
    }
    j--;
    while(j >= 0){
        for(auto& a : g.nodes){
            if(a.second->rank == j){
                a.second->policy->update();
                a.second->policy->sendCustomers();
            }
        }
        j--;
    }

    std::ofstream file4("output.csv"); // open CSV file

    if (!file4.is_open()) {
        std::cerr << "Failed to open file.\n";
        return 1;
    }

    

    // Write rows
    
    file4 << "asn,prefix,as_path" << "\n";
    for(auto& e : g.nodes){
        for(auto& f : e.second->policy->getLocalRib()){
            file4 << e.first << "," << f.first.toString() << "," << f.second.path_string() << "\n";
        }
        
    }
    file4.close();
    std::cout << "CSV written successfully.\n";
    

    // g.add_pc(1, 2);
    // g.add_pc(2, 3);
    // g.add_pc(3, 4);
    // std::cout << g.checkCycles() << std::endl;
    // g.add_peer(4, 2);
    // g.add_peer(4, 1);
    // std::cout << g.checkCycles() << std::endl;
    // ASNode* node = g.getNode(1);
    // ASNode* node2 = g.getNode(2);
    // ASNode* node3 = g.getNode(3);
    // ASNode* node4 = g.getNode(4);
    // Prefix pre = Prefix("1.2.0.0/16");
    // Announcement a = Announcement(pre, 4);
    // cout << "Initial: " << a.path_string() << endl;
    // node->policy = std::make_unique<BGP>(node);
    // node2->policy = std::make_unique<BGP>(node2);
    // node3->policy = std::make_unique<BGP>(node3);
    // node4->policy = std::make_unique<BGP>(node4);
    // node4->policy->newAnnouncement(a);
    // node2->print_relationships();
    // node4->policy->sendPeers();
    // node2->policy->update();
    // node->policy->update();
    // node2->policy->sendProviders();
    // node->policy->update();
    // std::cout << node4->policy->getAnnouncement(pre).nextHop << std::endl;
    // std::cout << node4->policy->getAnnouncement(pre).path_string() << std::endl;
    // std::cout << node->policy->getAnnouncement(pre).nextHop << std::endl;
    // std::cout << node->policy->getAnnouncement(pre).path_string() << std::endl;
    // std::cout << node2->policy->getAnnouncement(pre).nextHop << std::endl;
    // std::cout << node2->policy->getAnnouncement(pre).path_string() << std::endl;

    return 0;
}