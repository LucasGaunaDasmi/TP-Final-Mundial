#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VALIDOS_GRUPO 8

///ESTRUCTURAS

typedef struct Equipo
{
    char nomEquipo[40];
    int mp;              ///partidos jugados
    int gf;              ///goles a favor
    int ga;              ///goles en contra
    int win;             ///partidos ganados
    int loss;            ///partidos perdidos
    int pts;             ///puntos totales
    float probabilidad;  ///probabilidad de ganar el mundial
}Equipo;

typedef struct nodoEquipo
{
    Equipo equipo;
    struct nodoEquipo* siguiente;
}nodoEquipo;

typedef struct Grupo
{
    char letra;                         ///'A','B'....
    struct nodoGrupoEquipo* equipos;    ///siempre va a haber 4
}Grupo;

typedef struct nodoGrupoEquipo
{
    Equipo* equipo;
    struct nodoGrupoEquipo* siguiente;
}nodoGrupoEquipo;

typedef struct Partido
{
    int fecha; ///eligen ustedes el criterio
    Equipo* equipo1;
    Equipo* equipo2;
    int golesEq1;
    int golesEq2;
    int penales1; ///en la fase de grupos no hay penales
    int penales2;
}Partido;

typedef struct nodoPartido
{
   Partido partido;
   struct nodoPartido* siguiente;
}nodoPartido;

typedef struct GrupoPartido
{
   char letra;
   nodoPartido* partidos;
}GrupoPartido;


///PROTOTIPADO DE FUNCIONES

nodoEquipo* crearNodoEquipo(Equipo equipo);                                 ///CREA UN NODO EQUIPO (PARA LA LISTA DE EQUIPOS GENERAL)
void insertarAlFinalEquipo(nodoEquipo**,Equipo aInsertar);                  ///INSERTA UN NODO EQUIPO A LA LISTA GENERAL DE EQUIPOS
void leerArchivo(nodoEquipo** listaDeEquipos);                              ///LEE NUESTRO ARCHIVO CON LOS EQUIPOS Y PROBABILIDADES Y LO CARGA A LA LISTA GENERAL DE EQUIPOS
void inicializarGrupos(Grupo* grupos, int validosGrupo);                    ///LE PONE LAS LETRAS(NOMBRE) A CADA GRUPO DEL ARREGLO DE GRUPOS E INICIALIZA LA LISTA DENTRO DEL ARREGLO
nodoGrupoEquipo* crearNodoGrupoEquipo(Equipo* equipo);                      ///CREA UN NODO GRUPOEQUIPO (PARA LA LISTA DE EQUIPOS QUE ESTA EN CADA CELDA DEL ARREGLO DE GRUPOS)
void insertarEnElGrupo(Grupo*, int, Equipo*);                               ///INSERTA EL NODO GRUPOEQUIPO CREADO EN LA LISTA DE CADA CELDA DEL ARREGLO)
void cargarGrupos(Grupo*, int, nodoEquipo*);                                ///FUNCION DE LLAMADA EN EL MAIN QUE INVOCA LAS 3 FUNCIONES ANTERIORES
void mostrarGrupos(Grupo*, int);                                            ///MUESTRA (DESPUES TENDRIAMOS QUE HACER UNA PARA MOSTRAR LAS POSICIONES EN ORDEN)


///IMPLEMENTACION DE FUNCIONES

nodoEquipo* crearNodoEquipo(Equipo equipo)
{
    nodoEquipo* nuevo = (nodoEquipo*)malloc(sizeof(nodoEquipo));
    nuevo->equipo = equipo;
    nuevo->siguiente = NULL;

    return nuevo;
}

void insertarAlFinalEquipo(nodoEquipo** listaDeEquipos, Equipo aInsertar)
{
    nodoEquipo* nuevo = crearNodoEquipo(aInsertar);
    nodoEquipo* seg;

    if(*listaDeEquipos == NULL)
    {
        (*listaDeEquipos) = nuevo;
    }
    else
    {
        seg = *listaDeEquipos;

        while(seg->siguiente != NULL)
        {
            seg = seg->siguiente;
        }

        seg->siguiente = nuevo;
    }
}

