/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include "readsource.h"
#include "test.h"

#if RUN_TEST

void readsource_test() {
    std::list<std::list<Character *>*>* source = get_source_by_line("makefile", 0);

    std::ofstream ofs("readsource_test_result.txt");
    ofs << "a" << std::endl;

    int i = 1;
    std::list<std::list<Character *>*>::iterator it1;
    for (it1 = source->begin(); it1 != source->end(); it1++) {

        ofs << i << ' ';
        std::list<Character *>* linha = *it1;

        std::list<Character *>::iterator it2;
        for (it2 = linha->begin(); it2 != linha->end(); it2++) {
            ofs << (*it2)->get_c();
        }
        ofs << std::endl;

        i++;
    }

    ofs.close();
}

#endif /* RUN_TEST */