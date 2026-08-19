/*
    LeetCode 994: Rotting Oranges

    Algorithm: Multi-source BFS (level-by-level)

    - Grid contains: 0 = empty cell, 1 = fresh orange, 2 = rotten orange.
    - Every minute, any fresh orange 4-directionally adjacent to a rotten
      orange becomes rotten too. We need the minimum number of minutes
      until no fresh orange remains (or -1 if impossible).
    - Instead of starting BFS from a single source, we push ALL initially
      rotten oranges into the queue at once ("multi-source BFS"). This way,
      rotting spreads outward from every rotten orange simultaneously,
      exactly like it would happen minute by minute in real life.
    - We process the queue level by level: each full pass over the current
      queue represents "1 minute" of rotting spreading outward once.
    - We track countFresh (fresh oranges remaining) to know if all oranges
      eventually rot, and countMin to track elapsed minutes.

    Time Complexity: O(n * m) - each cell is enqueued and processed at most once
    Space Complexity: O(n * m) - queue can hold up to all cells in the worst case
*/

class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        int n = grid.size();
        int m = grid[0].size();
        
        int countFresh = 0;   // tracks how many fresh (1) oranges are left to rot
        
        int countMin = -1;    // starts at -1 to offset the "extra" first BFS round
                               // (see explanation below the class)
        
        queue<pair<int, int>> q;  // holds coordinates of all currently rotten oranges
        
        // scan the grid once: collect all initially rotten oranges as BFS sources,
        // and count how many fresh oranges exist in total
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                if (grid[i][j] == 2) {
                    q.push({i, j});
                }
                else if (grid[i][j] == 1) {
                    countFresh++;
                }
            }
        }
        
        
        // no fresh oranges at all -> nothing needs to rot -> 0 minutes needed
        if (countFresh == 0)
            return 0;
        
        // fresh oranges exist but there is no rotten orange to start the spread
        // -> they can never rot -> impossible
        if (q.empty())
            return -1;
        
        // the 4 directional moves: up, down, left, right
        vector<pair<int, int>> dir = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        
        int size, x, y, d_x, d_y;
        
        
        // standard level-by-level BFS: process one full "layer" of the
        // queue per while-loop iteration, which corresponds to 1 minute passing
        while (!q.empty()) {
            size = q.size();   // freeze the number of nodes in the current level
                                // before we start pushing new nodes for the next level
            
            for (int i = 0; i < size; i++) {
                pair<int, int> node = q.front();
                q.pop();
                
                
                // check all 4 neighbors of the current rotten orange
                for (pair<int, int> d: dir) {
                    x = node.first;
                    y = node.second;
                    d_x = d.first;
                    d_y = d.second;
                    
                    x = x + d_x;
                    y = y + d_y;
                    
                    // if neighbor is inside bounds and is a fresh orange, rot it
                    if (x >= 0 && x < n && y >= 0 && y < m && grid[x][y] == 1) {
                        grid[x][y] = 2;        // mark as rotten (acts as "visited")
                        countFresh--;          // one less fresh orange remaining
                        q.push({x, y});        // this orange will spread rot next round
                    }
                }
                
            }
            
            countMin++;   // one full level processed -> effectively 1 minute passed
                           // (except the very first round, which countMin = -1 cancels out)
            
            
        }
        
        // if every fresh orange eventually got rotten, countMin holds the answer;
        // otherwise some fresh oranges were unreachable -> impossible
        if (countFresh == 0) {
            return countMin;
        }
        else
            return -1;
        
        
    }
};

/*
    WHY countMin = -1:

    The while loop always runs one extra round compared to the actual number
    of minutes that pass. That's because the FIRST round of the loop just
    processes the oranges that were ALREADY rotten at the start (minute 0) —
    no real time has passed yet, but the loop still does countMin++ at the
    end of that round.

    - Round 1: processes oranges rotten at minute 0 (no time has actually
      elapsed) -> countMin still gets incremented once.
    - Round 2: processes oranges that rotted from round 1's spread -> this
      really is "1 minute later."
    - Round 3: this is "2 minutes later." ...and so on.

    So loop rounds = actual minutes + 1. Starting countMin at -1 shifts the
    counter down by that one extra round, so the final value correctly
    equals the number of minutes elapsed rather than the number of loop
    iterations.

    Example: grid = [[2,1,1]]
      Initial queue: {(0,0)}, countMin = -1
      Round 1: pop (0,0), rot (0,1)         -> countMin becomes 0
      Round 2: pop (0,1), rot (0,2)         -> countMin becomes 1
      Round 3: pop (0,2), nothing new to rot -> countMin becomes 2
      Answer: 2 minutes (correct)


    REAL-LIFE APPLICATIONS OF THIS "MULTI-SOURCE BFS SPREADING" PATTERN:

    This isn't just about oranges — this pattern models anything that
    spreads outward simultaneously from multiple starting points, one
    step/unit of time at a time. Some real-world uses:

    1. Epidemic / disease spread modeling
       - Infected people (multiple starting points) spread infection to
         adjacent healthy people over discrete time steps (days).

    2. Fire or wildfire spread simulation
       - Multiple ignition points spread to adjacent flammable cells over
         time, used in forest-fire spread prediction models.

    3. Network/virus propagation in computer networks
       - Malware or a broadcast signal spreading from multiple infected/
         source nodes to directly connected nodes, round by round.

    4. Social network "influence" or rumor spreading
       - Modeling how information/rumors spread from multiple people who
         already know it to their direct connections, minute/day by day.

    5. Wireless signal / Wi-Fi coverage expansion
       - Multiple routers/access points as sources, signal reaching
         adjacent areas over successive time steps.

    6. Water flooding / contamination spread simulation
       - Multiple flood or leak sources spreading to adjacent cells in a
         terrain/pipe grid over time (used in geographic flood modeling).

    7. Game development (AI vision / fog-of-war reveal, area-of-effect
       damage spread)
       - Multiple enemy units or effects (e.g. poison gas clouds) spreading
         effects to adjacent tiles turn by turn.

    The common theme: multiple simultaneous sources + expansion to
    neighbors + discrete time steps = multi-source BFS, and the number
    of BFS "levels" processed directly corresponds to the time taken.
*/