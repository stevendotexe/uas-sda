#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>

using namespace std;

struct Edge {
    string source, destination;
    int weight;
};

class Graph {
public:
    vector<Edge> edges;

    void addEdge(string source, string destination, int weight) {
        Edge edge;
        edge.source = source;
        edge.destination = destination;
        edge.weight = weight;
        edges.push_back(edge);
    }

    int primMST(string start, string end) {
        unordered_map<string, vector<pair<string, int>>> adjList;
        for (const auto &edge : edges) {
            adjList[edge.source].push_back({edge.destination, edge.weight});
            adjList[edge.destination].push_back({edge.source, edge.weight});
        }

        unordered_map<string, bool> visited;
        unordered_map<string, string> parent;
        unordered_map<string, int> key;

        for (const auto &edge : edges) {
            visited[edge.source] = false;
            visited[edge.destination] = false;
            parent[edge.source] = "";
            parent[edge.destination] = "";
            key[edge.source] = INT_MAX;
            key[edge.destination] = INT_MAX;
        }

        key[start] = 0;
        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        pq.push({0, start});

        while (!pq.empty()) {
            string u = pq.top().second;
            pq.pop();

            if (u == end)
                break;

            if (visited[u])
                continue;

            visited[u] = true;

            for (const auto &neighbor : adjList[u]) {
                string v = neighbor.first;
                int weight = neighbor.second;
                if (!visited[v] && weight < key[v]) {
                    parent[v] = u;
                    key[v] = weight;
                    pq.push({key[v], v});
                }
            }
        }

        int mstWeight = 0;
        string vertex = end;
        // cout << "Minimum Spanning Tree from " << start << " to " << end << ":" << endl;
        while (parent[vertex] != "") {
            // cout << parent[vertex] << " -- " << vertex << endl;
            mstWeight += key[vertex];
            vertex = parent[vertex];
        }
        
        return mstWeight;
    }
};

int getRange(string destination) {
    Graph graph;

    // Adding edges to the graph
    graph.addEdge("JABODETABEK", "SERANG", 83);
    graph.addEdge("JABODETABEK", "PURWAKARTA", 100);
    graph.addEdge("JABODETABEK", "INDRAMAYU", 203);
    graph.addEdge("JABODETABEK", "SUKABUMI", 119);
    graph.addEdge("BANDUNG", "SUKABUMI", 97);
    graph.addEdge("BANDUNG", "PURWAKARTA", 62);
    graph.addEdge("CIREBON", "INDRAMAYU", 55);
    graph.addEdge("CIREBON", "BANDUNG", 141);
    graph.addEdge("CIREBON", "PURWAKARTA", 152);
    graph.addEdge("CIREBON", "TASIKMALAYA", 109);
    graph.addEdge("CIREBON", "TEGAL", 79);
    graph.addEdge("TEGAL", "PURWOKERTO", 106);
    graph.addEdge("TEGAL", "PEKALONGAN", 90);
    graph.addEdge("PEKALONGAN", "SEMARANG", 93);
    graph.addEdge("SOLO", "SEMARANG", 107);
    graph.addEdge("SOLO", "YOGYAKARTA", 65);
    graph.addEdge("SOLO", "NGAWI", 83);
    graph.addEdge("NGAWI", "MADIUN", 38);
    graph.addEdge("SURABAYA", "NGAWI", 183);
    graph.addEdge("SURABAYA", "MALANG", 96);
    graph.addEdge("SURABAYA", "PULAU MADURA", 98);
    graph.addEdge("BANYUWANGI", "MALANG", 311);
    graph.addEdge("BANYUWANGI", "BALI", 138);

    int range = graph.primMST("JABODETABEK", destination);
    return 0;
}
