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
#include <thread>         // http://www.cplusplus.com/reference/thread/thread/
#include "../headers/bounds.h"


using namespace std;


int N_EDGES = 1000;
int N_VERTICES = 100;

bool parse(int args, char *argv[], std::string &filename, string &fileout, int &seed, bool &random);
template <class A>
void translate_dimacs(graph<A> &graph, std::string filename);
std::tuple<double, double, int> run_test_from_file(std::string filename, int b);
std::tuple<double, double, int> run_test_random(int seed, bool define, int N, int b);
void write_to_file(std::string fileout, std::tuple<double, double, int> result);
void print_help_message();
template <class T>
std::tuple<double, double, int> run_test(graph<T> g, int x);

int main(int argc, char *argv[]) {
    

    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool define = false;
    bool out = false;
    bool developer = false;
    int bound = 0;
    int size = 1;
    int seed = 0;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:b:n:v:e:d")) != -1) {
        switch (arg) {
            case 'h':
                print_help_message();
                return 1;
            case 's':
                seed = stoi(optarg);
                define = true;
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
            case 'v':
                N_VERTICES = stoi(optarg);
                break;
            case 'e':
                N_EDGES = stoi(optarg);
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

    
    std::tuple<double, double,  int> result;
        
    if (random) {
        for (int i = 0; i < size; i++) {
            result = run_test_random(seed, define, i, bound);
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

void write_to_file(std::string fileout, std::tuple<double, double, int> result) {
    ofstream file;
    file.open(fileout, std::fstream::app);
    if (file.fail()) return;
    file << get<2>(result) << "\t" << get<0>(result) << "\t" << get<1>(result) << endl;
    file.close();
}

void print_help_message() {
    cout << "Practica de MARP" << endl << endl;
    cout << "./pracmarp [options]" << endl;
    cout << "Opciones" << endl << endl;
    cout << "-s <seed>\tsemilla para el generador aleatorio, por defecto es aleatoria" << endl;
    cout << "-f <filename>\tNombre del archivo con la grafica en formato DIMACS, al utilizar esta opcion, la opcion -s es ignorada" << endl;
    cout << "-o <filename>\tNombre del archivo de salida, donde se escriben los tiempos de salida" << endl;
    cout << "-l <epoch>[default=100]\tNumero de interaciones por cada test.\tDefault: 100" << endl << endl;
    cout << "Random Options" << endl << endl;
    cout << "-n <graphs>[default=1]\tNumero de grafos que se quieren generar, solo funciona con -s" << endl;
    cout << "-v <vertices>[default=1000]\tRango maximo de nodos que ha de tener el grafo, esta entre [0, verices)" << endl;
    cout << "-e <edges>[default=100]\tRango de edges que ha de tener el grafo, esta entre [edges / 2, edges)" << endl;
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


template <class T>
std::tuple<double, double, int> run_test(graph<T> g, int x) {

    //cout << g << endl;
    double result_clique = 0;
    benchmark b;
    std::set<T> result;

    b.complete_time = clock();
    switch(x) {
        case 0:
            result = bnb_increment_max_clique_benchmarks(g, b, optimistic_bound_0, pesimistic_bound_0);
            break;
        case 1:
            result = bnb_increment_max_clique_benchmarks(g, b, optimistic_bound_1, pesimistic_bound_1);
            break;
        case 2:
            result = bnb_increment_max_clique_benchmarks(g, b, optimistic_bound_2, pesimistic_bound_2);
            break;
    }
    
    b.complete_time = clock() - b.complete_time;

    result_clique = ((double) b.complete_time) / CLOCKS_PER_SEC;
    double time_nodes = ((double) b.avg_clocks_node / (double) b.nodes) / CLOCKS_PER_SEC;
    
    cout << "result: ";
    for (typename std::set<T>::iterator it = result.begin(); it != result.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    cout << "Avg time: " << result_clique << endl 
    << "Solution size: " << result.size() << endl 
    << "Original Size: " << g.size() << endl 
    << "Nodes explored: " << b.nodes << endl
    << "Average Time / node: " << time_nodes << endl
    << endl;

    return std::make_tuple(result_clique, b.time_nodes, g.size());
}

std::tuple<double, double, int> run_test_random(int seed, bool define, int N, int b) {
    if (!define)
        srand(time(NULL));
    else
        srand(seed);

    graph<int> p;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (i % j == 0) p.add_edge(std::make_pair(i, j));
        }
    }

    return run_test(p, b);

}

std::tuple<double, double, int> run_test_from_file(std::string filename, int b) {

    // Translate from dimacs convention to graph
    graph<int> p;
    translate_dimacs(p, filename);

    
    return run_test(p, b);
}