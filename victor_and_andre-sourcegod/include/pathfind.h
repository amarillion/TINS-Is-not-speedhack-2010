/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#ifndef __SENTINELA_PATHFIND
#define __SENTINELA_PATHFIND

#include <list>
#include <map>
#include <queue>
#include <set>

#define PESO_INFINITO 0x7FFFFFFF

class Vizinhanca {
  public:
    virtual int get_num_vizinhos(int vertice) = 0;
    virtual int get_vizinho(int vertice, int indice_vizinho) = 0;
    virtual int peso_aresta(int vertice, int indice_vizinho) = 0;
    virtual int get_num_vertices() = 0;
    virtual float h(int vertice);
    virtual std::map<int, float> *get_destinos();
};

class PathFinder {
  public:
    virtual Vizinhanca *get_grid() = 0;
};

class ComparadorVertice {
  private:
    Vizinhanca *v;

  public:
    ComparadorVertice(Vizinhanca *v);
    bool operator() (const int& lhs, const int& rhs) const;
};

class Area {
  private:
    Vizinhanca *v;
    int *distancias;
    int *caminhos;
    std::list<int> caminho_origem_destino;
    std::priority_queue<int, std::vector<int>, ComparadorVertice> *fronteira;
    std::map<int, float> *destinos;

    bool relaxa(int no);

  public:
    Area(int origem, Vizinhanca *v);
    virtual ~Area();
    bool achou_caminho();
    std::list<int> caminho();
};

class VizinhancaGrid : public virtual Vizinhanca {
  private:
    int largura_array;
    int altura_array;
    std::set<int> obstaculos_array;
    std::map<int, float> destinos_array;

    bool canto_sup_esq(int v);
    bool canto_sup_dir(int v);
    bool canto_inf_esq(int v);
    bool canto_inf_dir(int v);
    bool borda_sup(int v);
    bool borda_inf(int v);
    bool borda_esq(int v);
    bool borda_dir(int v);
    int tipo_vizinho(int vertice, int vizinho);
    bool diagonal(int vertice, int vizinho);

  public:
    VizinhancaGrid(int largura_array, int altura_array);
    virtual ~VizinhancaGrid();
    int get_num_vizinhos(int vertice);
    int get_vizinho(int vertice, int indice_vizinho);
    int peso_aresta(int vertice, int indice_vizinho);
    int get_num_vertices();
    float h(int vertice);
    std::map<int, float> *get_destinos();

    void set_obstaculo(int vertice);
    bool is_obstaculo(int vertice);
    void set_destino(int vertice, float peso);
    bool is_destino(int vertice);
    float get_peso(int vertice);
    int largura();
    int altura();
};

#endif /* __SENTINELA_PATHFIND */