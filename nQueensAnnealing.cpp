#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <chrono>
using namespace std;
unsigned seed = chrono::system_clock::now().time_since_epoch().count();

vector<int> initializeState(int N){                                     // Initialise Randomly selected configuration: O(n) time complexity
    vector<int> initialState;
    for(int i = 1; i <= N; i++) initialState.push_back(i);
    shuffle(initialState.begin(), initialState.end(), default_random_engine(seed));
    return initialState;
}

vector<int> generateRandomSuccessor(vector<int> current, int N){        // Generate next successor state by swapping rows of 2 queens: O(1) 
    int col1 = N-1, col2 = N-1;
    while (current[col1] == current[col2]) col1 = (int) rand() % N, col2 = (int) rand() % N;
    swap(current[col1], current[col2]);                                 // Both queens picked at random
    return current;
}

int evaluateHeuristic(vector<int> state, int N){                        // Evaluate how many pairs of queens can attack each other: O(n^2)
    int h = 0;
    for (int i = 0; i < N; i++){
        for (int j = i+1; j < N; j++){                                  // Check only for same diagonals, no queens share same column or row
            if (abs(state[i]-state[j]) == j - i) h++;
        }
    }
    return h;
}

vector<int> simulatedAnnealing(int N, double schedule, double T, int m){// Core AI logic implementation: O(m*n^2), where m is the max Iterations
    vector<int> current = initializeState(N);
    bool solved = (evaluateHeuristic(current, N) == 0);                 // Check if Initial random state already satisfies constraint
    vector<int> next;
    int y = 0;                                                          // Bounding the number of iterations for computing n-queens
    while (T > 0 && !solved && y < m){
        T *= schedule;                                                  // Lowering T slowly enough to concentrate probability at global maxima
        next = generateRandomSuccessor(current, N);
        int deltaE = evaluateHeuristic(current, N) - evaluateHeuristic(next, N);
        if (deltaE > 0) {                                               // Case where the successor improves the situation (Hill Climbing)
            current.clear();
            current = next;
        }
        else{
            double prob = exp( deltaE / T );                            // Probability function that decides to follow a worse successor
            double x = (double) rand()/RAND_MAX;
            if (x < prob){
                current.clear();
                current = next;
            }
        }
        solved = (evaluateHeuristic(current, N) == 0);
        next.clear();
        y++;
    }
    return solved ? current : next;                                     // Return solution vector if successful, else an empty vector
}

int main(){
    int N, maxIterations = 75000;
    double T = 10000, schedule = 0.99;
    cout << "Please enter number of Queens: ";
    cin >> N;
    srand(seed);
    vector<int> v = simulatedAnnealing(N, schedule, T, maxIterations);
    if (v.empty()) cout << "Failure" << endl;
    else{
        cout << "Solution for " << N << " Queens: " << endl;
        cout << "[";
        for (int i = 0; i < N-1; i++) cout << v[i] << ", ";
        cout << v[N-1] << "]" << endl;
    }
    return 0;
}
