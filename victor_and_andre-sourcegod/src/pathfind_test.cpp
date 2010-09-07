/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "pathfind.h"
#include "test.h"

#if RUN_TEST

static int num_vizinhos[7] = {2, 2, 1, 1, 2, 1, 0};
static int vizinhos[7][2] = {
  {1, 2},
  {2, 4},
  {3, -1},
  {5, -1},
  {5, 6},
  {6, -1},
  {-1, -1}
};
static int pesos[7][2] = {
  {5, 3},
  {3, 8},
  {4, -1},
  {2, -1},
  {5, 3},
  {3, -1},
  {-1, -1}
};
static const char *nomes[7] = {"a", "b", "c", "d", "e", "f", "g"};

class MinhaVizinhanca : public virtual Vizinhanca {

  private:
    std::map<int, float> d;

  public:
    MinhaVizinhanca() {
        d[6] = 1.0;
    }

    int get_num_vizinhos(int vertice) {
        return num_vizinhos[vertice];
    }

    int get_vizinho(int vertice, int indice_vizinho) {
        return vizinhos[vertice][indice_vizinho];
    }

    int peso_aresta(int vertice, int indice_vizinho) {
        return pesos[vertice][indice_vizinho];
    }

    int get_num_vertices() {
        return 7;
    }

    const char *nome(int vertice) {
        return nomes[vertice];
    }

    float h(int vertice) {
        return 0.0;
    }

    std::map<int, float> *get_destinos() {
        return &d;
    }
};

void pathfind_test() {
    MinhaVizinhanca v;
    Area *area = new Area(0, &v);

    std::ofstream ofs("pathfind_test_result.txt");

    if (area->achou_caminho()) {
        ofs << "Achou" << std::endl;

        std::list<int> caminho = area->caminho();

        std::list<int>::iterator it;
        for (it = caminho.begin(); it != caminho.end(); it++) {
           ofs << v.nome(*it) << std::endl;
        }
    } else {
        ofs << "Nao Achou" << std::endl;
    }

    ofs.close();

    delete area;
}

#endif /* RUN_TEST */