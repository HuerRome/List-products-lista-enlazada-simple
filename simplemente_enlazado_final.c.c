#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para el registro de cada nodo
typedef struct {
    int codigo;
    char descripcion[50];
    int cantidad;
    float precioCosto;
} Registro;

// Estructura para el nodo de la lista enlazada
typedef struct Nodo {
    Registro datos;
    struct Nodo* siguiente;
} Nodo;

//Inicializar lista
typedef struct ListaEnlazada {
    struct Nodo* head;
} ListaEnlazada;

// Función para crear un nuevo nodo
Nodo* CrearNodo(Registro datos) {
    Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));

    if (nuevoNodo == NULL) {
        printf("Error: No se pudo asignar memoria para el nuevo nodo.\n");
        exit(1);
    }
    nuevoNodo->datos = datos;
    nuevoNodo->siguiente = NULL;

    return nuevoNodo;
}

// Función para insertar un nuevo nodo al final de la lista
void InsertarNodo(ListaEnlazada* lista, Registro datos) {
    Nodo* nuevoNodo = CrearNodo(datos);

    if (lista->head == NULL) {
        lista->head = nuevoNodo;
        return;
    }

    // Si la lista no está vacía, el nuevo nodo se colocará al final después de iterar la lista
    Nodo* actual = lista->head;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevoNodo; // Agregamos el nuevo nodo al final de la lista
}

// Función para eliminar un nodo con un código de artículo dado
void EliminarNodo(ListaEnlazada* lista, int valor) {
    Nodo* actual = lista->head;
    Nodo* anterior = NULL;

    // Buscamos el nodo a eliminar
    while (actual != NULL && actual->datos.codigo != valor) {
        anterior = actual;
        actual = actual->siguiente;
    }

    if (actual == NULL) {
        printf("El codigo del articulo %d no se encuentra en la lista.\n", valor);
        return;
    }

    // Si el nodo a eliminar es el primer nodo
    if (anterior == NULL) {
        lista->head = actual->siguiente;
    } else {
        anterior->siguiente = actual->siguiente;
    }
   
    free(actual);
}

// Función para imprimir toda la lista
void ImprimirLista(ListaEnlazada lista) {
    Nodo* actual = lista.head;

    while (actual != NULL) {
        printf("\n%d ", actual->datos.codigo);
        printf("Descripcion: %s, ", actual->datos.descripcion);
        printf("Cantidad: %d, ", actual->datos.cantidad);
        printf("Precio: %.2f, \n", actual->datos.precioCosto);

        actual = actual->siguiente;
    }
    printf("\n");
}

// Función para buscar un artículo por su código
//Metodo de busqueda: Busqueda lineal o secuencial
void BuscarNodo(ListaEnlazada lista, int valor) {
    FILE* archivo;
    archivo = fopen("stock.dat", "rb");
    if (archivo != NULL) {
        Registro reg;
        while (fread(&reg, sizeof(Registro), 1, archivo) == 1) {
            if (reg.codigo == valor) {
                printf("\nArtículo encontrado:\n");
                printf("\nCodigo: %d\n", reg.codigo);
                printf("Descripcion: %s\n", reg.descripcion);
                printf("Cantidad: %d\n", reg.cantidad);
                printf("Precio: %.2f\n", reg.precioCosto);
                fclose(archivo);
                return;
            }
        }
        fclose(archivo);
        printf("Artículo no encontrado\n");
    } else {
        printf("Error al abrir el archivo para buscar los datos\n");
    }
}

