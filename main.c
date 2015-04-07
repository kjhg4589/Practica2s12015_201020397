#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANO 81
#define CARGAR 1
#define IMPRIMIR 2
#define ORDENAR 3
#define REPORTE 4
#define DIAGRAMA 5
//
//NODO DE LA LISTA DOBLEMENTE ENLAZADA
typedef struct nodo{
    int dato;
    struct nodo *sig;
}Nodo;
//NODO DEL ARBL AVL
typedef struct arbol{
    int dato;
    int fe;//FACTOR DE EQUILIBRIO
    int cantidad;//NUMERO DE DATOS REPETIDOS
    struct arbol *izq;
    struct arbol *der;
}Arbol;
//SE INICIALIZAN LOS PUNTEROS
Arbol *raiz;
Nodo *cabeza;
Nodo *cabeza1;
//METODO QUE INSERTA EN LA LISTA Y RETORNA LA CABEZA
Nodo *Lista(Nodo *cabeza, int dato){
    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->dato = dato;
    nuevo->sig = NULL;
    if(nuevo==NULL){
        printf("No hay memoria suficiente \n");
    }

    if(cabeza==NULL){
        cabeza = nuevo;
    }else{
        Nodo *aux = cabeza;
        while(aux->sig!=NULL){
            aux = aux->sig;
        }
        aux->sig = nuevo;
    }
    return cabeza;
}
//METODO QUE LEE EL ARCHIVO .TXT EN INSERTA EN LA LISTA
Nodo *leerArchivo(Nodo *cabeza){
    printf("Coloque la ruta del archivo que contiene los datos\n");
    char archivo[TAMANO];
    scanf("%s", archivo);
    FILE* flujo = fopen(archivo, "rb");

    if(flujo == NULL){
        perror("Error no se pudo leer el archivo\n");
        return 1;
    }
    int dato;
    while(feof(flujo)==0){
        fscanf(flujo, "%d", &dato);
        cabeza = Lista(cabeza, dato);
    }

    fclose(flujo);
    printf("El archivo fue leido exitosamente \n");
    return cabeza;
}
//METODO QUE IMPRIME LA LISTA
void Imprimir(Nodo *cabeza){
    printf("los datos son los siguientes: \n");
    Nodo *aux = cabeza;
    while(aux!=NULL){
        printf("%d, ", aux->dato);
        aux = aux->sig;
    }
    printf("\n");
}
//ORDENAR LISTA CON BURBUJA
Nodo *ordenar(Nodo *cabeza){
    Nodo *actual, *siguiente;
    int n;
    actual = cabeza;
    while(actual->sig != NULL){
        siguiente = actual->sig;
        while(siguiente!=NULL){
            if(actual->dato > siguiente->dato){
                n = siguiente->dato;
                siguiente->dato = actual->dato;
                actual->dato = n;
            }
            siguiente = siguiente->sig;
        }
        actual= actual->sig;
        siguiente = actual->sig;
    }
    printf("La lista fue ordenada exitosamente\n");
    return cabeza;
}
//MAXIMO DE DOS NUMEROS
int MAX(int a, int b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}
//OBTENER FACTOR DE EQUILIBRIO
int obtenerFE(Arbol *x){
    if(x==NULL){
        return -1;
    }else{
        return x->fe;
    }
}
//ROTACIOIN SIMPLE A LA IZQUIERDA
Arbol *rotacionI(Arbol *raiz){
    Arbol *aux = raiz->izq;
    raiz->izq = aux->der;
    aux->der = raiz;
    raiz->fe = MAX(obtenerFE(raiz->izq), obtenerFE(raiz->der))+1;
    aux->fe = MAX(obtenerFE(aux->izq), obtenerFE(aux->der))+1;
    return aux;
}
//ROTACION SIMPLE DERECHA
Arbol *rotacionD(Arbol *raiz){
    Arbol *aux = raiz->der;
    raiz->der = aux->izq;
    aux->izq = raiz;
    raiz->fe = MAX(obtenerFE(raiz->izq), obtenerFE(raiz->der))+1;
    aux->fe = MAX(obtenerFE(aux->izq), obtenerFE(aux->der))+1;
    return aux;
}
//ROTACION DOBLE IZQUIERDA
Arbol *rotacionDI(Arbol *raiz){
    Arbol * aux;
    raiz->izq = rotacionD(raiz->izq);
    aux = rotacionI(raiz);
    return aux;
}
//ROTACION DOBLE DERECHA
Arbol *rotacionDD(Arbol *raiz){
    Arbol *aux;
    raiz->der = rotacionI(raiz->der);
    aux = rotacionD(raiz);
    return aux;
}
//METODO PARA INSERTAR AVL
Arbol *insertarAVL(Arbol *nuevo, Arbol *subr){
    Arbol *nuevoPadre = subr;
    if(nuevo->dato<subr->dato){
        if(subr->izq==NULL){
            subr->izq = nuevo;
        }else{
            subr->izq = insertarAVL(nuevo, subr->izq);
            if((obtenerFE(subr->izq)-obtenerFE(subr->der))==2){
                if(nuevo->dato<subr->izq->dato){
                    nuevoPadre = rotacionI(subr);
                }else{
                    nuevoPadre = rotacionDI(subr);
                }
            }
        }
    }else if(nuevo->dato>subr->dato){
        if(subr->der==NULL){
            subr->der = nuevo;
        }else{
            subr->der = insertarAVL(nuevo, subr->der);
            if((obtenerFE(subr->der)-obtenerFE(subr->izq))==2){
                if(nuevo->dato>subr->der->dato){
                    nuevoPadre = rotacionD(subr);
                }else{
                    nuevoPadre = rotacionDD(subr);
                }
            }
        }
    }else{
        subr->cantidad = subr->cantidad+1;
    }
    if((subr->izq == NULL) && (subr->der!=NULL)){
        subr->fe = subr->der->fe+1;
    }else if((subr->der==NULL)&&(subr->izq != NULL)){
        subr->fe = subr->izq->fe+1;
    }else{
        subr->fe = MAX(obtenerFE(subr->izq), obtenerFE(subr->der))+1;
    }
    return nuevoPadre;
}
//INSERTAR ARBOL
Arbol *insertar(Arbol *raiz, int dato){
    Arbol *nuevo = (Arbol *)malloc(sizeof(Arbol));
    nuevo->dato = dato;
    nuevo->cantidad = 1;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    if(raiz==NULL){
        raiz = nuevo;
    }else{
        raiz = insertarAVL(nuevo, raiz);
    }
    return raiz;
}
//INSERTAR EN LISTA1
Nodo *insertarLista(Nodo *cabeza, Nodo *cabeza1){
    Nodo *aux = cabeza;
    while(aux!=NULL){
        cabeza1=Lista(cabeza1, aux->dato);
        aux = aux->sig;
    }
    return cabeza1;
}
//RECORRIDO INORDEN
void inOrden(Arbol *raiz){
    if(raiz!=NULL){
            inOrden(raiz->izq);
            int n=0;
            for(n=0; n<raiz->cantidad; n++){
                printf("%d, ", raiz->dato);
            }
            inOrden(raiz->der);
    }
}
//INSERTA EN ARBOL RECORRIENDO LA LISTA
Arbol *insertaArbol(Nodo *cabeza, Arbol *raiz){
    Nodo *aux = cabeza;
    while(aux!=NULL){
        raiz = insertar(raiz, aux->dato);
        aux = aux->sig;
    }
    return raiz;
}
//METODO QUE DEVUELVE NODO POR INDICE EN LA LISTA
Nodo *getNodo(Nodo *lista, int i){
    Nodo *aux = lista;
    int j = 1;
    while(j<i && aux!=NULL){
        aux = aux->sig;
        j++;
    }
    return aux;
}
//CANTIDAD DE NODOS DENTRO DE LA LISTA
int cantidad(Nodo *lista){
    int i;
    Nodo *aux = lista;
    while(aux!=NULL){
        aux = aux->sig;
        i++;
    }
    return i;
}
//METODO QUICKSORT
void quicksirt(Nodo *lista, int cantidad){
    qs(lista, 1, cantidad);
}
//METODO RECURSIVO PARA QUIQKSORT
void qs(Nodo *lista, int lim_izq, int lim_der){
    int izq, der, temporal, pivote;
    izq = lim_izq;
    der = lim_der;
    pivote = getNodo(lista, (lim_izq+lim_der)/2)->dato;
    do{
        while((getNodo(lista,izq)->dato<pivote) && (izq<lim_der)){
            izq++;
        }
        while((pivote<getNodo(lista,der)->dato) && (der>lim_izq)){
            der--;
        }
        if(izq<=der){
            temporal = getNodo(lista,izq)->dato;
            getNodo(lista, izq)->dato = getNodo(lista,der)->dato;
            getNodo(lista, der)->dato = temporal;
            izq++;
            der--;
        }
    }while(izq<=der);
    if(lim_izq<der){qs(lista, lim_izq, der);}
    if(lim_der>izq){qs(lista, izq, lim_der);}
}
//GRAFICAR
void graficar(float t_inser, float t_inorden, float t_burbuja, float t_quicksort, int datos){
    freopen("salida.gp", "w", stdout);
    printf("set multiplot\n");
    printf("set size .5,.5\n");
    printf("set origin 0,.5\n");
    printf("set title \"INSERCION EN EL ARBOL\"\n");
    printf("set xlabel \"Datos\"\n");
    printf("set ylabel \"Tiempo\"\n");
    printf("plot %f*x/%d, log(x); \n ", t_inser, datos);
    printf("set origin .5,.5\n");
    printf("set title \"RECORRIDO EN EL ARBOL\"\n");
    printf("set xlabel \"Datos\"\n");
    printf("set ylabel \"Tiempo\"\n");
    printf("plot %f*x/%d, log(x); \n ", t_inorden, datos);
    printf("set origin 0,0\n");
    printf("set title \"TIEMPO EN BURBUJA\"\n");
    printf("set xlabel \"Datos\"\n");
    printf("set ylabel \"Tiempo\"\n");
    printf("plot %f*x/%d, x; \n", t_burbuja, datos);
    printf("set origin .5,0\n");
    printf("set title \"TIEMPO EN QUICKSORT\"\n");
    printf("set xlabel \"Datos\"\n");
    printf("set ylabel \"Tiempo\"\n");
    printf("plot %f*x/%d, log(x); \n", t_quicksort, datos);
    printf("set multiplot\n pause mouse \n");
    fclose(stdout);
    system("gnuplot salida.gp");
}
//METODO CONSTRUCTOR
int main()
{
    cabeza = NULL;
    cabeza1 = NULL;
    raiz = NULL;
    int seleccion;
    clock_t t1,t2;
    float t_ordenarBurbuja, t_ordenarQuicksort, t_insercion, t_recorido;
    //MENU DE LA APLICACION
    do{
        system("clear");
        if(seleccion==CARGAR){//CARGA LOS ARCHIVOS
            cabeza = leerArchivo(cabeza);
            t1=clock();
            raiz = insertaArbol(cabeza, raiz);
            t2=clock();
            t_insercion = (float)(t2-t1)/(float)(CLOCKS_PER_SEC);
            cabeza1 = insertarLista(cabeza, cabeza1);
            printf("El tiempo que tardo la inseccion = %f\n", (float)(t2-t1)/(float)(CLOCKS_PER_SEC));
        }
        if(seleccion==IMPRIMIR){//IMPRIME AMBOS DATOS
            printf("Quicksort\n");
            Imprimir(cabeza1);
            printf("Burbuja\n");
            Imprimir(cabeza);
            printf("Los datos del arbol son: \n");
            t1=clock();
            inOrden(raiz);
            t2=clock();
            t_recorido = (float)(t2-t1)/(float)(CLOCKS_PER_SEC);
            printf("\nEl tiempo que tardo en realizar el recorrido = %f\n", (float)(t2-t1)/(float)(CLOCKS_PER_SEC));
        }
        if(seleccion==ORDENAR){
            t1=clock();
            quicksirt(cabeza1, cantidad(cabeza1));
            t2=clock();
            t_ordenarBurbuja = (float)(t2-t1)/(float)(CLOCKS_PER_SEC);
            t1=clock();
            cabeza = ordenar(cabeza);
            t2=clock();
            t_ordenarQuicksort = (float)(t2-t1)/(float)(CLOCKS_PER_SEC);
        }
        if(seleccion==REPORTE){
            printf("REPORTE DE TIEMPOS \n\n");
            printf("Ingresar al arbol: %f\n", t_insercion);
            printf("Recorrido del arbol: %f\n", t_recorido);
            printf("Ordenado burbuja: %f\n", t_ordenarBurbuja);
            printf("Ordenado quicksort: %f\n", t_ordenarQuicksort);
        }
        if(seleccion==DIAGRAMA){
            graficar(t_insercion, t_recorido, t_ordenarBurbuja, t_ordenarQuicksort, cantidad(cabeza));
        }
        if(seleccion == 9){
            int n;
            scanf("%d", &n);
            printf("dato = %d\n", getNodo(cabeza1, n)->dato);
        }
        printf("\n------------------------- MENU ---------------------------\n");
        printf("0. Salir \n");
        printf("1. Leer Archivo \n");
        printf("2. Imprimir \n");
        printf("3. Ordenar Listas \n");
        printf("4. Reporte \n");
        printf("5. Diagrama \n");
        printf("6. Limpiar Consola \n");
        printf("-------------- Selecione una de las opciones ---------------\n");
        scanf("%d", &seleccion);
    }while(seleccion!=0);
    system("clear");
    printf("LA APLICACION TERMINO SU PROCESO \n");
    return 0;
}

