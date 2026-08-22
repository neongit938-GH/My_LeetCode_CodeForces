#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <queue>
#include <deque>
#include <list>
using namespace std;


class Solution {
private:
    bool dfs(vector<vector<int>>& G, vector<int>& visited, int src, int dst, bool check) {

        if (visited[src] == 1)
            return false;

        if (src == dst)
            return true;

        visited[src] = 1;

        for (int node: G[src]) {
            if (check && node == dst) {
                continue;
            }


            if (dfs(G, visited, node, dst, false)) {
                return true;
            }

        }

        return false;
    }
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {

        int n = edges.size();

        vector<int> res;



        vector<vector<int>> adjList(n + 1);

        for (const vector<int>& edge: edges) {
            adjList[edge[0]].push_back(edge[1]);
            adjList[edge[1]].push_back(edge[0]);
        }

        vector<int> t;
        for (const vector<int>& edge: edges) {
            vector<int> visited(n+1, 0);

            if (dfs(adjList, visited, edge[0], edge[1], true)) {
                res = edge;
            }
        }

        return res;







    }
};