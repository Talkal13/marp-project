
#pragma once

typedef struct {
    int nodes = 0;
    clock_t avg_clocks_node = 0;
    clock_t complete_time = 0;
} benchmark;


template <typename T>
struct bounds {
    int (*opt)(std::set<T>, std::set<T>, graph<T>); // Optimistic function 
    int (*pes)(std::set<T>, std::set<T>, graph<T>); // Pesimistic function
};


