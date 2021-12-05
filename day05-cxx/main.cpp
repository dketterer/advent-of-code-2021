#include <fstream>
#include <vector>
#include <iostream>
#include <regex>

using namespace std;


class VentLine {
private:
    int x1;
    int y1;
    int x2;
    int y2;
public:
    VentLine(int _x1, int _y1, int _x2, int _y2) : x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    bool horizontal() const {
        return y1 == y2;
    }

    bool vertical() const {
        return x1 == x2;
    }

    vector<tuple<int, int>> pointsOnLine(bool diagonal) const {
        vector<tuple<int, int>> result;
        if (horizontal()) {
            for (int x = min(x1, x2); x <= max(x1, x2); x++) result.emplace_back(x, y1);
        } else if (vertical()) {
            for (int y = min(y1, y2); y <= max(y1, y2); y++) result.emplace_back(x1, y);
        } else if (diagonal) {
            int x_direction = (x1 < x2) ? 1 : -1;
            int y_direction = (y1 < y2) ? 1 : -1;
            for (int i = 0; i < abs(x2 - x1) + 1; i++) {
                result.emplace_back(x1 + i * x_direction, y1 + i * y_direction);
            }
        }
        return result;
    }
};

class SeaGround {
private:
    int width;
    int height;
    vector<vector<int>> data;
public:
    SeaGround(int width, int height) : width(width), height(height),
                                       data(vector<vector<int>>(width, vector<int>(height, 0))) {}

    void addVent(tuple<int, int> point) {
        data[get<0>(point)][get<1>(point)]++;
    }

    int count_overlapping() {
        int result = 0;
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                if (data[x][y] > 1) result++;
            }
        }
        return result;
    }

    friend ostream &operator<<(ostream &os, const SeaGround &seaGround) {
        for (int x = 0; x < seaGround.width; x++) {
            for (int y = 0; y < seaGround.height; y++) {
                if (seaGround.data[y][x] > 0) os << seaGround.data[y][x];
                else os << '.';
            }
            os << endl;
        }
        os << endl;
        return os;
    }
};


int main() {
    std::ifstream infile("../input.txt");

    std::string line;
    std::vector<string> lines;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }

    std::regex rgx("([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)");
    vector<VentLine> vent_lines;
    for (auto &l: lines) {
        std::smatch matches;
        if (std::regex_search(l, matches, rgx)) {
            vent_lines.emplace_back(VentLine(stoi(matches[1]), stoi(matches[2]), stoi(matches[3]), stoi(matches[4])));
        }
    }
    auto width = 1000;
    int height = width;

    SeaGround sea_ground_part1(width, height);
    for (auto &vent_line: vent_lines) {
        auto points = vent_line.pointsOnLine(false);
        for (auto point: points) {
            sea_ground_part1.addVent(point);
        }
    }
    //cout << sea_ground_part1;
    cout << "part 1: " << sea_ground_part1.count_overlapping() << endl;

    SeaGround sea_ground_part2(width, height);
    for (auto &vent_line: vent_lines) {
        auto points = vent_line.pointsOnLine(true);
        for (auto point: points) {
            sea_ground_part2.addVent(point);
        }
    }
    //cout << sea_ground_part2;
    cout << "part 2: " << sea_ground_part2.count_overlapping() << endl;

    return 0;
}