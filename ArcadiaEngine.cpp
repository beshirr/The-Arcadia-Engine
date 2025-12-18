// ArcadiaEngine.cpp - STUDENT TEMPLATE
// TODO: Implement all the functions below according to the assignment requirements

#include "ArcadiaEngine.h"
#include <algorithm>
#include <bitset>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <set>

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---

class ConcretePlayerTable : public PlayerTable {
private:
    // TODO: Define your data structures here
    // Hint: You'll need a hash table with double hashing collision resolution

public:
    ConcretePlayerTable() {
        // TODO: Initialize your hash table
    }

    void insert(int playerID, string name) override {
        // TODO: Implement double hashing insert
        // Remember to handle collisions using h1(key) + i * h2(key)
    }

    string search(int playerID) override {
        // TODO: Implement double hashing search
        // Return "" if player not found
        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---

class ConcreteLeaderboard : public Leaderboard {
private:
    // TODO: Define your skip list node structure and necessary variables
    // Hint: You'll need nodes with multiple forward pointers

public:
    ConcreteLeaderboard() {
        // TODO: Initialize your skip list
    }

    void addScore(int playerID, int score) override {
        // TODO: Implement skip list insertion
        // Remember to maintain descending order by score
    }

    void removePlayer(int playerID) override {
        // TODO: Implement skip list deletion
    }

    vector<int> getTopN(int n) override {
        // TODO: Return top N player IDs in descending score order
        return {};
    }
};

// --- 3. AuctionTree (Red-Black Tree) ---

class ConcreteAuctionTree : public AuctionTree {
private:
    // TODO: Define your Red-Black Tree node structure
    // Hint: Each node needs: id, price, color, left, right, parent pointers

public:
    ConcreteAuctionTree() {
        // TODO: Initialize your Red-Black Tree
    }

    void insertItem(int itemID, int price) override {
        // TODO: Implement Red-Black Tree insertion
        // Remember to maintain RB-Tree properties with rotations and recoloring
    }

    void deleteItem(int itemID) override {
        // TODO: Implement Red-Black Tree deletion
        // This is complex - handle all cases carefully
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    // TODO: Implement partition problem using DP
    // Goal: Minimize |sum(subset1) - sum(subset2)|
    // Hint: Use subset sum DP to find closest sum to total/2
    const int total = accumulate(coins.begin(), coins.end(), 0);
    const int t = total / 2;
    vector dp(t + 1, false);
    dp[0] = true;

    for (const int c : coins) {
        for (int s = t; s >= c; s--) {
            dp[s] = dp[s] || dp[s - c];
        }
    }
    for (int s = t; s >= 0; s--) {
        if (dp[s]) {
            return abs(total - 2 * s);
        }
    }
    return INT_MAX;
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    // TODO: Implement 0/1 Knapsack using DP
    // items = {weight, value} pairs
    // Return maximum value achievable within capacity
    if (capacity <= 0 || items.empty()) return 0;

    vector dp(capacity + 1, 0);

    for (const auto& [weight, value] : items) {
        if (weight <= 0) continue;
        if (weight > capacity) continue;
        for (int w = capacity; w >= weight; --w) {
            dp[w] = max(dp[w], dp[w - weight] + value);
        }
    }
    return dp[capacity];
}

long long InventorySystem::countStringPossibilities(string s) {
    // TODO: Implement string decoding DP
    // Rules: "uu" can be decoded as "w" or "uu"
    //        "nn" can be decoded as "m" or "nn"
    // Count total possible decodings
    const int n = static_cast<int>(s.size());
    vector<long long> dp(n + 1, 0);
    dp[0] = 1;

    for (int i = 1; i <= n; i++) {
        dp[i] = dp[i - 1];

        if (i >= 2) {
            const char a = s[i - 2];
            const char b = s[i - 1];
            if (a == b && (b == 'u' || b == 'n')) {
                dp[i] += dp[i - 2];
            }
        }
    }
    return dp[n];
}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {

    vector<vector<int>> adjList(n);


    for (const auto& edge : edges) {
        if (edge[0] >= 0 && edge[0] < n && edge[1] >= 0 && edge[1] < n) {
            adjList[edge[0]].push_back(edge[1]);
            adjList[edge[1]].push_back(edge[0]);
        }
    }
    vector<bool> visited(n, false);


    queue<int> q;
    q.push(source);
    visited[source] = true;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == dest) {
            return true;
        }

        for (int neighbor : adjList[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    return false;

}

class UnionFind {
private:
    vector<int> parent, rank;

public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);  // Path compression
        return parent[x];
    }

    void unionSets(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX != rootY) {
            // Union by rank
            if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
        }
    }
};


long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData) {
    // TODO: Implement Minimum Spanning Tree (Kruskal's or Prim's)
    // roadData[i] = {u, v, goldCost, silverCost}
    // Total cost = goldCost * goldRate + silverCost * silverRate
    // Return -1 if graph cannot be fully connected
    vector<vector<long long>> edges;
    for (const auto& road : roadData) {
        int u = road[0];
        int v = road[1];
        long long goldCost = road[2];
        long long silverCost = road[3];

        long long totalCost = goldCost * goldRate + silverCost * silverRate;

        edges.push_back({totalCost, u, v});
    }

    sort(edges.begin(), edges.end());

    UnionFind uf(n);
    long long mstCost = 0;
    int edgesInMST = 0;

    for (const auto& edge : edges) {
        long long totalCost = edge[0];
        int u = edge[1];
        int v = edge[2];

        if (uf.find(u) != uf.find(v)) {
            uf.unionSets(u, v);
            mstCost += totalCost;
            edgesInMST++;
        }

        if (edgesInMST == n - 1) break;
    }

    if (edgesInMST == n - 1) {
        return mstCost;
    } else {
        return -1;
    }
}

