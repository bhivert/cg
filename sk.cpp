#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>

using namespace std;

typedef int                     node_t;
typedef vector<set<node_t>>     network_t;
typedef set<node_t>             backdoors_t;
typedef struct { node_t nodes[2]; }    link_t;
 
link_t  nearest_link(network_t &nw, backdoors_t &bds, node_t &skynet) {
    queue<link_t>   fifo;
    vector<bool>    marks(nw.size(), false);
    
    fifo.push({skynet, skynet});
    marks[skynet] = true;
    
    while (!fifo.empty()) {
        node_t  node = fifo.front().nodes[0];
        fifo.pop();
        for (auto it = nw[node].begin(); it != nw[node].end(); ++it) {
            if (bds.find(*it) != bds.end())
                return (link_t){*it, node};
            if (!marks[*it]) {
                fifo.push({*it, node});
                marks[*it] = true;
            }
        }
    }
    return (link_t){-1, -1};
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
    network_t nw(N, set<node_t>());
    backdoors_t bds;
    
    for (int i = 0; i < L; i++) {
        int N1, N2; // N1 and N2 defines a link between these nodes
        cin >> N1 >> N2; cin.ignore();
        nw[N1].insert(N2);
        nw[N2].insert(N1);
    }

    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
        bds.insert(EI);
    }
    
    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;

        link_t link = nearest_link(nw, bds, SI);

        // Example: 0 1 are the indices of the nodes you wish to sever the link between
        cout << link.nodes[0] << ' ' << link.nodes[1] << endl;
    }
}
