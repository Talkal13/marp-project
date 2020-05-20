#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <fstream>
#include <time.h>
#include <vector>
#include <unistd.h>
#include "../headers/bounds.h"
#include "../headers/globals.h"
#include "../headers/envases_opt.h"


using namespace std;

benchmark marks;
bounds bound;

std::tuple<double, double, int, int> run_test_from_file(std::string filename, int b);
std::tuple<double, double, int, int> run_test_random(int, int, double, double);
void write_to_file(std::string fileout, std::tuple<double, double, int, int> result);
void print_help_message();
std::tuple<double, double, int, int> run_test(std::vector<double>, double);
void select_bound(unsigned x);

int main(int argc, char *argv[]) {
    
    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool out = false;
    bool developer = false;
    int lower = 0;
    int N = 1;
    double E = 10;
    int seed = -1;
    unsigned x = 0;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:b:n:p:de:x:")) != -1) {
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
                N = stoi(optarg);
                break;
            case 'b':
                x = stoi(optarg);
                break;
            case 'x':
                lower = stod(optarg);
                break;
            case 'e':
                E = stod(optarg);
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

    select_bound(x);
    std::tuple<double, double, int,  int> result;

    if (developer) {
        result = run_test_random(seed, N, E, lower);
        if (out) {
            write_to_file(fileout, result);
        }
        return 0;
    }
        
    if (random) {
        for (int i = 1; i <= N; i++) {
            result = run_test_random(seed, i, E, lower);
            if (out) {
                write_to_file(fileout, result);
            }
        }
    }
    else {
        result = run_test_from_file(filename, lower);
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

void select_bound(unsigned x) {
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
    return;
}


void clean_marks() {
    marks.nodes = 0;
    marks.avg_clocks_node = 0;
    marks.complete_time = 0;
}


std::tuple<double, double, int, int> run_test(std::vector<double> V, double E) {

    double result_clique = 0;
    clean_marks();
    std::vector<int> result;

    

    // Execution of the program
    marks.complete_time = clock();
    result = brb_envases(E, V);
    marks.complete_time = clock() - marks.complete_time;

    result_clique = ((double) marks.complete_time) / CLOCKS_PER_SEC;
    double time_nodes = ((double) marks.avg_clocks_node / (double) marks.nodes) / CLOCKS_PER_SEC;
    
    cout << "Result: ";
    for (typename std::vector<int>::iterator it = result.begin(); it != result.end(); ++it) {
        cout << *it << " ";
    }
    cout << endl;

    int max = *std::max_element(result.begin(), result.end());

    cout << "Avg time: " << result_clique << endl 
    << "Solution size: " << (max + 1) << endl 
    << "Original Size: " << V.size() << endl 
    << "Nodes explored: " << marks.nodes << endl
    << "Average Time / node: " << time_nodes << endl;
    cout << endl;

    return std::make_tuple(result_clique, time_nodes, V.size(), marks.nodes);
}

std::tuple<double, double, int, int> run_test_random(int seed, int N, double E, double l_b) {
    if (seed == -1)
        srand(time(NULL));
    else
        srand(seed);

    std::vector<double> volumes;


    for (int i = 1; i <= N; i++) {
        volumes.push_back(rand() % int(E) + l_b);
    }

    return run_test(volumes, E);

}

std::tuple<double, double, int,  int> run_test_from_file(std::string filename, int b) {
    return std::tuple<double, double, int, int>();
}