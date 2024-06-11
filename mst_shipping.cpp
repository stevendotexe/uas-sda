#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string>

using namespace std;

struct Edge
{
  int from;
  int to;
  int cost;
  int distance;
};

struct Subset
{
  int parent;
  int rank;
};

int find(Subset subsets[], int i)
{
  if (subsets[i].parent != i)
  {
    subsets[i].parent = find(subsets, subsets[i].parent);
  }
  return subsets[i].parent;
}

void unionSets(Subset subsets[], int x, int y)
{
  int rootX = find(subsets, x);
  int rootY = find(subsets, y);

  if (subsets[rootX].rank < subsets[rootY].rank)
  {
    subsets[rootX].parent = rootY;
  }
  else if (subsets[rootX].rank > subsets[rootY].rank)
  {
    subsets[rootY].parent = rootX;
  }
  else
  {
    subsets[rootY].parent = rootX;
    subsets[rootX].rank++;
  }
}

vector<Edge> kruskalMST(vector<Edge> &edges, int V)
{
  vector<Edge> result;
  int e = 0;
  size_t i = 0;

  sort(edges.begin(), edges.end(), [](Edge a, Edge b)
       { return a.distance < b.distance; });

  Subset *subsets = new Subset[V];
  for (int v = 0; v < V; ++v)
  {
    subsets[v].parent = v;
    subsets[v].rank = 0;
  }

  while (e < V - 1 && i < edges.size())
  {
    Edge nextEdge = edges[i++];

    int x = find(subsets, nextEdge.from);
    int y = find(subsets, nextEdge.to);

    if (x != y)
    {
      result.push_back(nextEdge);
      unionSets(subsets, x, y);
      e++;
    }
  }

  delete[] subsets;
  return result;
}

int calculatePrice(const string &from, const string &to, int distance)
{
  if ((from == "BALI" && to == "MADURA") || (from == "MADURA" && to == "BALI"))
  {
    return 720 * distance;
  }
  else if (from == "BALI" || from == "MADURA" || to == "BALI" || to == "MADURA")
  {
    return 360 * distance;
  }
  else
  {
    return 120 * distance;
  }
}

int main()
{
  ifstream file("C:\\Users\\afkar\\OneDrive\\Documents\\.RPL\\SDA\\Pertemuan 16 - UAS\\uas-sda\\data\\Jarak Rute.csv");
  if (!file.is_open())
  {
    cerr << "Error opening file" << endl;
    return 1;
  }

  string line;
  vector<Edge> edges;
  unordered_map<string, int> cityIndex;
  unordered_map<int, string> indexCity;
  int index = 0;

  // Skip header
  getline(file, line);

  while (getline(file, line))
  {
    stringstream ss(line);
    string from, to, cost, distance, note;

    getline(ss, from, ',');
    getline(ss, to, ',');
    getline(ss, cost, ',');
    getline(ss, distance, ',');
    getline(ss, note, ',');

    if (cityIndex.find(from) == cityIndex.end())
    {
      cityIndex[from] = index;
      indexCity[index] = from;
      index++;
    }
    if (cityIndex.find(to) == cityIndex.end())
    {
      cityIndex[to] = index;
      indexCity[index] = to;
      index++;
    }

    edges.push_back({cityIndex[from], cityIndex[to], stoi(cost), stoi(distance)});
  }

  file.close();

  int V = cityIndex.size();
  vector<Edge> mst = kruskalMST(edges, V);

  ofstream outputFile("C:\\Users\\afkar\\OneDrive\\Documents\\.RPL\\SDA\\Pertemuan 16 - UAS\\uas-sda\\data\\MST Result.csv");
  if (!outputFile.is_open())
  {
    cerr << "Error opening output file" << endl;
    return 1;
  }

  outputFile << "FROM,TO,DISTANCE,PRICE\n";
  for (auto edge : mst)
  {
    string from = indexCity[edge.from];
    string to = indexCity[edge.to];
    int price = calculatePrice(from, to, edge.distance);
    outputFile << from << "," << to << "," << edge.distance << "," << price << "\n";
  }

  outputFile.close();

  cout << 'File has been generated successfully in "data/MST Result"' << endl;

  return 0;
}
