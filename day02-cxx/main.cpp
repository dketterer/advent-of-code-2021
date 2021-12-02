#include <fstream>
#include <iostream>
#include <regex>

using namespace std;


int main() {

    std::ifstream infile("../input.txt");
    bool part2 = true;
    int horizontal_pos = 0;
    int depth_pos = 0;
    int aim = 0;
    std::string line;
    while (std::getline(infile, line)) {
        std::string delimiter = " ";
        std::string direction = line.substr(0, line.find(delimiter));
        int val = stoi(line.substr(line.find(delimiter), line.size()));
        if (!part2) {
            if (direction == "forward") horizontal_pos += val;
            if (direction == "down") depth_pos += val;
            if (direction == "up") depth_pos -= val;
        } else {
            if (direction == "forward") {
                horizontal_pos += val;
                depth_pos += aim*val;
            }
            if (direction == "down") aim += val;
            if (direction == "up") aim -= val;
        }
    }

    cout << horizontal_pos * depth_pos;

    infile.close();
    return 0;
}