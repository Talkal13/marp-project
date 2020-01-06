#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <fstream>
#include <time.h>
#include <unistd.h>

#include "../headers/grafo_kruskal.h"
#include "../headers/particion.h"


using namespace std;

double run_test_random(int seed, float p, unsigned int N, unsigned int min, unsigned int max, bool verbose, int ciclos);
void write_to_file(std::string fileout, double result, unsigned int size);
void print_help_message();

int main(int argc, char *argv[]) {
    

    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool out = false;
    bool developer = false;
    bool verbose = false;
    int ciclos = 1;
    float p = 0.5;
    int size = 1;
    int seed = -1;
    int min = 0;
    int max = 100;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:n:p:dm:M:vc:")) != -1) {
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
            case 'c':
                ciclos = stoi(optarg);
                break;
            case '?':
                print_help_message();
                return -1;
            default:
                print_help_message();
                break;
        }
    }

    
    std::tuple<double, double, int,  int> result;

    if (developer) {
        double result = run_test_random(seed, p, size, min, max, verbose, ciclos);
        if (out) {
            write_to_file(fileout, result, size);
        }
        return 0;
    }
        
    if (random) {
        for (int i = 1; i <= size; i++) {
            double result = 0;
            for (int i = 0; i < 3; i++)
                result += run_test_random(seed, p, i, min, max, verbose, ciclos);
            result = result / 3; 
            if (out) {
                write_to_file(fileout, result, i);
            }
        }
    }
    
    
}

void write_to_file(std::string fileout, double result, unsigned int size) {
    ofstream file;
    file.open(fileout, std::fstream::app);
    if (file.fail()) return;
    file << size << "\t" << result << endl;
    file.close();
}

void print_help_message() {
    cout << "Practica de MARP" << endl << endl;
    cout << "Uso: ./pracmarp [options]" << endl << endl;
    cout << "Opciones" << endl << endl;
    cout << "-n <graphs>[default=1]\tNumero de grafos que se quieren generar" << endl;
    cout << "-o <filename>\tNombre del archivo de salida, donde se escriben los tiempos de salida" << endl;
    cout << "-v\tVerbose, el programa enseña el grafo y la solucion por la salida estandar" << endl;
    cout << "-c\tCiclos, numero de ejecuciones del algoritmo en una iteracion" << endl << endl;
    cout << "Opciones de aleatoriedad" << endl << endl;
    cout << "-s <seed>\tsemilla para el generador aleatorio, por defecto es aleatoria" << endl;
    cout << "-p <density>[default=0.5]\tProbabilidad de que exista una arista entre dos vertices, valor entre 0-1" << endl;
    cout << endl << "Otras opciones" << endl << endl;
    cout << "-d\tDeveloper option, solo genera un grafo de tamaño n[default=1]" << endl;
    cout << endl;
}


double run_test_random(int seed, float p, unsigned int N, unsigned int min, unsigned int max, bool verbose, int ciclos) {
    if (seed == -1)
        srand(time(NULL));
    else
        srand(seed);

    grafo_kruskal<int, unsigned int> grafo;
    particion<int> par;
    for (unsigned int i = 0; i < N; i++) {
        par.incluir_elemento(i);
    }

    while(par.representantes() > 1) {
        if (rand() % 100 < p * 100) {
            int i = rand() % N;
            int j = rand() % N;
            arista<int, unsigned int> a;
            a.vertices = {i, j};
            a.valor = (rand() % (max - min)) + min;
            grafo.insertar_arista(a);
            if (par.encontrar_set(i) != par.encontrar_set(j))
                par.unir_set(i, j);
        }
    }
    std::vector<arista<int, unsigned int>> kruskal;
    clock_t begin = clock();
    for (int i = 0; i < ciclos; i++)
        kruskal = grafo.kruskal_A();
    clock_t end = clock();

    double total_time = (double) (end - begin) / ciclos;

    if (verbose) {
        cout << grafo << endl << endl;
        cout << "Kruskal: ";
        for (auto it = kruskal.begin(); it != kruskal.end(); ++it) {
            cout << *it << " ";
        }
        cout << endl << endl << "N: " << kruskal.size() << endl;
        cout << endl;
        
    }
    return ((double) total_time) / CLOCKS_PER_SEC;
}

