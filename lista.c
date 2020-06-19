#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define EXITO 0
#define ERROR -1

typedef struct nodo {
  void* elemento;
  struct  nodo* siguiente;
} nodo_t;

typedef struct {
    size_t cantidad_elementos;
    nodo_t* primero;
    nodo_t* ultimo;
} lista_t;

typedef struct lista_iterador {
  lista_t* lista;
  nodo_t* nodo_actual;
  int indice;
} lista_iterador_t;

/*--------------------- Funciones privadas ----------------------------*/

static nodo_t* lista_nodo_n(lista_t* lista, size_t posicion){
  size_t i = 0;
  if(!lista)
    return NULL;

  nodo_t* nodo_a_ret = lista -> primero;

  for(i = 0; i < posicion; i++){
    nodo_a_ret = nodo_a_ret -> siguiente;
  }

  return nodo_a_ret;
}


/*----------- Funciones comunes a lista, pila y cola ------------------*/

lista_t* lista_crear(){

  lista_t* lista = calloc(1, sizeof(lista_t));

  return lista;
}

void lista_destruir(lista_t* lista){
  if(!lista)
    return;

  nodo_t* aux;
  while(lista -> cantidad_elementos){
      aux = lista -> primero;
      lista -> primero = lista -> primero -> siguiente;
      free(aux);
      lista -> cantidad_elementos--;
  }

  free(lista);
}

bool lista_vacia(lista_t* lista){
  if(!lista)
    return true;

  return lista -> cantidad_elementos == 0;
}

size_t lista_elementos(lista_t* lista){
  size_t cantidad_elementos = !lista ? 0 : lista -> cantidad_elementos;

  return cantidad_elementos;
}


/*--------------------- Funciones de lista ----------------------------*/

void* lista_elemento_en_posicion(lista_t* lista, size_t posicion){
  if(!lista || posicion >= lista -> cantidad_elementos)
    return NULL;

  nodo_t* ret = lista_nodo_n(lista, posicion);

  return ret -> elemento;
}

void* lista_ultimo(lista_t* lista){
  return lista_vacia(lista) ? NULL : lista -> ultimo -> elemento;
}

int lista_insertar(lista_t* lista, void* elemento){
  if(!lista)
    return ERROR;

  if(lista_vacia(lista)){
    lista -> primero = calloc(1, sizeof(nodo_t));
    if(!lista -> primero)
      return ERROR;

    lista -> primero -> elemento = elemento;
    lista -> ultimo = lista -> primero;
  }

  else{
    lista -> ultimo -> siguiente = calloc(1, sizeof(nodo_t));
    if(!lista -> ultimo -> siguiente)
      return ERROR;

    lista -> ultimo = lista -> ultimo -> siguiente;
    lista -> ultimo -> elemento = elemento;
  }
  lista -> cantidad_elementos++;
  return EXITO;
}

int lista_insertar_en_posicion(lista_t* lista, void* elemento, size_t posicion){
  if(!lista)
    return ERROR;

  if(posicion >= lista -> cantidad_elementos)
    return lista_insertar(lista, elemento);

  nodo_t* nodo_a_agregar = calloc(1, sizeof(nodo_t));
  nodo_a_agregar -> elemento = elemento;

  if(posicion != 0){
    nodo_t* nodo_precesor = lista_nodo_n(lista, posicion - 1);

    nodo_a_agregar -> siguiente = nodo_precesor -> siguiente;
    nodo_precesor -> siguiente = nodo_a_agregar;
  }

  else{
    nodo_a_agregar -> siguiente = lista -> primero;
    lista -> primero = nodo_a_agregar;
  }

  lista -> cantidad_elementos++;
  return EXITO;
}

int lista_borrar(lista_t* lista){
  if(!lista || lista_vacia(lista))
    return ERROR;


  if(lista -> cantidad_elementos == 1){
    free(lista -> primero);
    lista -> primero = NULL;
  }

  else {
    nodo_t* anteultimo_nodo = lista_nodo_n(lista, lista -> cantidad_elementos - 2);
    free(lista -> ultimo);
    lista -> ultimo = anteultimo_nodo;
    lista -> ultimo -> siguiente = NULL;
  }

  (lista -> cantidad_elementos)--;
  return EXITO;
}

int lista_borrar_de_posicion(lista_t* lista, size_t posicion){
  if(!lista)
    return ERROR;

  if(posicion >= lista -> cantidad_elementos)
    return lista_borrar(lista);

  nodo_t* nodo_a_eliminar;

  if(posicion == 0){
    nodo_a_eliminar = lista -> primero;
    lista -> primero = lista -> primero -> siguiente;
  }
  else{
    nodo_t* nodo_precesor = lista_nodo_n(lista, posicion - 1);
    nodo_a_eliminar = nodo_precesor -> siguiente;
    nodo_precesor -> siguiente = nodo_precesor -> siguiente -> siguiente;
  }

  free(nodo_a_eliminar);
  lista -> cantidad_elementos--;
  return EXITO;
}


/*-------------------- Funciones de pila -----------------------------*/

int lista_apilar(lista_t* lista, void* elemento){
  return lista_insertar(lista, elemento);
}

int lista_desapilar(lista_t* lista){
  return lista_borrar(lista);
}

void* lista_tope(lista_t* lista){
  return lista_ultimo(lista);
}


/*-------------------- Funciones de cola -----------------------------*/

int lista_encolar(lista_t* lista, void* elemento){
  return lista_insertar(lista, elemento);
}

int lista_desencolar(lista_t* lista){
  return lista_borrar_de_posicion(lista, 0);
}

void* lista_primero(lista_t* lista){
  return lista_vacia(lista) ? NULL : lista -> primero -> elemento;
}


/*------------------ Funciones de iteradores ---------------------------*/

lista_iterador_t* lista_iterador_crear(lista_t* lista){
  if(!lista)
    return NULL;

  lista_iterador_t* lista_iterador = calloc(1, sizeof(lista_iterador_t));

  if(!lista_iterador)
    return NULL;

  lista_iterador -> lista = lista;
  lista_iterador -> nodo_actual = lista -> primero;

  return lista_iterador;
}

bool lista_iterador_tiene_siguiente(lista_iterador_t* lista_iterador){
  if(!lista_iterador)
    return false;

  bool tiene_siguiente = (lista_iterador -> lista -> cantidad_elementos) > (lista_iterador -> indice) ? true : false;

  return tiene_siguiente;
}

void* lista_iterador_siguiente(lista_iterador_t* lista_iterador){
  if(!lista_iterador)
    return NULL;

  if(!lista_iterador_tiene_siguiente(lista_iterador))
    return NULL;

  void* elemento_ret = lista_iterador -> nodo_actual -> elemento;
  lista_iterador -> nodo_actual = lista_iterador -> nodo_actual -> siguiente;
  lista_iterador -> indice++;

  return elemento_ret;
}

void lista_iterador_destruir(lista_iterador_t* lista_iterador){
  free(lista_iterador);
}


void lista_con_cada_elemento(lista_t* lista, void (*funcion)(void*, void*), void *contexto){
  nodo_t* nodo_actual = lista -> primero;
  for(size_t i = 0; i < lista -> cantidad_elementos; i++){
    funcion(nodo_actual -> elemento, contexto);
    nodo_actual = nodo_actual -> siguiente;
  }
}
