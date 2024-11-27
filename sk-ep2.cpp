#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>

using namespace std;

typedef int                     node_t;
typedef vector<set<node_t>>     network_t;
typedef set<node_t>             backdoors_t;

typedef struct { node_t node; node_t father; size_t dist; double weight; }    link_t;
typedef struct { double weight; list<link_t> links; }    heuristic_t;
 
map<node_t, heuristic_t>  backdoors_weight(network_t &nw, backdoors_t &bds, vector<size_t> &weight, node_t &skynet, list<link_t> &nearest_links) {
    queue<link_t>               fifo;
    vector<bool>                marks(nw.size(), false);
    map<node_t, heuristic_t>    rsl;
    
    nearest_links.clear();
    fifo.push({skynet, skynet, 0, 0});
    marks[skynet] = true;
        
    while (!fifo.empty()) {
        link_t  tmp = fifo.front();
        size_t  children_dist = tmp.dist + 1;

        fifo.pop();
        for (auto &child : nw[tmp.node]) {
            if (marks[child])
                continue ;
            if (bds.find(child) != bds.end()) {
                double child_weight = (weight[child] + tmp.weight) / children_dist;
                map<node_t, heuristic_t>::iterator found = rsl.find(child);
                if (found != rsl.end())
                    rsl[child].weight = (rsl[child].weight + child_weight) / 2;
                else
                    rsl[child].weight = child_weight;
                rsl[child].links.push_back({ child, tmp.node, children_dist, child_weight });
                nearest_links.push_back({ child, tmp.node, children_dist, child_weight });
                continue ;
            }
            marks[child] = true;
            fifo.push({ child, tmp.node, children_dist, weight[child] + tmp.weight });
        }
    }
    return(rsl);
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways

    cin >> N >> L >> E; cin.ignore();
    network_t       network(N, set<node_t>());
    backdoors_t     backdoors;
    vector<size_t>  weight(N, 0);
    vector<size_t>  heuristic;
    
    for (int i = 0; i < L; ++i) {
        int N1, N2; // N1 and N2 defines a link between these nodes
        cin >> N1 >> N2; cin.ignore();
        network[N1].insert(N2);
        network[N2].insert(N1);
    }

    for (int i = 0; i < E; ++i) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
        backdoors.insert(EI);
        weight[EI] = 1;
        for (auto &child : network[EI]) {
            weight[child] += 1;
        }
    }
    
    heuristic = weight;
    for (auto &bd : backdoors) {
        for (auto &direct_link : network[bd]) {
            for (auto &link : network[direct_link]) {
                if (heuristic[link] > 0)
                    heuristic[direct_link] += 1;
            }
        }
    }

    // game loop
    for (;;) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();
        stringstream out;

        list<link_t>    nearest_links;
        map<node_t, heuristic_t> bd_heuristics = backdoors_weight(network, backdoors, heuristic, SI, nearest_links);

        link_t  tmp = *nearest_links.begin();
        if (tmp.dist > 1) {
            heuristic_t htmp = (*bd_heuristics.begin()).second;
            for (auto &h : bd_heuristics) {
                if (h.second.weight > htmp.weight)
                    htmp = h.second;
                    cerr << h.first << ":\t" << h.second.weight << '|' << heuristic[h.first] << endl;
            }
            tmp = *htmp.links.begin();
            for (auto &l : htmp.links) {
                if (heuristic[l.father] > heuristic[tmp.father])
                    tmp = l;
                    cerr << '-' << l.father << ":\t" << l.weight << '|' << heuristic[l.father] << endl;
            }
        }

        out << tmp.node << ' ' << tmp.father << endl;
        cout << out.str();
        network[tmp.node].erase(tmp.father);
        network[tmp.father].erase(tmp.node);
        --weight[tmp.father];
        
        heuristic = weight;
        for (auto &bd : backdoors) {
            for (auto &direct_link : network[bd]) {
                for (auto &link : network[direct_link]) {
                    if (heuristic[link] > 0)
                        heuristic[direct_link] += 1;
                }
            }
        }
    }
}