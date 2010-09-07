/*
 * TINS 2010 - The source god
 * Authors: Victor Williams Stafusa da Silva
 *          André Luiz Pereira Álvares
 */
#include <stdio.h>
#include "debug.h"

void debug_clear() {
    std::ofstream ofs("debug.txt");
    ofs.close();
}

void debug(const char *texto) {

//#if RUN_TEST
    std::ofstream ofs("debug.txt", std::ifstream::app);
    ofs << texto << std::endl;
    ofs.close();
//#endif /* RUN_TEST */

}