// Función para liberar la memoria asignada a la lista
void LiberarLista(ListaEnlazada* lista) {
    Nodo* actual = lista->head;
    Nodo* siguiente;

    while (actual != NULL) {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    lista->head = NULL;
}

// Función para intercambiar los valores de dos nodos
void Intercambiar(Nodo* a, Nodo* b) {
    Registro temporal = a->datos;
    a->datos = b->datos;
    b->datos = temporal;
}

// Función para ordenar los nodos por el código de artículo
//Metodos de ordenamiento: Algoritmo de seleccion
void OrdenarNodos(ListaEnlazada* lista) {
    Nodo *i, *j, *min;

    for (i = lista->head; i != NULL; i = i->siguiente) {
        min = i;
        for (j = i->siguiente; j != NULL; j = j->siguiente) {
            if (j->datos.codigo < min->datos.codigo) {
                min = j;
            }
        }
        Intercambiar(i, min);
    }
}

// Función para crear el archivo
void CrearArchivo() {
    FILE* archivo;
    archivo = fopen("stock.dat", "wb");

    if (archivo == NULL) {
        printf("Error al crear el archivo.\n");
        exit(1);
    } else {
        printf("Se creo el archivo exitosamente.\n");
        fclose(archivo);
    }
}

// Función para guardar la lista en el archivo binario
void GuardarListaEnArchivo(ListaEnlazada lista) {
    FILE* archivo;
    archivo = fopen("stock.dat", "wb");

    if (archivo != NULL) {
        Nodo* actual = lista.head;
        while (actual != NULL) {
            fwrite(&(actual->datos), sizeof(Registro), 1, archivo);
            actual = actual->siguiente;
        }
        fclose(archivo);
    } else {
        printf("Error al abrir el archivo para guardar los datos\n");
    }
}

// Función para cargar la lista desde el archivo binario
void CargarListaDesdeArchivo(ListaEnlazada* lista) {
    FILE* archivo = fopen("stock.dat", "rb");
    if (archivo != NULL) {
        Registro reg;
        while (fread(&reg, sizeof(Registro), 1, archivo) == 1) {
            InsertarNodo(lista, reg);
        }
        fclose(archivo);
    } else {
        printf("Error al abrir el archivo para cargar los datos\n");
    }
}

int main() {
    // Creamos una lista enlazada vacía
    ListaEnlazada myList;
    myList.head = NULL;
    Registro reg;
    int opcion;

    // Cargar datos del archivo binario al iniciar el programa
    CargarListaDesdeArchivo(&myList);

    do {
        printf("---------------------------------------------------------------\n");
        printf("Menu:\n");
        printf("1) Crear un archivo binario llamado productos.dat\n");
        printf("2) Altas(Insertar Nodos)\n");
        printf("3) Baja(Eliminar nodos)\n");
        printf("4) Visualizar toda la lista\n");
        printf("5) Ordenar por codigo de articulo\n");
        printf("6) Buscar articulo por codigo\n");
        printf("7) Salir\n");
        printf("---------------------------------------------------------------\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
       
        switch (opcion) {
            case 1:
                CrearArchivo();
                break;
            case 2:
                printf("Ingrese el codigo de articulo: ");
                scanf("%d", &reg.codigo);
                printf("Ingrese la descripcion: ");
                scanf(" %[^\n]", reg.descripcion);
                printf("Ingrese la cantidad: ");
                scanf("%d", &reg.cantidad);
                printf("Ingrese el precio de costo: ");
                scanf("%f", &reg.precioCosto);
                InsertarNodo(&myList, reg);
                break;
            case 3:
                printf("Ingrese el codigo de articulo a eliminar: ");
                scanf("%d", &reg.codigo);
                EliminarNodo(&myList, reg.codigo);
                break;
            case 4:
                ImprimirLista(myList);
                break;
            case 5:
                OrdenarNodos(&myList);
                ImprimirLista(myList);
                printf("Lista ordenada correctamente.\n");
                break;
            case 6:
                printf("Ingrese el codigo de articulo a buscar: ");
                scanf("%d", &reg.codigo);
                GuardarListaEnArchivo(myList);
                BuscarNodo(myList, reg.codigo);
                break;
            case 7:
                break;
            default:
                printf("Opcion no valida\n");
        }
    } while (opcion != 7);

    // Guardar información de la lista en el archivo binario al salir del programa
    GuardarListaEnArchivo(myList);

    // Liberar memoria asignada a la lista antes de salir del programa
    LiberarLista(&myList);

    return 0;
}