void leerArchivo(nodoEquipo** listaDeEquipos)
{
    Equipo aInsertar;
    FILE* fp = fopen("equipo.bin","rb");
    if(fp)
    {
        while(fread(&aInsertar,sizeof(Equipo),1,fp)>0)
        {
            insertarAlFinalEquipo(listaDeEquipos,aInsertar);
        }
        fclose(fp);
    }
}

void inicializarGrupos(Grupo* grupos, int validosGrupo)
{
    char letras[8]= "ABCDEFGH";

    int i = 0;

    while(i < validosGrupo)
    {
        grupos[i].letra = letras[i];
        grupos[i].equipos = NULL;
        i++;
    }
}

nodoGrupoEquipo* crearNodoGrupoEquipo(Equipo* equipo)
{
    nodoGrupoEquipo* nuevo = (nodoGrupoEquipo*)malloc(sizeof(nodoGrupoEquipo));
    nuevo->equipo = equipo;
    nuevo->siguiente = NULL;

    return nuevo;
}

void insertarEnElGrupo(Grupo* grupos, int i, Equipo* punteroAEquipo)
{
    nodoGrupoEquipo* nuevo = crearNodoGrupoEquipo(punteroAEquipo);
    nodoGrupoEquipo* seg;

    if(grupos[i].equipos == NULL)
    {
        grupos[i].equipos = nuevo;
    }
    else
    {
        seg = grupos[i].equipos;

        while(seg->siguiente != NULL)
        {
            seg = seg->siguiente;
        }

        seg->siguiente = nuevo;
    }
}

void cargarGrupos(Grupo* grupos, int validos, nodoEquipo* listaDeEquipos)
{
    for(int i = 0; i < validos; i++)
    {
        int j = 0;

        while(j < 4)
        {
            Equipo* punteroAEquipo = &(listaDeEquipos->equipo);
            insertarEnElGrupo(grupos,i,punteroAEquipo);
            j++;
            listaDeEquipos = listaDeEquipos->siguiente;
        }
    }
}

void mostrarGrupos(Grupo* grupos, int validos)
{
    for(int i = 0; i < validos; i++)
    {
        printf("----------------------------------------------------------------------------\n");
        printf("GRUPO %c\n",grupos[i].letra);
        printf("\t\tMP\tW\tD\tL\tGF\tGA\tGD\tPts\n");

        while(grupos[i].equipos != NULL)
        {
            printf("%s\t",grupos[i].equipos->equipo->nomEquipo);
            {
                if(strlen(grupos[i].equipos->equipo->nomEquipo)<=7)
                {
                    printf("\t");
                }
            }
            printf("%i\t", grupos[i].equipos->equipo->mp);                                                                       ///JUGADOS
            printf("%i\t", grupos[i].equipos->equipo->win);                                                                      ///GANADOS
            printf("%i\t", grupos[i].equipos->equipo->mp - grupos[i].equipos->equipo->win - grupos[i].equipos->equipo->loss);    ///EMPATADOS
            printf("%i\t", grupos[i].equipos->equipo->loss);                                                                     ///PERDIDOS
            printf("%i\t", grupos[i].equipos->equipo->gf);                                                                       ///GOLES A FAVOR
            printf("%i\t", grupos[i].equipos->equipo->ga);                                                                       ///GOLES EN CONTRA
            printf("%i\t", grupos[i].equipos->equipo->gf - grupos[i].equipos->equipo->ga);                                       ///DIFERENCIA DE GOL
            printf("%i\n", grupos[i].equipos->equipo->pts);                                                                      ///PUNTOS
            grupos[i].equipos = grupos[i].equipos->siguiente;
        }

        printf("----------------------------------------------------------------------------\n\n");
    }
}


///MAIN

int main()
{
    Grupo grupos[VALIDOS_GRUPO];
    nodoEquipo* listaDeEquipos = NULL;

    leerArchivo(&listaDeEquipos);
    inicializarGrupos(grupos, VALIDOS_GRUPO);
    cargarGrupos(grupos, VALIDOS_GRUPO, listaDeEquipos);

    mostrarGrupos(grupos, VALIDOS_GRUPO);

    return 0;
}
