#include <fstream>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

vector<int> get_stats(const vector<unsigned int> &numbers, int line_length) {
    vector<int> sum(line_length, 0);
    for (unsigned int num: numbers) {
        for (int i = line_length - 1; i >= 0; i--) {
            sum[i] += (num % 2 == 1) ? 1 : -1;
            num = num >> 1;
        }
    }
    return sum;
}

unsigned int part2(const vector<unsigned int> &numbers, int const line_length, bool oxygen) {

    vector<int> sum = get_stats(numbers, line_length);
    vector<unsigned int> candidates_next;
    vector<unsigned int> candidates_now = numbers;
    for (int i = 0; i < line_length; i++) {
        // look at first, second, ...
        // if sum[index] is >= 0
        if (candidates_now.size() == 1) break;
        // for oxygen compare >= and non oxygen <
        int deciding_bit = (int) (sum[i] >= 0) == oxygen;
        for (unsigned int const &cand: candidates_now) {
            // keep all which have a deciding_bit at the index
            if ((cand >> (line_length - i - 1)) % 2 == deciding_bit) candidates_next.push_back(cand);
        }
        sum = get_stats(candidates_next, line_length);
        candidates_now = candidates_next;
        candidates_next.erase(candidates_next.begin(), candidates_next.end());
    }
    return candidates_now[0];
}


int main() {

    std::ifstream infile("../input.txt");

    std::string line;
    vector<string> lines;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    int line_length = (int) lines[0].size();
    vector<unsigned int> numbers;
    for (string const &l: lines) {
        numbers.push_back((unsigned int) stoi(l, nullptr, 2));
    }
    vector<int> sum = get_stats(numbers, line_length);
    // part 1
    int eps = (int) sum[0] > 0;
    for (int i = 1; i < line_length; i++) {
        eps = eps << 1;
        if (sum[i] > 0) eps++;
    }
    int gamma = (int) pow(2, line_length) - eps - 1;
    cout << "gamma: " << gamma << endl;
    cout << "epsilon: " << eps << endl;
    cout << "part1: " << gamma * eps << endl;

    // part2
    unsigned int oxygen = part2(numbers, line_length, true);
    unsigned int co2 = part2(numbers, line_length, false);
    cout << "oxygen: " << oxygen << endl;
    cout << "co2: " << co2 << endl;
    cout << "part2: " << co2 * oxygen << endl;

    return 0;
}