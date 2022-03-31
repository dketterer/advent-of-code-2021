#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <set>
#include <iterator>
#include <unordered_map>
#include <boost/algorithm/string/trim.hpp>

using namespace std;


int part1(const vector<vector<string>> &four_digits) {
    // segments => number
    // 2 => 1
    // 3 => 7
    // 4 => 4
    // 7 => 8
    int count = 0;
    for (const auto &digits: four_digits) {
        for (const auto &digit: digits) {
            switch (digit.size()) {
                case 2:
                case 3:
                case 4:
                case 7:
                    count++;
                    break;
                default:
                    break;
            }
        }
    }
    return count;
}

int read_display(unordered_map<char, char> assignment, const vector<string> &digits) {
    unordered_map<string, int> decode_seven_digit = {{"abcefg",  0},
                                                     {"cf",      1},
                                                     {"acdeg",   2},
                                                     {"acdfg",   3},
                                                     {"bcdf",    4},
                                                     {"abdfg",   5},
                                                     {"abdefg",  6},
                                                     {"acf",     7},
                                                     {"abcdefg", 8},
                                                     {"abcdfg",  9}};
    string display;
    for (const auto &digit: digits) {
        string seven_digit;
        for (auto c: digit) {
            seven_digit += assignment[c];
        }
        sort(seven_digit.begin(), seven_digit.end());
        display += to_string(decode_seven_digit[seven_digit]);
    }
    return stoi(display);
}


unordered_map<char, char> solve_line(vector<string> signal_patterns) {
    // solve the easy ones and write to a map (string => int)
    // segments => number
    // 2 => 1
    // 3 => 7
    // 4 => 4
    // 7 => 8
    // solve a map, each pos a, b, c, ... maps to one => a, b, c, ... in the end

    unordered_map<char, set<char>> candidates = {{'a', set<char>()},
                                                 {'b', set<char>()},
                                                 {'c', set<char>()},
                                                 {'d', set<char>()},
                                                 {'e', set<char>()},
                                                 {'f', set<char>()},
                                                 {'g', set<char>()}};
    // sort signal patterns a...g
    // sort by length like 2,3,4,6,7,5
    for (auto &pattern: signal_patterns) std::sort(pattern.begin(), pattern.end());
    std::sort(signal_patterns.begin(), signal_patterns.end(), []
            (const std::string &first, const std::string &second) {
        if (second.size() == 5) return true;
        if (first.size() == 5) return false;
        return first.size() < second.size();
    });
    for (auto &pattern: signal_patterns) {
        if (pattern.size() == 2) {
            for (char c: pattern) candidates['c'].insert(c);
            for (char c: pattern) candidates['f'].insert(c);
        }
        if (pattern.size() == 3) {
            set_difference(pattern.begin(), pattern.end(), candidates['c'].begin(), candidates['c'].end(),
                           std::inserter(candidates['a'], candidates['a'].begin()));
        }
        if (pattern.size() == 4) {
            set_difference(pattern.begin(), pattern.end(), candidates['c'].begin(), candidates['c'].end(),
                           std::inserter(candidates['b'], candidates['b'].begin()));
            candidates['d'] = candidates['b'];
        }
        if (pattern.size() == 6) {
            vector<char> diff;
            set<char> in_use;
            in_use.insert(candidates['c'].begin(), candidates['c'].end());
            in_use.insert(candidates['f'].begin(), candidates['f'].end());
            in_use.insert(candidates['a'].begin(), candidates['a'].end());
            in_use.insert(candidates['b'].begin(), candidates['b'].end());
            in_use.insert(candidates['d'].begin(), candidates['d'].end());
            set_difference(pattern.begin(), pattern.end(), in_use.begin(), in_use.end(),
                           std::inserter(diff, diff.begin()));
            if (diff.size() == 1) { // pattern must be the 9
                candidates['g'].insert(diff[0]);
                in_use.insert(diff[0]);
                string all = "abcdefg";
                set_difference(all.begin(), all.end(), in_use.begin(), in_use.end(),
                               std::inserter(candidates['e'], candidates['e'].begin()));
            } else if (candidates['c'].size() == 2) {
                vector<char> diff_zero_six;
                set_difference(candidates['c'].begin(), candidates['c'].end(), pattern.begin(), pattern.end(),
                               std::inserter(diff_zero_six, diff_zero_six.begin()));
                if (diff_zero_six.size() == 1) { // must be 6
                    candidates['c'].erase(candidates['c'].begin(), candidates['c'].end());
                    candidates['c'].insert(diff_zero_six[0]);
                    candidates['f'].erase(diff_zero_six[0]);
                }
            }
        }
        if (pattern.size() == 5) {
            vector<char> diff;
            set<char> in_use;
            in_use.insert(candidates['a'].begin(), candidates['a'].end());
            in_use.insert(candidates['c'].begin(), candidates['c'].end());
            in_use.insert(candidates['f'].begin(), candidates['f'].end());
            in_use.insert(candidates['g'].begin(), candidates['g'].end());
            set_difference(pattern.begin(), pattern.end(), in_use.begin(), in_use.end(),
                           std::inserter(diff, diff.begin()));
            if (diff.size() == 1) { // must be the 3
                candidates['d'].erase(candidates['d'].begin(), candidates['d'].end());
                candidates['d'].insert(diff[0]);
                candidates['b'].erase(diff[0]);
                break;
            }
        }
    }
    unordered_map<char, char> assignment;
    for (auto &pair: candidates) assignment[*pair.second.begin()] = pair.first;
    return assignment;
}


int part2(const vector<vector<string>> &pattern_observations, const vector<vector<string>> &four_digits) {
    int sum = 0;
    for (int i = 0; i < pattern_observations.size(); i++) {
        auto pattern = pattern_observations[i];
        auto four = four_digits[i];
        auto assignment = solve_line(pattern);
        sum += read_display(assignment, four);
    }

    return sum;
}


int main() {
    std::ifstream infile("../input.txt");

    std::string line;
    std::vector<string> lines;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    vector<vector<string>> pattern_observations(lines.size(), vector<string>());
    vector<vector<string>> four_digits(lines.size(), vector<string>());

    for (int i = 0; i < lines.size(); i++) {
        stringstream ss(lines[i]);
        string item;
        getline(ss, item, '|');
        boost::algorithm::trim(item);
        stringstream ss_patterns(item);
        string pattern;
        while (getline(ss_patterns, pattern, ' ')) {
            pattern_observations[i].push_back(pattern);
        }
        getline(ss, item, '|');
        boost::algorithm::trim(item);
        stringstream ss_digits(item);
        string digit;
        while (getline(ss_digits, digit, ' ')) {
            four_digits[i].push_back(digit);
        }
    }

    cout << "part 1: " << part1(four_digits) << endl;
    cout << "part 2: " << part2(pattern_observations, four_digits) << endl;

    return 0;
}