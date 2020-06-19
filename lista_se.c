#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "tester.h"
#include "lista.h"


#define ERROR -1
#define EXITO 0
#define LINEA_DIVISORIA "--------------------------------------------------------------------------------------\n"
#define MAX_DESCRIPCION 50
#define FORMATO_PRUEBAS_GENERALES "\t\t______________________\n\t\t|                    |\n\t\t|      GENERALES     |\n\t\t|____________________|\n"
#define FORMATO_PRUEBAS_LISTA "\t\t______________________\n\t\t|                    |\n\t\t|        LISTA       |\n\t\t|____________________|\n"
#define FORMATO_PRUEBAS_COLA "\t\t______________________\n\t\t|                    |\n\t\t|        COLA        |\n\t\t|____________________|\n"
#define FORMATO_PRUEBAS_PILA "\t\t______________________\n\t\t|                    |\n\t\t|        PILA        |\n\t\t|____________________|\n"
#define FORMATO_PRUEBAS_ITERADOR "\t\t______________________\n\t\t|                    |\n\t\t|      ITERADOR      |\n\t\t|____________________|\n"

/*
 * recibe dos void* que deben ser int* para que la función imprima por pantalla el
 * doble del valor del primero antecedido por el número al que apunta contexto. Además, lo aumenta en uno.
 */
void duplicar_valor(void* elemento, void* contexto){
  int* elemento_int = elemento;
  *elemento_int *= 2;
  int* contexto_int = contexto;

  printf("\t\tEl doble del valor del elemento %i se modificó a %i\n", *contexto_int, *elemento_int);
  (*contexto_int)++;
}

/*
 *  Recibe un char* en el que concatena los strings enviados por ...
 * La variable nargs representa la cantidad de strings a concatenar
 * devuelve el puntero a dest.
 */
char* armar_string(char* dest, int nargs, ... ){

  if(*dest != '\0'){
    size_t letras = strlen(dest);
    memset(dest, 0, letras);
  }

	va_list args;
	va_start(args, nargs);

  strcpy(dest, va_arg(args, char*));

	for(int i = 1; i < nargs; i++){
	  strcat(dest, va_arg(args, char*));
	}

	va_end(args);
  return dest;
}

void probar_crear(tester_t* tester){
  lista_t* lista = lista_crear();
  tester_afirmar(tester, "Lista se crea correctamente", lista);
  tester_afirmar(tester, "Lista se crea vacía", lista_vacia(lista));
  tester_afirmar(tester, "Lista se crea con 0 elementos", lista_elementos(lista) == 0);
  lista_destruir(lista);
}

void probar_lista_vacia(tester_t* tester){

  lista_t* lista = lista_crear();
  if(!lista)
    return;

  int a = 2;

  tester_afirmar(tester, "Lista NULL es lista vacia", lista_vacia(NULL));
  tester_afirmar(tester, "Lista está vacía", lista_vacia(lista));

  lista_insertar(lista, &a);
  tester_afirmar(tester, "Lista con un elemento", !lista_vacia(lista));

  lista_destruir(lista);
}

void probar_lista_elemento_en_posicion(tester_t* tester){
  lista_t* lista = lista_crear();
  if(!lista)
    return;

  int a = 0, b = 1, c = 2, d = 3;

  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);

  tester_afirmar(tester, "Leer un elemento de una posición no inicializada devuelve NULL", !lista_elemento_en_posicion(lista, lista_elementos(lista)));
  tester_afirmar(tester, "El elemento leido de la posición 0 es el esperado", *( (int*) lista_elemento_en_posicion(lista, 0) ) == a);
  tester_afirmar(tester, "El elemento leido de la segunda posición es el esperado", *( (int*) lista_elemento_en_posicion(lista, 1) ) == b);
  tester_afirmar(tester, "El elemento leido de la última posición es el esperado", *( (int*) lista_elemento_en_posicion(lista, lista_elementos(lista) - 1) ) == d);

  lista_destruir(lista);
}


