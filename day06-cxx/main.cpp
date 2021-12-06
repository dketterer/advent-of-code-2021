#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <unordered_map>

using namespace std;


long long reproduce(vector<long long> lanternfish, int days) {
    for (int day = 0; day < days; day++) {
        vector<long long> lanternfish_next(9, 0);
        lanternfish_next[8] = lanternfish[0];
        lanternfish_next[6] = lanternfish[0];
        for (int timer_val = 1; timer_val < 9; timer_val++) {
            lanternfish_next[timer_val - 1] = lanternfish_next[timer_val - 1] + lanternfish[timer_val];
        }
        lanternfish = lanternfish_next;
        lanternfish_next.clear();
    }
    long long sum = 0;
    for (int timer_val = 0; timer_val < 9; timer_val++) {
        sum += lanternfish[timer_val];
    }
    return sum;
}


int main() {
    std::ifstream infile("../input.txt");

    std::string line;
    std::getline(infile, line);

    vector<int> queue;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        queue.push_back(stoi(item));
    }
    vector<long long> lanternfish(9, 0);
    for (auto inp: queue) {
        lanternfish[inp]++;
    }

    cout << "part 1: " << reproduce(lanternfish, 80) << endl;
    cout << "part 2: " << reproduce(lanternfish, 256) << endl;
    return 0;
}