string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    // TODO: Implement All-Pairs Shortest Path (Floyd-Warshall)
    // Sum all shortest distances between unique pairs (i < j)
    // Return the sum as a binary string
    // Hint: Handle large numbers carefully
    // Step 1: Initialize the distance matrix with infinity (INT_MAX) for unconnected cities
    // Step 1: Initialize the adjacency list for the graph
    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));





    for (int i = 0; i < n; ++i) {
        dist[i][i] = 0;
    }
    for (const auto& road : roads) {
        int u = road[0];
        int v = road[1];
        int distance = road[2];
        dist[u][v] = min(dist[u][v], distance);
        dist[v][u] = min(dist[v][u], distance);
    }




    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }



    long long totalDistance = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (dist[i][j] != INT_MAX) {
                totalDistance += dist[i][j];
            }
        }
    }

    string binarySum = bitset<64>(totalDistance).to_string();

    size_t firstOnePos = binarySum.find('1');

    if (firstOnePos == string::npos) {
        return "0";
    }

    return binarySum.substr(firstOnePos);

}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int find (const vector<char>& container, const char target) {
    if (container.empty()) return -1;
    for (int i = 0; i < container.size(); ++i) {
        if (container[i] == target) return i;
    }
    return -1;
}


int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // handles edge case of one task, the cooling time won't affect the output
    if (tasks.size() == 1) return 1;
    // handles edge case of no cooling time, processes order does not matter
    if (n == 0) return tasks.size();
    map<char, int> processes;
    vector<int> freq;
    for (char task : tasks) {
        processes[task]++;
    }
    for (auto p : processes) {
        freq.push_back(p.second);
    }
    int numDistinct = freq.size();
    // handles edge case of number of distinct values greater than the cooling gaps.
    if (numDistinct >= n+1) return tasks.size();
    sort(freq.begin(), freq.end(), greater<int>());
    int maxFreq = freq[0];
    int numMaxFreq = 0;
    for (int i = 1; i < freq.size(); ++i) {
        if (freq[i] == maxFreq) numMaxFreq++;
        else break;
    }
    return ((maxFreq-1) * n + maxFreq + numMaxFreq);
}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C" {
    PlayerTable* createPlayerTable() { 
        return new ConcretePlayerTable(); 
    }

    Leaderboard* createLeaderboard() { 
        return new ConcreteLeaderboard(); 
    }

    AuctionTree* createAuctionTree() { 
        return new ConcreteAuctionTree(); 
    }
}
