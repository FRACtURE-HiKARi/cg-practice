#pragma once
#include <sstream>
#include <chrono>
#include <list>
typedef std::chrono::high_resolution_clock hrClock;
typedef std::chrono::time_point<std::chrono::steady_clock> TimePoint;
typedef struct Watch{
    TimePoint s;
    TimePoint e;
    void start() {
        s = hrClock::now();
    }
    void end() {
        e = hrClock::now();
    }
    long duration() {
        return (long)std::chrono::duration_cast<std::chrono::microseconds>(e - s).count();
    }
    void print_duration() {
        std::cout << duration() << "us\n";
    }
    void tick() {end(); print_duration();}
} Watch;

typedef struct Avg_n {
    int n = 10;
    std::list<float> numbers;
    Avg_n(int num) { n = num;}
    float getAverage() {
        if (numbers.empty()) return 0;
        float sum = 0;
        for (auto x: numbers)
            sum += x;
        return sum / (float)numbers.size();
    }
    void push(float x) {
        numbers.push_back(x);
        if (numbers.size() > n) numbers.pop_front();
    }
}Avg_n;