/*
 *  Recibe la funcion a probar y un char* que debe contener un string que represente la funcionalidad
 * que va a probar.
 */
void probar_insertar_generico(tester_t* tester, int (*fn)(lista_t*, void*), char* funcionalidad){
  lista_t* lista = lista_crear();
  if(!lista)
    return;

  int a = 1, b = 2, c = 3;

  char descripcion[MAX_DESCRIPCION];
  memset(descripcion, '\0', MAX_DESCRIPCION);

  tester_afirmar(tester, armar_string(descripcion, 3, "No se puede ", funcionalidad, " con lista NULL"), fn(NULL, &a) == ERROR);
  tester_afirmar(tester, armar_string(descripcion, 3, "No se puede ", funcionalidad, " con puntero NULL"), fn(lista, NULL) == EXITO);
  lista_borrar(lista);
  tester_afirmar(tester, armar_string(descripcion, 3, "Se puede ", funcionalidad, " un elemento"), fn(lista, &a) == EXITO);
  tester_afirmar(tester, "El valor del primer elemento coincide con el ingresado", *( (int*) lista_elemento_en_posicion(lista, 0)) == a);
  tester_afirmar(tester, armar_string(descripcion, 3, "Se puede ", funcionalidad, " otro elemnto"), fn(lista, &b) == EXITO);
  tester_afirmar(tester, "El valor del segundo elemento coincide con el ingresado", *( (int*) lista_elemento_en_posicion(lista, 1)) == b);
  tester_afirmar(tester, armar_string(descripcion, 3, "Se puede ", funcionalidad, " otro elemnto"), fn(lista, &c) == EXITO);
  tester_afirmar(tester, "El valor del tercer elemento coincide con el ingresado", *( (int*) lista_elemento_en_posicion(lista, 2)) == c);
  tester_afirmar(tester, "La cantidad elementos en la lista es la esperada", lista_elementos(lista) == 3);

  lista_destruir(lista);
}

/*
 *  Recibe una funcion a probar, una función para chequear una posición de una lista
 * y un char* que debe ser "primero" o "ultimo" según si se debe revisar el primer o
 * último elemento. Para que la función tenga un funcionamiento correcto esto debe ir
 * acorde a la funcion fn_ver.
 */
void probar_borrar_generico(tester_t* tester, char* elemento_a_revisar, int(*fn_borrar)(lista_t*), void* (*fn_ver)(lista_t*)){
  lista_t* lista = lista_crear();
  if(!lista)
    return;

  int a = 1, b = 2, c = 3, d = 4, e = 5;
  int numero_a_revisar = strcmp(elemento_a_revisar, "último") == 0 ? d : b;
  int borra_correctamente = 0;

  char descripcion[MAX_DESCRIPCION];
  memset(descripcion, 0, MAX_DESCRIPCION);

  tester_afirmar(tester, "No se puede borrar con lista NULL", fn_borrar(NULL) == ERROR);
  tester_afirmar(tester, "No se puede borrar de una lista vacia", fn_borrar(lista) == ERROR);

  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);
  lista_insertar(lista, &e);

  tester_afirmar(tester, "Se puede borrar el último elemento de la lista", fn_borrar(lista) == EXITO);
  tester_afirmar(tester, "La cantidad de elementos después de borrar es la esperada", lista_elementos(lista) == 4);
  tester_afirmar(tester, armar_string(descripcion, 3, "El ", elemento_a_revisar, " elemento es el esperado"), *( (int*)fn_ver(lista) ) == numero_a_revisar);

  while(lista_elementos(lista) && borra_correctamente == 0)
    borra_correctamente = fn_borrar(lista);

  tester_afirmar(tester, "Se borraron todos los elementos de la lista", lista_elementos(lista) == 0);
  tester_afirmar(tester, "Todos los borrados devolvieron el valor esperado", borra_correctamente == 0);

  lista_destruir(lista);

}

