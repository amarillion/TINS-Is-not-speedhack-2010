/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "readsource.h"
#include "debug.h"

std::list<std::list<Character*>*> *get_source_by_line(const char *file, const int linhainicial) {
    char str[100];
    sprintf(str, "Carregando fonte %s, linha inicial %d", file, linhainicial);
    debug(str);

    std::list<std::list<Character*>*> *l = new std::list<std::list<Character*>*>();

    int maxlinhas = 27;
    int linhaatual = 0;
    int y = 0;
    std::ifstream ifs(file, std::ifstream::in);
    while (ifs.good() && maxlinhas >= 0) {
        bool linhapermitida = (linhaatual >= linhainicial);

        std::list<Character*> *a = new std::list<Character*>();

        if (linhapermitida) {
            l->push_back(a); // push_front deixa a ordem das linhas invertida.
        }

        int x = 0;
        char c;
        for (c = (char) ifs.get(); ifs.good() && c != '\n'; c = ifs.get()) {
            if (linhapermitida) {
                a->push_back(new Character(x, y, c)); // push_front deixa a linha de trás para frente.
                x++;
            }
        }

        if (linhapermitida) {
            y++;
            maxlinhas--;
        }

        linhaatual++;
    }
    ifs.close();

    debug("carregado!");
    return l;
}