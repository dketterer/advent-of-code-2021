#include <fstream>
#include <vector>
#include <iostream>
#include <numeric>

using namespace std;


int main() {

    std::ifstream infile("../input.txt");

    std::string line;
    std::vector<int> measurements;
    while (std::getline(infile, line)) {
        measurements.push_back(stoi(line));
    }
    int increased = 0;
    int window_size = 3;
    for (int i = 0; i < measurements.size() - window_size; i++) {
        if (accumulate(measurements.begin() + i, measurements.begin() + i + window_size, 0) <
            accumulate(measurements.begin() + i + 1, measurements.begin() + i + window_size + 1, 0))
            increased++;
    }
    cout << increased;

    return 0;
}