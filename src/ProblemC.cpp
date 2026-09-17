#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>
#include <stack>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
using namespace std;
using namespace chrono;

vector<vector<int>> graph;
vector<int> disc, low;
vector<pair<int, int>> bridges;

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

void read_input(int& N, int& M, vector<vector<char>>& maze, Point& door, Point& exit, unordered_set<int>& manholes, int& C) {
    scanf("%d %d", &N, &M);

    maze.resize(N, vector<char>(M));
    manholes.clear();
    graph.clear();
    graph.resize(N * M);

    //Temporary buffer to hold input row
    vector<char> row(M);

    //Read the first row and find the exit 'E'
    for (int i = 0; i < M; i++) {
        scanf(" %c", &row[i]);
        maze[0][i] = row[i];
        if (row[i] == 'E') {
            exit = {0, i};
        }
    }

    //Read the middle rows
    for (int i = 1; i < N - 1; i++) {
        for (int j = 0; j < M; j++) {
            scanf(" %c", &row[j]);
            maze[i][j] = row[j];
            if (row[j] == 'M') {
                manholes.insert(i * M + j);
            }
            if (row[j] != '#') {
                if (maze[i - 1][j] != '#') {
                    graph[(i - 1) * M + j].push_back(i * M + j);
                    graph[i * M + j].push_back((i - 1) * M + j);
                }
                if (j > 0 && row[j - 1] != '#') {
                    graph[i * M + j - 1].push_back(i * M + j);
                    graph[i * M + j].push_back(i * M + j - 1);
                }
            }
        }
    }

    //Read the last row and find the door 'D'
    for (int i = 0; i < M; i++) {
        scanf(" %c", &row[i]);
        maze[N - 1][i] = row[i];
        if (row[i] == 'D') {
            door = {N - 1, i};
            if (maze[N - 2][i] != '#') {
                graph[(N - 2) * M + i].push_back((N - 1) * M + i);
                graph[(N - 1) * M + i].push_back((N - 2) * M + i);
            }
        }
    }

    scanf("%d", &C);
}

void dfs(int start, const vector<int>& blockedBridge, vector<bool>& visited, vector<int>& exitPath, vector<Point>& remainingManholes, const unordered_set<int>& manholeSet, const int M, int e) {
    stack<int> stack;
    stack.push(start);

    unordered_map<int, int> predecessor;
    predecessor[start] = -1;

    bool found = false;

    while (!stack.empty() && !found) {
        int v = stack.top();
        stack.pop();

        if (!visited[v]) {
            visited[v] = true;

            if (v == e) {
                found = true;
                while (v != -1) {
                    exitPath.push_back(v);
                    v = predecessor[v];
                }
                break;
            }

            if (manholeSet.find(v) != manholeSet.end()) {
                int x = v / M;
                int y = v % M;
                remainingManholes.push_back({x, y});
            }

            for (int w : graph[v]) {
                if (!visited[w] && !(v == blockedBridge[0] && w == blockedBridge[1]) && !(v == blockedBridge[1] && w == blockedBridge[0])) {
                    stack.push(w);
                    if (predecessor.find(w) == predecessor.end()) {
                        predecessor[w] = v;
                    }
                }
            }
        }
    }
}

