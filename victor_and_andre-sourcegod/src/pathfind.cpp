/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include <malloc.h>
#include <vector>
#include "pathfind.h"
#include "debug.h"

// Classe ComparadorVertice

ComparadorVertice::ComparadorVertice(Vizinhanca *v) {
    this->v = v;
}

bool ComparadorVertice::operator() (const int &lhs, const int &rhs) const {
    return (v->h(lhs) > v->h(rhs));
}

// Classe Area

Area::Area(int origem, Vizinhanca *v) {

    //char str[200];

    int num_vertices = v->get_num_vertices();
    this->v = v;
    this->destinos = v->get_destinos();
    this->distancias = (int *) malloc(sizeof(int) * num_vertices);
    this->caminhos = (int *) malloc(sizeof(int) * num_vertices);

    ComparadorVertice cv = ComparadorVertice(v);
    fronteira = new std::priority_queue<int, std::vector<int>, ComparadorVertice>(cv);

    // Inicializa as distâncias para infinito.
    int i;
    for (i = 0; i < num_vertices; i++) {
        distancias[i] = PESO_INFINITO;
        caminhos[i] = -1;
    }

    distancias[origem] = 0;
    fronteira->push(origem);

    // Executa A*.
    int destino = -1;
    while (destino == -1 && !fronteira->empty()) {
        int n = fronteira->top();
        fronteira->pop();
        //sprintf(str, "Vou relaxar %d", n);
        //debug(str);
        if (relaxa(n)) destino = n;
    }
    delete fronteira;
    fronteira = NULL;

    // Monta o caminho até o melhor destino encontrado.
    if (destino != -1) {
        int c;
        for (c = destino; c != -1; c = caminhos[c]) {
            caminho_origem_destino.push_front(c);
        }
    }

    free(this->distancias);
    free(this->caminhos);
    this->distancias = NULL;
    this->caminhos = NULL;
}

Area::~Area() {
}

bool Area::relaxa(int no) {
    int num_vizinhos = v->get_num_vizinhos(no);
    int peso_agora = distancias[no];

    std::map<int, float>::iterator it = destinos->find(no);
    if (it != destinos->end()) {
        return true;
    }

    int i;
    for (i = 0; i < num_vizinhos; i++) {
        int destino_aresta = v->get_vizinho(no, i);
        int peso_atual = distancias[destino_aresta];
        int peso_aresta = v->peso_aresta(no, i);
        if (peso_aresta == PESO_INFINITO || peso_agora + peso_aresta >= peso_atual) continue;

        distancias[destino_aresta] = peso_agora + peso_aresta;
        caminhos[destino_aresta] = no;
        fronteira->push(destino_aresta);
    }
    return false;
} 

bool Area::achou_caminho() {
    return !caminho_origem_destino.empty();
}

std::list<int> Area::caminho() {
    return caminho_origem_destino;
}

// Classe VizinhancaGrid

VizinhancaGrid::VizinhancaGrid(int largura_array, int altura_array) {
    this->largura_array = largura_array;
    this->altura_array = altura_array;
}

VizinhancaGrid::~VizinhancaGrid() {
}

int VizinhancaGrid::largura() {
    return this->largura_array;
}

int VizinhancaGrid::altura() {
    return this->altura_array;
}

bool VizinhancaGrid::canto_sup_esq(int v) {
    return v == 0;
}

bool VizinhancaGrid::canto_sup_dir(int v) {
    return v == largura_array - 1;
}

bool VizinhancaGrid::canto_inf_esq(int v) {
    return v == (altura_array - 1) * largura_array;
}

bool VizinhancaGrid::canto_inf_dir(int v) {
    return v == (altura_array * largura_array) - 1;
}

bool VizinhancaGrid::borda_sup(int v) {
    return v < largura_array - 1 && !canto_sup_dir(v);
}

bool VizinhancaGrid::borda_inf(int v) {
    return v > ((altura_array - 1) * largura_array) && !canto_sup_dir(v);
}

bool VizinhancaGrid::borda_esq(int v) {
    return v % largura_array == 0 && !canto_sup_esq(v) && !canto_inf_esq(v);
}

bool VizinhancaGrid::borda_dir(int v) {
    return v % largura_array == largura_array - 1 && !canto_sup_dir(v) && !canto_inf_dir(v);
}

int VizinhancaGrid::get_num_vizinhos(int vertice) {
    if (canto_sup_esq(vertice) || canto_inf_esq(vertice) || canto_sup_dir(vertice) || canto_inf_dir(vertice)) return 3;
    if (borda_sup(vertice) || borda_inf(vertice) || borda_esq(vertice) || borda_dir(vertice)) return 5;
    return 8;
}

#define VIZINHO_1 (vertice + largura_array - 1)
#define VIZINHO_2 (vertice + largura_array)
#define VIZINHO_3 (vertice + largura_array + 1)
#define VIZINHO_4 (vertice - 1)
#define VIZINHO_6 (vertice + 1)
#define VIZINHO_7 (vertice - largura_array - 1)
#define VIZINHO_8 (vertice - largura_array)
#define VIZINHO_9 (vertice - largura_array + 1)

int VizinhancaGrid::tipo_vizinho(int vertice, int vizinho) {
    if (vizinho == VIZINHO_1) return 1;
    if (vizinho == VIZINHO_2) return 2;
    if (vizinho == VIZINHO_3) return 3;
    if (vizinho == VIZINHO_4) return 4;
    if (vizinho == VIZINHO_6) return 6;
    if (vizinho == VIZINHO_7) return 7;
    if (vizinho == VIZINHO_8) return 8;
    if (vizinho == VIZINHO_9) return 9;

    char str[50];
    sprintf(str, "Nao sao vizinhos %d e %d", vertice, vizinho);
    debug(str);
    return -1;
}

