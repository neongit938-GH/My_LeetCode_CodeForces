/*
    LeetCode 785: Is Graph Bipartite?
 
    Algorithm: 2-Coloring using DFS
 
    - A graph is bipartite if we can color every node using only 2 colors
      such that no two adjacent nodes share the same color.
    - We do a DFS from every uncolored node (to handle disconnected components).
    - When we visit a node, we color it opposite to the color of the node
      that led us to it (parent).
    - While exploring neighbors:
        - If a neighbor is uncolored, recursively color it with the opposite color.
        - If a neighbor is already colored, check if it has the SAME color as
          the current node. If yes, it's a conflict -> graph is NOT bipartite.
    - If DFS completes for all components without conflicts, the graph is bipartite.
 
    Time Complexity: O(V + E) - each node and edge visited once
    Space Complexity: O(V) - color array + recursion stack
*/
 
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
    // DFS helper: tries to color 'node' with color C, then colors all
    // its neighbors with the opposite color, checking for conflicts.
    bool helper_dfs(int node, int C, vector<int>& color, vector<vector<int>>& graph) {
        color[node] = C;  // assign current color to this node
 
        int opp_color = 1 - C;  // the color neighbors must take (0 -> 1, 1 -> 0)
 
        // visit all adjacent nodes of the current node
        for (int adj_node: graph[node]) {
            if (color[adj_node] == -1) {
                 // neighbor not colored yet -> recursively color it
                 // with the opposite color and keep exploring deeper
                 if (!helper_dfs(adj_node, opp_color, color, graph) {
                     return false; 
                 }
                     
            }
            else if (color[adj_node] == color[node]) {
                // neighbor already colored, but same color as current node
                // -> conflict, graph cannot be 2-colored -> not bipartite
                return false;
            }
        }
 
        // no conflicts found among this node's neighbors
        return true;
    }
public:
    bool isBipartite(vector<vector<int>>& graph) {
 
        int n = graph.size();
 
        // -1 means "not colored yet"; 0 and 1 are the two colors
        vector<int> color(n, -1);
 
        // graph may be disconnected, so we must start a fresh DFS
        // from every node that hasn't been colored yet
        for (int i = 0; i < n; i++) {
            if (color[i] == -1) {
                if (helper_dfs(i, 0, color, graph) == false) {
                    return false;  // conflict found in this component
                }
            }
        }
 
        // all components processed with no conflicts
        return true;
 
    }
};
 
/*
    Important test case that shows WHY DFS (proper traversal) is needed
    instead of just a nested for loop over the adjacency list:
 
        graph = [[1], [0,3], [3], [1,2]]
 
    Node 0 -- Node 1 -- Node 3 -- Node 2 (back to 1)
    This forms a cycle: 1 -> 3 -> 2 -> 3... wait, actually here
    1 connects to 3, and 3 connects to 2, and 2 connects to 3 again,
    forming the cycle 1-3-2 combined with 3-1.
 
    A naive nested loop (just checking direct neighbors pairwise) can miss
    the deeper conflict that only shows up once you propagate colors
    ALONG the path (0 -> 1 -> 3 -> 2 -> back to 3). DFS naturally propagates
    the alternating color assignment through chains of nodes, which is
    exactly what's needed to correctly detect whether a cycle of odd length
    creates a conflict. A flat nested loop over edges without traversal/
    propagation would not correctly assign colors transitively through
    the graph and could give a wrong answer.
*/
 