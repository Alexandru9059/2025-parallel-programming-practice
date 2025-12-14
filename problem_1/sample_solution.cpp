#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>

std::vector<std::vector<double>> read_matrix() {
    size_t rows, cols;
    std::cin >> rows >> cols;

    size_t a, b, x, y, z, p;
    std::cin >> a >> b >> x >> y >> z >> p;
    std::vector<std::vector<size_t>> intermediate(rows, std::vector<size_t>(cols, b % p));
    intermediate[0][0] = a % p;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (i > 0 && j > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i - 1][j - 1] * x) % p;
            }
            if (i > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i - 1][j] * y) % p;
            }
            if (j > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i][j - 1] * z) % p;
            }
        }
    }
    size_t max_value = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            max_value = std::max(max_value, intermediate[i][j]);
        }
    }

    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = static_cast<double>(intermediate[i][j]) / static_cast<double>(max_value);
        }
    }

    return result;
}

std::vector<std::vector<double> > Left;
std::vector<std::vector<double> > Right;
std::vector<std::vector<double> > answer;

int left_rows, left_cols, right_cols;

void Worker (int startRow, int endRow) {
    for (int i = startRow; i < endRow; ++ i ) {
        for (int j = 0; j < left_cols; ++ j ) {
            for (int k = 0; k < right_cols; ++ k ) {
                answer[i][k] += Left[i][j] * Right[j][k];
            }
        }
    }
}

int main() {
    Left = read_matrix();
    Right = read_matrix();
    left_rows = Left.size();
    left_cols = Left[0].size();
    right_cols = Right[0].size();

    if (Left.empty() || Right.empty() || Left[0].size() != Right.size()) {
        std::cerr << "Wrong matrices";
        return 1;
    }

    answer.resize(left_rows);
    for (int i = 0; i < left_rows; ++ i )
        answer[i].resize(right_cols, 0);

    int num_threads = 24;
    std::vector <std::thread> threads;
    int cnt_rows = (left_rows + num_threads - 1) / num_threads;
    int start = 0;

    for (int i = 0; i < num_threads; ++ i ) {
        threads.push_back(std::thread(Worker, start, std::min(left_rows, start + cnt_rows)));
        start += cnt_rows;
    }

    for (auto &t : threads)
        t.join();

    std::cout << left_rows << ' ' << right_cols << "\n";
    for (int i = 0; i < left_rows; ++i) {
        for (int j = 0; j < right_cols; ++j) {
            std::cout << std::setprecision(12) << answer[i][j] << ' ';
        }
        std::cout << "\n";
    }

    return 0;
}