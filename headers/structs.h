
#pragma once
#include <vector>

typedef struct {
    int nodes = 0;
    clock_t avg_clocks_node = 0;
    clock_t complete_time = 0;
} benchmark;


struct bounds {
    unsigned (*opt)(std::vector<int>, std::vector<double>, std::vector<double>, double); // Optimistic function 
    unsigned (*pes)(std::vector<int>, std::vector<double>, std::vector<double>, double); // Pesimistic function
};


