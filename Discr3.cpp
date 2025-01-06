#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <queue>

using namespace std;

ifstream inFile("input.txt");
ofstream outFile("output.txt");

void Printedges(const vector <int> inDegree, int n) {
    for (int i = 0; i < n; i++) {
        outFile << i << " - " << inDegree[i] << endl;
    }
}

bool hasRoot(int n, const vector<pair<int, int>>& edges, int& root) {
    vector<int> inDegree(n, 0);
    vector<int> outDegree(n, 0);
    int count = 0; // Проверка, что корень 1 
    for (const auto& edge : edges) {
        int v = edge.second;
        int i = edge.first;
        inDegree[v]++;
        outDegree[i]++;
    }

    for (int i = 0; i < n; ++i) {
        if (inDegree[i] == 0) {
            count++;
            if (outDegree[i]!=0)
                root = i;
            
        }
    }

    if (count != 1) {
        if (count>0)
            outFile << "Корень должен быть один." << endl;
        else 
            outFile << "Кореня не найдено. " <<endl;
        Printedges(inDegree, n);
        return false;
    }
    else {
        outFile << "Корень найден в вершине: " << root << endl;
        return true;
    }

}

bool checkInDegree(int n, const vector<pair<int, int>>& edges, int root) {
    vector<int> inDegree(n, 0);
    bool res = true;
    int count = 0;
    for (const auto& edge : edges) {
        int v = edge.second;
        inDegree[v]++;
    }

    for (int i = 0; i < n; ++i) {
        if ( i!=root && inDegree[i] != 1) {
            if (res)
                outFile << "Нарушение степени полузахода" << endl;  
            res = false;
            outFile << i << " - " << inDegree[i] << endl;
        }
    }

    return res;  
}

bool checkReachability(int n, const vector<pair<int, int>>& edges, int root) {
    vector<bool> visited(n, false);
    if (root == -1)
        root = edges[0].first;
    vector<vector<int>> adjList(n);
    bool res = true;
    for (const auto& edge : edges) {
        adjList[edge.first].push_back(edge.second);
    }

    // BFS 
    queue<int> q;
    q.push(root);
    visited[root] = true;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int neighbor : adjList[curr]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            outFile << i << " - недостижима из корня " << endl;
            res= false;
        }
    }

    return res;
}


int main() {
    if (!inFile.is_open()) {
        cerr << "Ошибка при открытии входного файла!" << endl;
        return 1;
    }

    int n, m;
    inFile >> n >> m;
    if (m==n-1)
        outFile << "Древочисленный граф." << endl;
    else
        outFile << "Не древочисленный граф." << endl;
    vector<pair<int, int>> edges(m);    
    for (int i = 0; i < m; ++i) {
        inFile >> edges[i].first >> edges[i].second;
    }
    bool req1, req2, req3;
    inFile.close();  

    int root = -1;
    req1 = hasRoot(n, edges, root);
    req2 = checkInDegree(n, edges, root);
    req3 = checkReachability(n, edges, root);
    if (req1 && req2 && req3){
        outFile << "Ордерево." << endl;
    }
    else {
        outFile << "Не ордерево." << endl;
    }
    outFile.close(); 

    return 0;
}
