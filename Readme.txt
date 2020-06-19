
******************************     QUÉ ES LO ENTREGADO     *******************************

    Lo entregado es en sentido general un tipo de dato abstracto (TDA), particularmente es
una estructura de datos, en este caso, una lista. Adicionalmente cuenta con las funciones
para comportarse como una cola o pila según sea requerido por el usuario.
    Esta implementación particular fue realizada con nodos simplemente enlazados. La mayoría
de las funciones de cola y pila fueron implementadas reutilizando las herramientas desarrolladas
para el comportamiento de lista, buscando tener un código sin repeticiones y asimismo consistencia
teórica con el hecho de que cola y una pila son casos particulares de lista, por lo tanto se pueden
pensar como listas con reglas diferentes en cuanto a cómo acceder y como se almacena la información.



**************************  LINEAS DE COMPILACIÓN Y EJECUCIÓN  ***************************

    Para ejecutar este programa primero se debe compilar el código fuente. Para lograr esto
debemos abrir una terminal y navegar hasta la carpeta donde se encuentre el código fuente junto con
los archivos lista.h, lista.c, lista_se.c, ansicolor.h, tester.h y tester.c. Una vez allí, ejecutar el
comando

        $ make lista_se

    De este modo se generará un archivo binario de nombre "lista_se". Para ejecutarlo, basta con escribir
el comando

        ./lista_se


*********************************       TEORÍA       **********************************

    ¿Qué es una lista? Diferencias entre simple y doblemente enlazada.

    Una lista es una estructura de datos en la que la información que guardamos puede
ser insertada en cualquier posición arbitraria que el usuario requiera y puede ser accedida
con la misma libertad. Esta, en una implementación con nodos, puede ser simplemente enlazada
o doblemente enlazada. Las principales diferencias son que la implementación de una lista simplemente
enlazada es más sencilla ya que hay menos punteros en juego en cada operación, sin embargo esto
tiene la desventaja de que la lista puede ser recorrida únicamente en un sentido, lo que hace que
ciertas operaciones tengan necesariamente una complejidad algorítmica superior. Esto tiene como
consecuencia que, considerando el tiempo de ejecución, una lista doblemente enlazada sea más óptima.
Por otro lado, si nos centramos en la memoria, una lista doblemente enlazada cuenta con nodos de 3 punteros
en lugar de 2 como la simplemente enlazada, por lo tanto consume un 50% más de memoria por nodo.

    ¿Cuáles son las características fundamentales de las pilas y de las colas?
    

    Las características fundamentales de las pilas y de las colas son las que nos dicen cómo almacenar
información en ellas y cómo podemos acceder a esta.
    En el caso de las pilas, sólo podemos acceder al último dato ingresado. Una vez que sacamos este,
podemos acceder al que fue ingresado antes y así hasta llegar  al primer dato ingresado que es el último
que podemos obtener. A esto se lo denomina LIFO (last in first out), que en español es: último en entrar
primero en salir.
    En el caso de las colas, siempre podemos acceder al dato que fue ingresado con mayor anterioridad,
es decir al primero que fue ingresado de los que quedan en la cola. Por lo tanto, al último  dato que
podremos acceder es al que fue ingresado por último.  A esto se lo denomina FIFO. First in, first out.


    ¿Qué es un iterador? ¿Cuál es su función? ¿En que se diferencia uno interno de uno externo?

    Un iterador es un TDA que contiene como dato un puntero a una cierta estructura de datos y se utiliza
para iterar sobre ella. Su función en el caso de lista es reducir el tiempo de ejecución de iterar sobre
ella, ya que al conocer su construcción interna puede acceder a información que el usuario no tiene.
    Contamos con dos tipos de iteradores, el externo y el interno. En el caso del externo, el que se debe
encargar de su manejo es el usuario de la lista, el iterador simplemente es una herramienta para reducir
la complejidad algorítmica de la iteración. Esto se debe a que al conocer la estructura de la lista, puede
recorrerla con una complejidad O(n), sin esta herramienta el usuario debería recorrer la lista con una complejidad
O(n²), ya que en la implementación de una lista simplemente enlazada la operación que tendría que utilizar el usuario
sin un iterador implicaría recorrer internamente la lista desde el elemento 0 al deseado en cada iteración. Esto
con el iterador no sucede ya que este apunta al último elemento accedido y puede ir de este al siguiente sin
necesidad de volver a empezar por el elemento 0.
    En el caso del iterador interno, el usuario no conoce su funcionamiento, simplemente recorre toda la lista
realizando una operación determinada por el usuario con cada elemento.
