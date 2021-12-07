#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;


int sum_between(int start, int end) {
    return ((end - start) + 1) * (start + end) / 2;
}

int main() {

    std::ifstream infile("../input.txt");

    std::string line;
    std::getline(infile, line);

    vector<int> crab_positions_list;
    std::stringstream ss(line);
    std::string item;
    while (std::getline(ss, item, ',')) {
        crab_positions_list.push_back(stoi(item));
    }
    const auto[min, max] = std::minmax_element(begin(crab_positions_list), end(crab_positions_list));
    int num_elements = *max - *min + 1;
    // fast lookup
    unordered_map<int, int> crab_locations_dict;
    for (auto pos: crab_positions_list) crab_locations_dict[pos]++;

    // ---------------- part1 --------------------------------------------

    vector<int> fuel_sum_left_right(num_elements, 0);
    vector<int> crabs(num_elements, 0);

    // from left
    vector<int> fuel_crabs_left(num_elements, 0);
    for (int i = 1; i < num_elements; i++) {
        crabs[i] = crabs[i - 1] + crab_locations_dict[i + *min - 1];
        int fuel = fuel_crabs_left[i - 1] + crabs[i];
        fuel_crabs_left[i] = fuel;
        fuel_sum_left_right[i] = fuel;
    }

    // from right
    crabs[num_elements - 1] = 0;
    vector<int> fuel_crabs_right(num_elements, 0);
    for (int i = num_elements - 1; i >= 0; i--) {
        crabs[i] = crabs[i + 1] + crab_locations_dict[i + *min + 1];
        int fuel = fuel_crabs_right[i + 1] + crabs[i];
        fuel_crabs_right[i] = fuel;
        fuel_sum_left_right[i] += fuel;
    }

    // ---------------------- part2-----------------------------------------
    vector<int> fuel_exp_crabs(num_elements, 0);
    for (int target = 0; target < num_elements; target++) {
        for (int start = 0; start < num_elements; start++) {
            if (target == start) continue;
            fuel_exp_crabs[target] += sum_between(0, abs(target - start)) * crab_locations_dict[start + *min];
        }
    }

    const auto min_fuel_part1 = min_element(fuel_sum_left_right.begin(), fuel_sum_left_right.end());
    const auto min_fuel_part2 = min_element(fuel_exp_crabs.begin(), fuel_exp_crabs.end());
    cout << "part 1: " << *min_fuel_part1 << endl;
    cout << "part 2: " << *min_fuel_part2 << endl;

    return 0;
}