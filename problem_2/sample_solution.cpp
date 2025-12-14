#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>

std::vector<size_t> read_array() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    return result;
}

std::vector <size_t> array;
std::vector <size_t> copy_array;

void Worker (int l, int r) {
    std::sort(array.begin() + l, array.begin() + r);
}

void WorkerMerge (int start, int mid, int end) {
    std::merge(array.begin() + start, array.begin() + mid, array.begin() + mid, array.begin() + end, copy_array.begin() + start);
}

int main() {
    array = read_array();
    copy_array.resize(array.size());


    int start = 0;
    std::vector <std::thread> threads;

    for (int bsize = 1; bsize < array.size(); ++ bsize ) {
        threads.clear();
        for (int start = 0; start < array.size(); start += 2 * bsize) {
            int mid = std::min(start + bsize, (int)array.size());
            int end = std::min((int)array.size(), start + 2 * bsize);

            threads.push_back(std::thread(WorkerMerge, start, mid, end));
        }

        for (auto &t : threads)
            t.join();
        array = copy_array;
    }

    size_t k;
    std::cin >> k;
    for (size_t i = k - 1; i < array.size(); i += k) {
        std::cout << array[i] << ' ';
    }
    std::cout << "\n";

    return 0;
}