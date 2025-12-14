#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <thread>
#include <cmath>
#include <mutex>

std::istream& operator>>(std::istream& in, __int128& value) {
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-') {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i) {
        value = value * 10 + (s[i] - '0');
    }
    if (negative) value = -value;
    return in;
}

std::ostream& operator<<(std::ostream& out, __int128 value) {
    if (value == 0) {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0) {
        negative = true;
        value = -value;
    }
    while (value > 0) {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative) s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

void Factorize (__int128 N, __int128 left, __int128 right, std::vector <__int128> &own_candidates) {
    for (__int128 i = left; i < right && i <= N / i; i += 2) {
        if (N % i == 0)
            own_candidates.push_back(i);

        while (N % i == 0)
            N /= i;
    }
}

int main() {
    __int128 n;
    std::cin >> n;
    if (n <= 1) {
        return 0;
    }

    std::vector<__int128> factors;
    while (n % 2 == 0) {
        factors.push_back(2);
        n /= 2;
    }

    int CNT_THREADS = 24;
    std::vector <std::vector <__int128> > factors_threads(CNT_THREADS);
    std::vector <std::thread> threads;
    std::vector <__int128> candidates;

    __int128 range = sqrt(n) / CNT_THREADS;
    if (range == 0) range = 2;

    __int128 start = 3;

    for (int i = 0; i < CNT_THREADS; ++ i ) {
        threads.emplace_back(std::thread(Factorize, n, start, start + range, std::ref(factors_threads[i])));
        start = start + range + (range % 2);
    }

    for (auto &t : threads)
        t.join();

    for (int i = 0; i < CNT_THREADS; ++ i ) {
        for (auto c : factors_threads[i]) {
            if (c > n / c) break;

            while (n % c == 0) {
                n /= c;
                factors.push_back(c);
            }
        }
    }

    if (n > 1)
        factors.push_back(n);

    for (const auto& factor : factors) {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}