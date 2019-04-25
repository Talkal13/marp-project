#include "../headers/digraph.h"
#include <string>
#include <sstream>
#include <utility>
#include <iterator>
#include <fstream>
#include <time.h>
#include <unistd.h>


using namespace std;


int EPOCH_SIZE = 100;
int N_EDGES = 1000;
int N_VERTICES = 100;

bool parse(int args, char *argv[], std::string &filename, string &fileout, int &seed, bool &random);
template <class A>
void translate_dimacs(digraph<A> &graph, std::string filename);
std::tuple<double, double, double, int> run_test_from_file(std::string filename);
std::tuple<double, double, double, int> run_test_random(int seed, bool define, int N);
void write_to_file(std::string fileout, std::tuple<double, double, double, int> result);
template <class B>
std::tuple<double, double, double, int> run_test(digraph<B> &digraph);
void print_help_message();

int main(int argc, char *argv[]) {
    

    // Parse comand line arguments
    std::string filename;
    std::string fileout;
    bool random = true;
    bool define = false;
    bool out = false;
    int size = 1;
    int seed = 0;
    int arg;
    while ((arg = getopt(argc, argv, "hs:o:f:l:n:v:e:")) != -1) {
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
            case 'l':
                EPOCH_SIZE = stoi(optarg);
                break;
            case 'n':
                size = stoi(optarg);
                break;
            case 'v':
                N_VERTICES = stoi(optarg);
                break;
            case 'e':
                N_EDGES = stoi(optarg);
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
    
    std::tuple<double, double, double, int> result;
    if (random) {
        for (int i = 0; i < size; i++) {
            result = run_test_random(seed, define, (rand() % N_EDGES) + i * N_EDGES);
            if (out) {
                write_to_file(fileout, result);
            }
        }
    }
    else {
        result = run_test_from_file(filename);
        if (out) {
            write_to_file(fileout, result);
        }
    }
    
    
    
}

void write_to_file(std::string fileout, std::tuple<double, double, double, int> result) {
    ofstream file;
    file.open(fileout, std::fstream::app);
    if (file.fail()) return;
    file << get<3>(result) << "\t" << get<0>(result) << "\t" << get<1>(result) << "\t" << get<2>(result) << endl;
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
void translate_dimacs(digraph<A> &graph, std::string filename) {
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
                graph.n_nodes = nodes;
                graph.n_edges = edges; 
                break;
            case 'e':
                file >> nodes >> edges;
                graph.add_edge(nodes, edges);
                break;
            default:
                getline(file, s);
        }
    }
    file.close();
}

std::tuple<double, double, double, int> run_test_random(int seed, bool define, int N) {
    if (!define)
        srand(time(NULL));
    else
        srand(seed);

    digraph<int> p;

    for (int i = 0; i < N; i++) {
        int v = (rand() % N);
        int e = (rand() % N);
        if (!p.has_node(v))
            p.n_nodes++;
        if (!p.has_node(e))
            p.n_nodes++;
        p.n_edges++;
        p.add_edge(v, e);
    }

    return run_test(p);

}

std::tuple<double, double, double, int> run_test_from_file(std::string filename) {

    // Translate from dimacs convention to digraph
    digraph<int> p;
    translate_dimacs(p, filename);

    
    return run_test(p);
}

template <class B>
std::tuple<double, double, double, int> run_test(digraph<B> &p) {

    double result_topological_sort = 0;
    double result_tarjan = 0;
    double result_cycles = 0;

    clock_t t[3];
    bool cycles;
    clock_t t_avg = 0;

    for (int i = 0; i < 3; i++) {
        t[i] = clock();
        for (int j = 0; j < EPOCH_SIZE; j++)
            cycles = p.dfs_search_cycles();
        t_avg += (clock() - t[i]) / EPOCH_SIZE;
    }

    result_cycles = ((double) t_avg) / CLOCKS_PER_SEC * 1.0e3;

    if (cycles) {
        // Strong components
        std::vector<std::vector<Node<int>>> vec;
        clock_t t_avg = 0;
        for (int i = 0; i < 3; i++) {
            t[i] = clock();
            for (int j = 0; j < EPOCH_SIZE; j++)
               vec = p.tarjan();
            t_avg += (clock() - t[i]) / EPOCH_SIZE;
        }

        result_tarjan = ((double) t_avg) / CLOCKS_PER_SEC * 1.0e3;
       
        
        for (int i = 0; i < vec.size(); i++) {
            cout << "Cycle number " << i << endl;
            for (int j = vec[i].size() - 1; j > 0; j--) {
                cout << vec[i][j]._node << " -> ";
            }
            cout << vec[i][0]._node << endl;
        }
        cout << "Time to compleate tarjan: " << result_tarjan << endl;
    }
    else {

        std::stack<Node<int>> s;
        
        clock_t t_avg = 0;
        for (int i = 0; i < 3; i++) {
            t[i] = clock();
            for (int j = 0; j < EPOCH_SIZE ; j++)
                s = p.dfs_topological_sort();
            t_avg += (clock() - t[i]) / EPOCH_SIZE;
        }

        result_topological_sort = ((double) t_avg) / CLOCKS_PER_SEC * 1.0e3;


        while(s.size() > 1) {
            cout << s.top()._node << " - ";
            s.pop();
        }
        cout << s.top()._node  << endl;

        cout << "Time to compleate topological sort: " << result_topological_sort << endl;
    }
    
    cout << "Time to complete cycles search: " << result_cycles << endl;
    return std::make_tuple(result_cycles, result_topological_sort, result_tarjan, p.n_nodes);
}
