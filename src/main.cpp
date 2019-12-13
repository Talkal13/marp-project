#include <iostream>
#include "../headers/grafo_kruskal.h"
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <fstream>
#include <time.h>
#include <unistd.h>



using namespace std;

void run_test_random(int seed, float p, int N, unsigned int min, unsigned int max, bool verbose);
void print_help_message();

int main(int argc, char *argv[]) {
    
    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool out = false;
    bool developer = false;
    bool verbose = false;
    float p = 0.5;
    int size = 1;
    int seed = -1;
    int min = 0;
    int max = 100;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:n:p:dm:M:v")) != -1) {
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
            case 'p':
                p = stof(optarg);
                break;
            case 'd':
                developer = true;
                break;
            case 'm':
                min = stoi(optarg);
                break;
            case 'M':
                max = stoi(optarg);
                break;
            case 'v':
                verbose = true;
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
        run_test_random(seed, p, size, min, max, verbose);
        if (out) {
            //write_to_file(fileout, result);
        }
        return 0;
    }
        
    if (random) {
        for (int i = 1; i <= size; i++) {
            run_test_random(seed, p, i, min, max, verbose);
            if (out) {
                //write_to_file(fileout, result);
            }
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


void run_test_random(int seed, float p, int N, unsigned int min, unsigned int max, bool verbose) {
    if (seed == -1)
        srand(time(NULL));
    else
        srand(seed);

    grafo_kruskal<int, unsigned int> grafo;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (rand() % 100 < p * 100) {
                arista<int, unsigned int> a;
                a.vertices = {i, j};
                a.valor = (rand() % (max - min)) + min;
                grafo.insertar_arista(a);
            }
        }
    }
    if (verbose) {
        cout << grafo << endl;
        std::vector<arista<int, unsigned int>> kruskal = grafo.kruskal_A();
        for (auto it = kruskal.begin(); it != kruskal.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl;
        
    }
    return;
}
