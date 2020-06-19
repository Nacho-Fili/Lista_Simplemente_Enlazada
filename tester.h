#ifndef __TESTER_H__
#define __TESTER_H__

#include <stdbool.h>

typedef struct tester tester_t;


tester_t* tester_crear();

void tester_afirmar(tester_t* tester, char* descripcion, bool valor);

void tester_mostrar_estadisticas(tester_t* tester);

void tester_destruir(tester_t* tester);


#endif /*__TESTER_H__*/