void findBridges(int N, const int M, const unordered_set<int>& manholes, const Point& door, const Point& exit) {
    int time = 0;
    stack<pair<int, int>> dfsStack;
    vector<int> parent(N, -1);
    vector<int> manholeSpots(N, 0);
    vector<int> finalPath;

    //percorrer nós
    for (int i = 0; i < N; i++) {
        if (disc[i] == -1) { //nó não visitado
            dfsStack.push({i, -1}); //nó atual, nó pai 

            while (!dfsStack.empty()) { //Descida na recursao
                int v = dfsStack.top().first;
                int p = dfsStack.top().second;
                dfsStack.pop();

                if (disc[v] == -1) { //se não visitado
                    disc[v] = low[v] = time++;
                    parent[v] = p;

                    //Ver se é um manhole
                    Point pos;
                    pos.x= v / M;
                    pos.y= v % M;

                    if(manholes.find(v) != manholes.end()){
                        manholeSpots[v]= 1;
                    } else if(pos == door){
                        manholeSpots[v]= - (manholes.size() + 1);
                    }
                }

                bool hasUnvisited = false;
                for (int w : graph[v]) { //percorrer nós q ligam ao atual
                    if (disc[w] == -1) { //se nó ainda não tiver sido percorrida, adiciona-se à stack
                        dfsStack.push({v, p});
                        dfsStack.push({w, v});
                        hasUnvisited = true;
                        break;
                    } else if (w != parent[v]) {
                        low[v] = min(low[v], disc[w]);
                    }
                }

                if (!hasUnvisited) { //Subida na recursao
                    if (p != -1) {
                        
                        //Manholes
                        manholeSpots[p] += manholeSpots[v];

                        //store on final path
                        if(manholeSpots[v] < 0){
                            finalPath.push_back(v);
                        }

                        low[p] = min(low[p], low[v]);
                        if (low[v] > disc[p]) {
                            bridges.push_back({p, v});
                        }
                    }
                }
            }
        }
    }

    auto cellIndex = [M](int x, int y) { return x * M + y; };

    //Ver quais bridges teem mais manholes
    Point best= {-1, -1};
    for(auto bridge: bridges){
        if(manholeSpots[bridge.second] > manholeSpots[best.y]){
            best.x= bridge.first;
            best.y= bridge.second;
        }
    }
    
    // Perform DFS to find the remaining manholes and path from door to exit
    vector<int> blockedBridge = {best.x, best.y}; 
    vector<bool> visited(N, false);
    vector<Point> remainingManholes;
    vector<int> exitPath;

    dfs(cellIndex(door.x, door.y), blockedBridge, visited, exitPath, remainingManholes, manholes, M, cellIndex(exit.x, exit.y));

    //PRINTS
    cout<< best.x / M <<" "<< best.x % M << " ";
    cout<< best.y / M <<" "<< best.y % M << "\n";
    cout<<remainingManholes.size()<<"\n";
    for(Point p: remainingManholes){
        cout<< p.x << " " << p.y << "\n";
    }
    cout<<exitPath.size()<<"\n";
    for(int i= exitPath.size()-1; i>=0; i--){
        cout<< exitPath[i] / M << " " << exitPath[i] % M << "\n";
    }
}

void process_test_case(const int N, const int M, const vector<vector<char>>& maze, const Point& door, const Point& exit, const unordered_set<int>& manholes, const int C, Point& flood_gate, vector<Point>& manhole_cover_positions, vector<Point>& path) {
    int totalCells = N * M;
    
    disc.assign(totalCells, -1);
    low.assign(totalCells, -1);
    bridges.clear();

    auto cellIndex = [M](int x, int y) { return x * M + y; };

    // Find bridges in the graph
    findBridges(totalCells, M, manholes, door, exit);
}

int main() {
    int num_test_cases;
    cin >> num_test_cases;

    //Start timer
    //auto start_time = high_resolution_clock::now();

    for (int t = 0; t < num_test_cases; t++) {
        int N, M, C;
        vector<vector<char>> maze;
        Point door, exit;
        unordered_set<int> manholes;

        read_input(N, M, maze, door, exit, manholes, C);

        Point flood_gate;
        vector<Point> manhole_cover_positions;
        vector<Point> path;

        process_test_case(N, M, maze, door, exit, manholes, C, flood_gate, manhole_cover_positions, path);
    }

    //auto end_time = high_resolution_clock::now();
    //auto duration = duration_cast<milliseconds>(end_time - start_time);
    //cout << "Time taken: " << duration.count() << " milliseconds" << endl;

    return 0;
}