void probar_lista_posicion(tester_t* tester){
  lista_t* lista = lista_crear();
  if(!lista)
    return;

  bool cumple_lo_pedido;

  int numeros[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

  for(int i = 0; i < 10; i++){
    lista_insertar(lista, numeros + i);
  }
  for(int i = 0; i < 10; i++){
    cumple_lo_pedido = *((int*)(lista_elemento_en_posicion(lista, (size_t)i))) == i ? true : false;
  }

  tester_afirmar(tester, "No se puede leer de lista NULL", lista_elemento_en_posicion(NULL, 3) == NULL);
  tester_afirmar(tester, "No se puede leer una posición que todavía no existe", lista_elemento_en_posicion(lista, 100) == NULL);
  tester_afirmar(tester, "Se leen 10 números, todos son los esperados", cumple_lo_pedido);


  lista_destruir(lista);
}

void probar_lista_insertar_en_posicion(tester_t* tester){

  lista_t* lista =  lista_crear();
  if(!lista)
    return;

  int a = 0, b = 1, c = 2, d = 3, e = 100, f = -1;

  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);

  //lista: 0 1 2 (a b c)

  tester_afirmar(tester, "No se puede insertar en lista NULL", lista_insertar_en_posicion(NULL, &a, 1) == ERROR);
  tester_afirmar(tester, "Se puede insertar elemento NULL", lista_insertar_en_posicion(lista, NULL, 1) == EXITO);
  lista_borrar_de_posicion(lista, 1);
  tester_afirmar(tester, "Elemento se insertó correctamente", lista_insertar_en_posicion(lista, &d, 2) == EXITO);
  //lista: 0 1 3 2 (a b d c)

  tester_afirmar(tester, "Elemento se insertó en la posición esperada", *( (int*) lista_elemento_en_posicion(lista, 2) ) == d);
  tester_afirmar(tester, "La cantidad de elementos es la esperada", lista_elementos(lista) == 4);
  tester_afirmar(tester, "Se puede insertar elemento en una posición superior a la cantidad", lista_insertar_en_posicion(lista, &e, 100) == EXITO);
  //lista: 0 1 3 2 100 (a b d c e)

  tester_afirmar(tester, "Luego de insertar un elemento en una posición superior a la cantidad, la cantidad de elementos es la esperada", lista_elementos(lista) == 5);
  tester_afirmar(tester, "El elemento se insertó en la última posición", *( (int*) lista_elemento_en_posicion(lista, lista_elementos(lista) - 1)) == e);
  tester_afirmar(tester, "Se puede insertar en la posición 0", lista_insertar_en_posicion(lista, &f, 0) == EXITO);
  //lista: -1 0 1 3 2 100 (f a b d c e)

  tester_afirmar(tester, "Luego de insertar en la posición 0 la cantidad de elementos es la esperada", lista_elementos(lista) == 6);
  tester_afirmar(tester, "El resto de los valores permanecen en su posición inalterados",
     ( *( (int*) lista_elemento_en_posicion(lista, 0)) == f ) &&
     ( *( (int*) lista_elemento_en_posicion(lista, 1)) == a ) &&
     ( *( (int*) lista_elemento_en_posicion(lista, 2)) == b ) &&
     ( *( (int*) lista_elemento_en_posicion(lista, 3)) == d ) &&
     ( *( (int*) lista_elemento_en_posicion(lista, 4)) == c ) &&
     ( *( (int*) lista_elemento_en_posicion(lista, 5)) == e )
  );

  lista_destruir(lista);
}

