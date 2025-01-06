#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

bool isSafe(int v, const vector<vector<int>>& graph, vector<int>& path, int pos) {
    // Проверка на наличие ребра
    if (graph[path[pos - 1]][v] == 0) {
        return false;
    }

    for (int i = 0; i < pos; i++) {
        if (path[i] == v) {
            return false;
        }
    }

    return true;
}

bool hamCycleDFS(const vector<vector<int>>& graph, vector<int>& path, int pos) {
    int n = graph.size();

    if (pos == n) {
        return graph[path[pos - 1]][path[0]] == 1;
    }

    for (int v = 1; v < n; v++) {
        if (isSafe(v, graph, path, pos)) {
            path[pos] = v;

            if (hamCycleDFS(graph, path, pos + 1)) {
                return true;
            }

            path[pos] = -1;
        }
    }

    return false;
}


bool findHamiltonianCycle(const vector<vector<int>>& graph, const string filename) {
    int n = graph.size();
    ofstream file(filename);
    if (n == 2) {
        file << "Гамильтонов цикл не найден." << endl;
        return false;
    }
    vector<int> path(n, -1);
    path[0] = 0;
    if (!hamCycleDFS(graph, path, 1)) {
        file << "Гамильтонов цикл не найден." << endl;
        return false;
    }
    file << "Гамильтонов путь найден: ";
    for (int v : path) {
        file << v << " ";
    }
    file << path[0] << endl; 
    return true;
}


vector<vector<int>> readGraph(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл.");
    }

    int n;
    file >> n;
    vector<vector<int>> graph(n, vector<int>(n, 0));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> graph[i][j];
        }
    }

    file.close();
    return graph;
}

int main() {
    string file="output.txt";
    string filename = "input.txt"; 
    vector<vector<int>> graph = readGraph(filename);
    findHamiltonianCycle(graph,file);


    return 0;
}