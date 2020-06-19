#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ansicolor.h"

typedef struct tester{
  int pruebas_totales;
  int pruebas_pasadas;
} tester_t;


tester_t* tester_crear(){
  tester_t* tester = calloc(1, sizeof(tester_t));
  tester -> pruebas_totales = 0;
  tester -> pruebas_pasadas = 0;

  return tester;
}

void tester_afirmar(tester_t* tester, char* descripcion, bool valor){
  if(valor) (tester -> pruebas_pasadas)++;
  (tester -> pruebas_totales)++;

  printf("\tPrueba %s: %s%s%s\n", descripcion, valor ? ANSI_COLOR_GREEN:ANSI_COLOR_RED, valor ? "EXITOSA":"FALLADA", ANSI_RESET);
}

void tester_avisar(tester_t* tester, char* descripcion, bool afirmacion){
  if(afirmacion)
    printf("\t%s%s%s\n", ANSI_COLOR_YELLOW, descripcion, ANSI_RESET);
}

void tester_mostrar_estadisticas(tester_t* tester){
  printf("PRUEBAS PASADAS: %i/%i\n", tester -> pruebas_pasadas, tester -> pruebas_totales);
  if(tester -> pruebas_pasadas == tester -> pruebas_totales)
    printf("Todo en orden :) \n");
  else
     printf("Hay %i %s :(\n", tester -> pruebas_totales - tester -> pruebas_pasadas, (tester -> pruebas_totales - tester -> pruebas_pasadas) == 1 ? "error" : "errores");
}

void tester_destruir(tester_t* tester){
  free(tester);
}