void probar_lista_borrar_de_posicion(tester_t* tester){

  lista_t* lista = lista_crear();
  if(!lista)
    return;

  int a = 0, b = 1, c = 2, d = 3, e = 4;

  lista_insertar(lista, &a);
  lista_insertar(lista, &b);
  lista_insertar(lista, &c);
  lista_insertar(lista, &d);
  lista_insertar(lista, &e);

  //lista: 0 1 2 3 4

  tester_afirmar(tester, "No se puede borrar de lista NULL", lista_borrar_de_posicion(NULL, 2) == ERROR);
  tester_afirmar(tester, "Elemento se borrar correctamente", lista_borrar_de_posicion(lista, 2) == EXITO);
  //lista: 0 1 3 4 (a b d e)

  tester_afirmar(tester, "La cantidad de elementos es la esperada", lista_elementos(lista) == 4);
  tester_afirmar(tester, "El elemento borrado no se encuentra en la lista", *( (int*) lista_elemento_en_posicion(lista, 2) ) != c);
  tester_afirmar(tester, "Se puede borrar un elemento de una posicion que no existe", lista_borrar_de_posicion(lista, 100) == EXITO);
  //Lista: 0 1 3 (a b d)

  tester_afirmar(tester, "Se puede borrar el elemento en la posición 0", lista_borrar_de_posicion(lista, 0) == EXITO);
  //lista: 1 3 (b d)

  tester_afirmar(tester, "Hay un nuevo elemento en la posición 0", *( (int*) lista_elemento_en_posicion(lista, 0) ) != a);
  tester_afirmar(tester, "Se borró el último elemento", *( (int*) lista_elemento_en_posicion(lista, lista_elementos(lista) - 1) ) == d);
  tester_afirmar(tester, "Luego de borrar la última posición, la cantidad de elementos es la esperada", lista_elementos(lista) == 2);
  tester_afirmar(tester, "El resto de valores permanecen en su posición inalterados",
    ( *( (int*) lista_elemento_en_posicion(lista, 0)) == b ) &&
    ( *( (int*) lista_elemento_en_posicion(lista, 1)) == d )
  );

  lista_destruir(lista);
}

void probar_iterador_en_lista(tester_t* tester){
  lista_t* lista = lista_crear();
  if(!lista)
    return;

  int enteros [5] = {0, 1, 2, 3, 4};

  lista_insertar(lista, enteros);
  lista_insertar(lista, enteros + 1);
  lista_insertar(lista, enteros + 2);
  lista_insertar(lista, enteros + 3);
  lista_insertar(lista, enteros + 4);

  lista_iterador_t* iterador = lista_iterador_crear(lista);
  int i = 0;
  int numero_actual;

  while(lista_iterador_tiene_siguiente(iterador)){
    numero_actual = *( (int*) lista_iterador_siguiente(iterador) );
    tester_afirmar(tester, "Elemento leido coincide con lo esperado", numero_actual == enteros[i]);
    i++;
  }

  lista_iterador_destruir(iterador);

  int* contador = 0;
  lista_con_cada_elemento(lista, duplicar_valor, &contador);

  lista_destruir(lista);
}

void prueba_de_volumen(tester_t* tester){
  lista_t* lista = lista_crear();

  if(!lista)
    return;

  int se_puede_insertar;
  int se_puede_borrar;
  int* numero_a_cargar = 0;
  int** direccion_numero_a_cargar = &numero_a_cargar;
  int i = 0;
  int j = 0;

  printf("Se cargan 7k elementos\n");

  do{
      se_puede_insertar = lista_insertar(lista, direccion_numero_a_cargar);
      numero_a_cargar ++;
      i++;
  } while(se_puede_insertar == EXITO && i < 7000);

  tester_afirmar(tester, "Se pudieron insertar 7000 elementos", se_puede_insertar == EXITO);
  tester_afirmar(tester, "La cantidad de elementos es la esperada", lista_elementos(lista) == 7000);

  printf("Se borran 1k elementos\n");

  do{
    se_puede_borrar = lista_borrar(lista);
    j++;
  } while(se_puede_borrar == EXITO && j < 1000);

  j = 0;
  printf("Se borran 2k elementos\n");

  do{
    se_puede_borrar = lista_borrar(lista);
    j++;
  } while(se_puede_borrar == EXITO && j < 2000);

  j = 0;

  printf("Se borran 4k elementos\n");
  do{
    se_puede_borrar = lista_borrar(lista);
    j++;
  } while(se_puede_borrar == EXITO && j < 4000);

  tester_afirmar(tester, "Se pudieron borrar 3 elementos", se_puede_borrar == EXITO);
  tester_afirmar(tester, "La cantidad de elementos es la esperada", lista_vacia(lista));

  lista_destruir(lista);
}

