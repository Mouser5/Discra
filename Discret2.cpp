#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<vector<int>> readGraphFromFile(const string& filename) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Ошибка при открытии файла: " << filename << endl;
        exit(1);
    }

    int n;
    inputFile >> n; 
    vector<vector<int>> adjMatrix(n, vector<int>(n, 0));  

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inputFile >> adjMatrix[i][j];
        }
    }

    inputFile.close();
    return adjMatrix;
}

bool areIsomorphic(const vector<vector<int>>& adjMatrix1, const vector<vector<int>>& adjMatrix2, const vector<int>& perm) {
    int n = adjMatrix1.size();

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (adjMatrix1[i][j] != adjMatrix2[perm[i]][perm[j]]) {
                return false;
            }
        }
    }

    return true;
}


bool checkIsomorphism(const vector<vector<int>>& adjMatrix1, const vector<vector<int>>& adjMatrix2) {
    int n = adjMatrix1.size();
    if (n != adjMatrix2.size()) {
        return false;
    }
 
    vector<int> perm(n);
    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    do {
        for (int i = 0; i < n; i++) {
            cout << perm[i];
        }
        cout << endl;
    } while (next_permutation(perm.begin(), perm.end())); // идя с конца, находим пару, где значени справа больше, чем слева. запоминаем позицию (i). После этого находим наименьший элемент, которовый больше i-го. Меняем местами. Разворачиваем все элементы справа от i.

    for (int i = 0; i < n; ++i) {
        perm[i] = i;
    }

    do {
        if (areIsomorphic(adjMatrix1, adjMatrix2, perm)) {
            return true;
        }
    } while (next_permutation(perm.begin(), perm.end()));

    return false;
}

int main() {

    string file1="input1.txt", file="input.txt";

    vector<vector<int>> adjMatrix1 = readGraphFromFile(file);
    vector<vector<int>> adjMatrix2 = readGraphFromFile(file1);


    bool isIsomorphic = checkIsomorphism(adjMatrix1, adjMatrix2);


    string resultFile = "output.txt";
    ofstream outputFile(resultFile);

    if (isIsomorphic) {
        outputFile << "Графы изоморфны" << endl;
    }
    else {
        outputFile << "Графы не изоморфны" << endl;
    }

    outputFile.close();
    return 0;
}