static int select_vizinho_3(int indice_vizinho, int v1, int v2, int v3) {
    switch (indice_vizinho) {
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        default:
            char str[200];
            sprintf(str, "Parametro invalido %d em select_vizinho_3, selecao: %d, %d, %d", indice_vizinho, v1, v2, v3);
            debug(str);
            return -1;
    }
}

static int select_vizinho_5(int indice_vizinho, int v1, int v2, int v3, int v4, int v5) {
    switch (indice_vizinho) {
        case 0: return v1;
        case 1: return v2;
        case 2: return v3;
        case 3: return v4;
        case 4: return v5;
        default:
            char str[200];
            sprintf(str, "Parametro invalido %d em select_vizinho_5, selecao: %d, %d, %d, %d, %d", indice_vizinho, v1, v2, v3, v4, v5);
            debug(str);
            return -1;
    }
}

int VizinhancaGrid::get_vizinho(int vertice, int indice_vizinho) {
    if (indice_vizinho < 0) debug("Parametro negativo em get_vizinho");
    if (canto_sup_esq(vertice)) return select_vizinho_3(indice_vizinho, VIZINHO_6, VIZINHO_2, VIZINHO_3);
    if (canto_sup_dir(vertice)) return select_vizinho_3(indice_vizinho, VIZINHO_4, VIZINHO_1, VIZINHO_2);
    if (canto_inf_esq(vertice)) return select_vizinho_3(indice_vizinho, VIZINHO_8, VIZINHO_9, VIZINHO_6);
    if (canto_inf_dir(vertice)) return select_vizinho_3(indice_vizinho, VIZINHO_7, VIZINHO_8, VIZINHO_4);

    if (borda_sup(vertice)) return select_vizinho_5(indice_vizinho, VIZINHO_4, VIZINHO_6, VIZINHO_1, VIZINHO_2, VIZINHO_3);
    if (borda_inf(vertice)) return select_vizinho_5(indice_vizinho, VIZINHO_7, VIZINHO_8, VIZINHO_9, VIZINHO_4, VIZINHO_6);
    if (borda_esq(vertice)) return select_vizinho_5(indice_vizinho, VIZINHO_8, VIZINHO_9, VIZINHO_6, VIZINHO_2, VIZINHO_3);
    if (borda_dir(vertice)) return select_vizinho_5(indice_vizinho, VIZINHO_7, VIZINHO_8, VIZINHO_4, VIZINHO_1, VIZINHO_2);

    // Vértice normal:
    if (indice_vizinho > 7) debug("Parametro invalido em get_vizinho");
    int v[] = {VIZINHO_7, VIZINHO_8, VIZINHO_9, VIZINHO_4, VIZINHO_6, VIZINHO_1, VIZINHO_2, VIZINHO_3};
    return v[indice_vizinho];
}

int VizinhancaGrid::peso_aresta(int vertice, int indice_vizinho) {
    int vertice_vizinho = get_vizinho(vertice, indice_vizinho);
    if (is_obstaculo(vertice_vizinho)) return PESO_INFINITO;
    switch (tipo_vizinho(vertice, vertice_vizinho)) {
        case 2: case 4: case 6: case 8: return 10;
        case 1: return (is_obstaculo(VIZINHO_2) && is_obstaculo(VIZINHO_4)) ? PESO_INFINITO : 14;
        case 3: return (is_obstaculo(VIZINHO_2) && is_obstaculo(VIZINHO_6)) ? PESO_INFINITO : 14;
        case 7: return (is_obstaculo(VIZINHO_8) && is_obstaculo(VIZINHO_4)) ? PESO_INFINITO : 14;
        case 9: return (is_obstaculo(VIZINHO_8) && is_obstaculo(VIZINHO_6)) ? PESO_INFINITO : 14;
        default: return -1;
    }
}

int VizinhancaGrid::get_num_vertices() {
    return largura_array * altura_array;
}

float VizinhancaGrid::h(int vertice) {
   int vx = vertice % largura_array;
   int vy = vertice / largura_array;
   int min = PESO_INFINITO;
   std::map<int, float>::iterator it;
   for (it = destinos_array.begin(); it != destinos_array.end(); it++) {
       int dest = (*it).first;
       float peso = (*it).second;
       if (peso <= 0.0) continue;
       int px = dest % largura_array;
       int py = dest / largura_array;
       int dx = px - vx;
       int dy = py - vy;
       int meu_h = (dx * dx + dy * dy) / peso;
       if (min > meu_h) min = meu_h;
   }
   return min;
}

std::map<int, float> *VizinhancaGrid::get_destinos() {
    return &destinos_array;
}

void VizinhancaGrid::set_obstaculo(int vertice) {
    obstaculos_array.insert(vertice);
}

bool VizinhancaGrid::is_obstaculo(int vertice) {
    return obstaculos_array.find(vertice) != obstaculos_array.end();
}

void VizinhancaGrid::set_destino(int vertice, float valor) {
    destinos_array[vertice] = valor;
}

bool VizinhancaGrid::is_destino(int vertice) {
    std::map<int, float>::iterator it = destinos_array.find(vertice);
    return it != destinos_array.end();
}
 
float VizinhancaGrid::get_peso(int vertice) {
    return destinos_array[vertice];
}