int main() {
  tester_t* tester = tester_crear();

  if(!tester)
    return ERROR;

  int (*insertar)(lista_t*, void*) = lista_insertar;
  int (*apilar)(lista_t*, void*) = lista_apilar;
  int (*encolar)(lista_t*, void*) = lista_encolar;
  int (*borrar)(lista_t*) = lista_borrar;
  int (*desapilar)(lista_t*) = lista_desapilar;
  int (*desencolar)(lista_t*) = lista_desencolar;
  void* (*ver_ultimo)(lista_t*) = lista_ultimo;
  void* (*ver_ultimo_pila)(lista_t*) = lista_tope;
  void* (*ver_primero)(lista_t*) = lista_primero;


  printf(FORMATO_PRUEBAS_GENERALES);

  printf(LINEA_DIVISORIA);
  printf("PRUEBAS A LISTA CREAR\n");
  probar_crear(tester);

  printf(LINEA_DIVISORIA);
  printf("Pruebas lista vacia\n");
  probar_lista_vacia(tester);

  printf(LINEA_DIVISORIA);



  printf("\n\n");
  printf(FORMATO_PRUEBAS_LISTA);
  printf("\n");

  printf(LINEA_DIVISORIA);
  printf("Pruebas insertar\n");
  probar_insertar_generico(tester, insertar, "insertar");

  printf(LINEA_DIVISORIA);
  printf("Pruebas borrar\n");
  probar_borrar_generico(tester, "último", borrar, ver_ultimo);

  printf(LINEA_DIVISORIA);
  printf("Pruebas elemento en posición\n");
  probar_lista_posicion(tester);

  printf(LINEA_DIVISORIA);
  printf("Pruebas insertar en posición\n");
  probar_lista_insertar_en_posicion(tester);

  printf(LINEA_DIVISORIA);
  printf("Pruebas borrar de posición\n");
  probar_lista_borrar_de_posicion(tester);


  probar_lista_elemento_en_posicion(tester);
  prueba_de_volumen(tester);
  printf(LINEA_DIVISORIA);



  printf("\n\n");
  printf(FORMATO_PRUEBAS_PILA);
  printf("\n");

  printf(LINEA_DIVISORIA);
  printf("Pruebas apilar");
  probar_insertar_generico(tester, apilar, "apilar");

  printf(LINEA_DIVISORIA);
  printf("Pruebas desapilar\n");
  probar_borrar_generico(tester, "último", desapilar, ver_ultimo_pila);

  printf(LINEA_DIVISORIA);



  printf("\n\n");
  printf(FORMATO_PRUEBAS_COLA);
  printf("\n");

  printf(LINEA_DIVISORIA);
  printf("Pruebas encolar");
  probar_insertar_generico(tester, encolar, "encolar");

  printf(LINEA_DIVISORIA);
  printf("Pruebas desencolar\n");
  probar_borrar_generico(tester, "primero", desencolar, ver_primero);

  printf(LINEA_DIVISORIA);

  printf("\n\n");
  printf(FORMATO_PRUEBAS_ITERADOR);
  printf("\n");
  probar_iterador_en_lista(tester);
  printf(LINEA_DIVISORIA);
  printf("\n\n");



  tester_mostrar_estadisticas(tester);
  tester_destruir(tester);

  return 0;
}
