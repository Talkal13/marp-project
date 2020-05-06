
#pragma once
#include <vector>

typedef struct {
    int nodes = 0;
    clock_t avg_clocks_node = 0;
    clock_t complete_time = 0;
} benchmark;


template <typename T>
struct bounds {
    int (*opt)(std::vector<int>, std::vector<double>, std::vector<double>, int); // Optimistic function 
    int (*pes)(std::vector<int>, std::vector<double>, std::vector<double>, int); // Pesimistic function
};


