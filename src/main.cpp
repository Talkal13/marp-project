#include <iostream>
#include "../headers/graph.h"
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include "../headers/max_clique.h"
#include "../headers/bounds.h"
#include "../headers/globals.h"


using namespace std;

benchmark marks;
bounds<int> bound;

bool parse(int args, char *argv[], std::string &filename, string &fileout, int &seed, bool &random);
template <class A>
void translate_dimacs(graph<A> &graph, std::string filename);
std::tuple<double, double, int, int> run_test_from_file(std::string filename, int b);
std::tuple<double, double, int, int> run_test_random(int seed, float p, int N, int b);
void write_to_file(std::string fileout, std::tuple<double, double, int, int> result);
void print_help_message();
template <class T>
std::tuple<double, double, int, int> run_test(graph<T> g, int x, float p);

int main(int argc, char *argv[]) {
    
    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool out = false;
    bool developer = false;
    float p = 0.5;
    int bound = 0;
    int size = 1;
    int seed = -1;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:b:n:p:d")) != -1) {
        switch (arg) {
            case 'h':
                print_help_message();
                return 1;
            case 's':
                seed = stoi(optarg);
                break;
            case 'o':
                fileout = optarg;
                out = true;
                break;
            case 'f':
                filename = optarg;
                random = false;
                break;
            case 'n':
                size = stoi(optarg);
                break;
            case 'b':
                bound = stoi(optarg);
                break;
            case 'p':
                p = stof(optarg);
                break;
            case 'd':
                developer = true;
                break;
            case '?':
                if (optopt == 'c')
                    cerr << "la opcion " << optopt << " necesita un argumento" << endl;
                else if (isprint (optopt))
                    cerr << "Opcion " << optopt << " desconocida" << endl;
                print_help_message();
            default:
                break;
        }
    }

    
    std::tuple<double, double, int,  int> result;

    if (developer) {
        result = run_test_random(seed, p, size, bound);
        if (out) {
            write_to_file(fileout, result);
        }
        return 0;
    }
        
    if (random) {
        for (int i = 1; i <= size; i++) {
            result = run_test_random(seed, p, i, bound);
            if (out) {
                write_to_file(fileout, result);
            }
        }
    }
    else {
        result = run_test_from_file(filename, bound);
        if (out) {
            write_to_file(fileout, result);
        }
    }
    
    
}

void write_to_file(std::string fileout, std::tuple<double, double, int, int> result) {
    ofstream file;
    file.open(fileout, std::fstream::app);
    if (file.fail()) return;
    file << get<2>(result) << "\t" << get<0>(result) << "\t" << get<1>(result) << '\t' <<  get<3>(result) << endl;
    file.close();
}

void print_help_message() {
    cout << "Practica de MARP" << endl << endl;
    cout << "./pracmarp [options]" << endl;
    cout << "Opciones" << endl << endl;
    cout << "-s <seed>\tsemilla para el generador aleatorio, por defecto es aleatoria" << endl;
    cout << "-f <filename>\tNombre del archivo con la grafica en formato DIMACS, al utilizar esta opcion, la opcion -s es ignorada" << endl;
    cout << "-o <filename>\tNombre del archivo de salida, donde se escriben los tiempos de salida" << endl;
    cout << "Random Options" << endl << endl;
    cout << "-n <graphs>[default=1]\tNumero de grafos que se quieren generar" << endl;
    cout << "-d\tDeveloper option, solo genera un grafo de tamaño n[default=1]" << endl;
    cout << "-p <density>[default=0.5]\tProbabilidad de que exista una arista entre dos vertices, valor entre 0-1" << endl;
    cout << endl;
}



template <class A>
void translate_dimacs(graph<A> &graph, std::string filename) {
    std::ifstream file;
    file.open(filename);
    if (file.fail()) return;
    char delimeter;
    std::string s;
    int nodes, edges;
    while(file >> delimeter) {
        switch(delimeter) {
            case 'p':
                file >> s >> nodes >> edges;
                //graph.nodes = nodes;
                //graph.edges = edges; 
                break;
            case 'e':
                file >> nodes >> edges;
                graph.add_edge(std::make_pair(nodes, edges));
                break;
            default:
                getline(file, s);
        }
    }
    file.close();
}

void clean_marks() {
    marks.nodes = 0;
    marks.avg_clocks_node = 0;
    marks.complete_time = 0;
}

template <class T>
std::tuple<double, double, int, int> run_test(graph<T> g, int x, float p) {

    double result_clique = 0;
    clean_marks();
    std::set<T> result;
    
    // Bound selection
    switch(x) {
        case 0:
            bound.opt = optimistic_bound_0;
            bound.pes = pesimistic_bound_0;
            break;
        case 1:
            bound.opt = optimistic_bound_1;
            bound.pes = pesimistic_bound_1;
            break;
        case 2:
            bound.opt = optimistic_bound_2;
            bound.pes = pesimistic_bound_2;
            break;
    }

    

    // Execution of the program
    marks.complete_time = clock();
    result = bnb_max_clique_benchmarked(g);
    marks.complete_time = clock() - marks.complete_time;

    result_clique = ((double) marks.complete_time) / CLOCKS_PER_SEC;
    double time_nodes = ((double) marks.avg_clocks_node / (double) marks.nodes) / CLOCKS_PER_SEC;
    
    cout << "Result: ";
    for (typename std::set<T>::iterator it = result.begin(); it != result.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Avg time: " << result_clique << endl 
    << "Solution size: " << result.size() << endl 
    << "Original Size: " << g.size() << endl 
    << "Nodes explored: " << marks.nodes << endl
    << "Average Time / node: " << time_nodes << endl;
    if (p != -1.0) cout << "Density: " << p << endl;
    cout << endl;

    return std::make_tuple(result_clique, time_nodes, g.size(), marks.nodes);
}

std::tuple<double, double, int, int> run_test_random(int seed, float p, int N, int b) {
    if (seed == -1)
        srand(time(NULL));
    else
        srand(seed);

    graph<int> graph;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (rand() % 100 < p * 100) graph.add_edge({i, j});
        }
    }

    return run_test(graph, b, p);

}

std::tuple<double, double, int,  int> run_test_from_file(std::string filename, int b) {

    // Translate from dimacs convention to graph
    graph<int> p;
    translate_dimacs(p, filename);

    
    return run_test(p, b, -1.0);
}