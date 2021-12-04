#include <fstream>
#include <vector>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <algorithm>
#include <iomanip>

using namespace std;

std::ostream &bold_on(std::ostream &os) {
    return os << "\e[1m";
}

std::ostream &bold_off(std::ostream &os) {
    return os << "\e[0m";
}

class BingoField {
public:
    int number = -1;
    bool marked = false;

    BingoField() = default;
};

class Bingo {
private:
    int field_size;
    vector<vector<BingoField>> data;
    unordered_map<int, tuple<int, int>> index;
    bool finished = false;

public:
    Bingo(int _field_size, vector<string> lines) : field_size(_field_size),
                                                   data(field_size, vector<BingoField>(field_size)) {
        vector<string> strs;
        for (int i = 0; i < lines.size(); i++) {
            boost::split(strs, lines[i], boost::is_any_of(" "));
            for (int j = 0; j < strs.size(); j++) data[i][j].number = stoi(strs[j]);
            strs.erase(strs.begin(), strs.end());
        }
        for (int i = 0; i < field_size; i++) {
            for (int j = 0; j < field_size; j++) index[data[i][j].number] = {i, j};
        }
    }

    void mark(int number) {
        if (index.count(number) != 0) {
            auto pos = index[number];
            data[get<0>(pos)][get<1>(pos)].marked = true;
        }
    }

    bool won() {
        vector<int> count_marked_row(field_size);
        for (int i = 0; i < field_size; i++) {
            int count_marked_col = 0;
            for (int j = 0; j < field_size; j++) {
                if (data[i][j].marked) {
                    count_marked_col++;
                    count_marked_row[j]++;
                }
            }
            if (count_marked_col == field_size) {
                finished = true;
                return true;
            }
        }

        if (any_of(count_marked_row.begin(), count_marked_row.end(), [this](int i) { return i == field_size; })) {
            finished = true;
            return true;
        }

        return false;
    }

    int sum_unmarked() {
        int sum = 0;

        for (int i = 0; i < field_size; i++) {
            for (int j = 0; j < field_size; j++) sum += (data[i][j].marked) ? 0 : data[i][j].number;
        }
        return sum;
    }

    bool is_finished() const {
        return finished;
    }

    virtual ~Bingo() =
    default;

    friend ostream &operator<<(ostream &os, const Bingo &bingo) {
        for (int i = 0; i < bingo.field_size; i++) {
            for (int j = 0; j < bingo.field_size; j++) {
                if (bingo.data[i][j].marked) os << bold_on;
                os << std::setfill(' ') << std::setw(3) << bingo.data[i][j].number;
                if (bingo.data[i][j].marked) os << bold_off;
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
        boost::replace_all(line, "  ", " ");
        boost::trim(line);
        lines.push_back(line);
    }
    // build the queue
    vector<int> queue;
    std::stringstream ss(lines[0]);
    std::string item;
    while (std::getline(ss, item, ',')) {

        queue.push_back(stoi(item));
    }
    lines.erase(lines.begin(), lines.begin() + 2);

    // build the games
    vector<Bingo> games;
    vector<string> strs;
    boost::split(strs, lines[0], boost::is_any_of(" "));
    int field_size = (int) strs.size();

    vector<string> line_buffer;
    for (const string &l: lines) {
        if (l.empty()) {
            games.emplace_back(field_size, line_buffer);
            line_buffer.erase(line_buffer.begin(), line_buffer.end());
        } else line_buffer.push_back(l);
    }
    if (!line_buffer.empty()) games.emplace_back(field_size, line_buffer);
    // play
    bool part1_done = false;
    int finished_games = 0;
    for (auto inp: queue) {
        for (auto &game: games) {
            if (game.is_finished()) continue;
            game.mark(inp);
            if (game.won()) finished_games++;
            if (game.won() and !part1_done) {
                cout << "part 1: " << inp * game.sum_unmarked() << endl;
                cout << game;
                part1_done = true;
            }
            if (finished_games == games.size()) {
                cout << "part 2: " << inp * game.sum_unmarked() << endl;
                cout << game;
            }
        }
        if (finished_games == games.size()) break;
    }

    return 0;
}