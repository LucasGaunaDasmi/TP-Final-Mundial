#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
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
    char fecha[30];
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

typedef struct fase
{
int idFase;
nodoPartido* partidos;
}fase;

///PROTOTIPADO DE FUNCIONES

/// LECTURA DE ARCHIVOS

void leerFechas(char[63][30]);                                                                    ///LEE EL ARCHIVO CON LAS FECHAS (NO ESTA ORDENADAS CRONOLOGICAMENTE 100%, PREGUNTENME EL ORDEN)

nodoEquipo* crearNodoEquipo(Equipo equipo);                                                       ///CREA UN NODO EQUIPO (PARA LA LISTA DE EQUIPOS GENERAL)

void insertarAlFinalEquipo(nodoEquipo**,Equipo aInsertar);                                        ///INSERTA UN NODO EQUIPO A LA LISTA GENERAL DE EQUIPOS

void leerArchivo(nodoEquipo** listaDeEquipos);                                                    ///LEE NUESTRO ARCHIVO CON LOS EQUIPOS Y PROBABILIDADES Y LO CARGA A LA LISTA GENERAL DE EQUIPOS

/// INICIALIZACIONES

void inicializarGrupos(Grupo*, int);                                                              ///LE PONE LAS LETRAS(NOMBRE) A CADA GRUPO DEL ARREGLO DE GRUPOS E INICIALIZA LA LISTA DENTRO DEL ARREGLO

nodoGrupoEquipo* crearNodoGrupoEquipo(Equipo* equipo);                                            ///CREA UN NODO GRUPOEQUIPO (PARA LA LISTA DE EQUIPOS QUE ESTA EN CADA CELDA DEL ARREGLO DE GRUPOS)

void insertarEnElGrupo(Grupo*, int, Equipo*);                                                     ///INSERTA EL NODO GRUPOEQUIPO CREADO EN LA LISTA DE CADA CELDA DEL ARREGLO)

void cargarGrupos(Grupo*, int, nodoEquipo*);                                                      ///FUNCION DE LLAMADA EN EL MAIN QUE INVOCA LAS 3 FUNCIONES ANTERIORES

void mostrarGrupos(Grupo*, int);                                                                  ///MUESTRA (DESPUES TENDRIAMOS QUE HACER UNA PARA MOSTRAR LAS POSICIONES EN ORDEN)

/// SIMULADOR

int existeEquipo(char*, nodoEquipo*);                                                             ///VERIFICA SI UN EQUIPO(STRING) ESTA EN LA LISTA DE EQUIPOS

nodoPartido* crearNodoPartido(Partido);                                                           ///CREA UN NODO CON UNA ESTRUCTURA PARTIDO Y PUNTERO AL SIGUIENTE

void insertarAlFinalNodoPartido(nodoPartido**,Partido); ///REUTILIZAR EN OCTAVOS                  ///RECIBE UN PARTIDO, LO PONE EN UN NODO E INSERTA EL NODO EN LA LISTA

Equipo* getEquipo(nodoGrupoEquipo*,int);                                                          ///DEVUELVE UN EQUIPO DE UN GRUPO SEGUN SU POSICION EN LA SUBLISTA

void definirGolesVictoria(int*, int, int*, int);                                                  ///EN CASO DE VICTORIA, ESTA FUNCION ELIGE UN RESULTADO CONSIDERANDO EL DESEMPEÑO DE LOS EQUIPOS

void resultadoDelEmpate(int*, int*);                                                              ///EN CASO DE EMPATE, ESTA FUNCION ELIGE UN RESULTADO

void agregarPartido(nodoPartido**, Equipo*, Equipo*,char[][30]);  ///REUTILIZAR EN OCTAVOS        ///DECIDE LOS RESULTADOS DEL PARTIDO, LOS GUARDA EN UNA ESTRUCTURA PARTIDO Y SE LO PASA A INSERTAR_AL_FINAL_NODO_PARTIDO

void cargarPartidosGrupos(nodoPartido**, Grupo, char[][30]);                                      ///REALIZA LOS ENFRENTAMIENTOS DE UN DETERMINADO GRUPO

bool comparar_puntos(Equipo*, Equipo*);                                                           ///COMPARA LOS PUNTOS Y GOLES DE 2 EQUIPOS, TRUE = TIENE MAS PUNTOS EL SEGUNDO, FALSE = TIENE MAS PUNTOS EL PRIMERO

void insertar_equipo_ordenado(nodoGrupoEquipo**, Equipo*);                                        ///INSERCION PARA LA FUNCION DE ORDENAR POR PUNTOS

void ordenar_grupo_por_puntos(nodoGrupoEquipo**);                                                 ///ORDENA LA LISTA DE EQUIPOS DEL GRUPO (PRIMERO EL QUE TIENE MAS PUNTOS)

bool is_in_grupo(nodoGrupoEquipo*, char*);                                                        ///SE FIJA SI UN EQUIPO(STRING) ESTA EN UN GRUPO, TRUE = ESTA, FALSE = NO ESTA

bool clasifica(nodoGrupoEquipo*, char*);                                                          ///SE FIJA SI UN EQUIPO(STRING) CLASIFICA(LA LIST DE EQUIPOS DEBE ESTAR ORDENADA POR POSICION), TRUE = CLAS, FALSE = NO

void crearArregloGrupoPartidosRandom(GrupoPartido*, Grupo*, char[][30]);                          ///LLAMA A LAS FUNCIONES NECESARIAS, LOS GRUPOS SE JUEGAN DE FORMA ALEATORIA (TENIENDO EN CUENTA PROBABILIDADES)

Equipo* get_equipo_por_nombre(nodoGrupoEquipo*, char*);                                           ///DEVUELVE UN EQUIPO QUE BUSCA POR NOMBRE

void crearArregloGrupoPartidosManipulado(GrupoPartido*, Grupo*, char*, bool, char[][30]);         ///LLAMA A LAS FUNCIONES NECESARIAS, EL GRUPO DEL EQ SELECCIONADO SE MAINPULA, LOS DEMAS SON RANDOM (CON PROB)

void jugarFaseDeGrupos(GrupoPartido*, Grupo* , char [][30], nodoEquipo*, fase[]);                 ///FUNCION PRINICIPAL DE LA FASE DE GRUPOS

void print_partido(Partido*);                                                                     ///IMPRIME LOS RESULTADOS DE UN PARTIDO


///IMPLEMENTACION DE FUNCIONES

/// LECTURA DE ARCHIVOS

void leerFechas(char fechas[63][30])
{
    char unaFecha[30];
    FILE* fp = fopen("fechas.bin","rb");
    if(fp)
    {
        for(int i = 0; i <63; i++)
        {
            fread((&unaFecha),sizeof(char[30]),1,fp);
            strcpy(fechas[i],unaFecha);
        }
    }
}

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

/// INICIALIZACIONES

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

        nodoGrupoEquipo* seg = grupos[i].equipos;

        while(seg != NULL)
        {
            printf("%s\t",seg->equipo->nomEquipo);
            {
                if(strlen(seg->equipo->nomEquipo)<=7)
                {
                    printf("\t");
                }
            }
            printf("%i\t", seg->equipo->mp);                                                  ///JUGADOS
            printf("%i\t", seg->equipo->win);                                                 ///GANADOS
            printf("%i\t", seg->equipo->mp - (seg->equipo->win + seg->equipo->loss));         ///EMPATADOS
            printf("%i\t", seg->equipo->loss);                                                ///PERDIDOS
            printf("%i\t", seg->equipo->gf);                                                  ///GOLES A FAVOR
            printf("%i\t", seg->equipo->ga);                                                  ///GOLES EN CONTRA
            printf("%i\t", seg->equipo->gf - seg->equipo->ga);                                ///DIFERENCIA DE GOL
            printf("%i\n", seg->equipo->pts);                                                 ///PUNTOS
            seg = seg->siguiente;
        }
        printf("----------------------------------------------------------------------------\n\n");
    }
}

/// SIMULADOR

int existeEquipo(char* nombreBuscado, nodoEquipo* listaDeEquipos)
{
    if(listaDeEquipos)
    {
        if(strcmpi(listaDeEquipos->equipo.nomEquipo, nombreBuscado) == 0)
        {
            return 1;
        }
        else
        {
            existeEquipo(nombreBuscado,listaDeEquipos->siguiente);
        }
    }
    else
    {
        return 0;
    }
}

nodoPartido* crearNodoPartido(Partido aInsertar)
{
    nodoPartido* nuevo = (nodoPartido*)malloc(sizeof(nodoPartido));
    nuevo->partido = aInsertar;
    nuevo->siguiente = NULL;

    return nuevo;
}

void insertarAlFinalNodoPartido(nodoPartido** lista,Partido aInsertar)
{
    nodoPartido* nuevo = crearNodoPartido(aInsertar);
    nodoPartido* seg;

    if(*lista == NULL)
    {
        (*lista) = nuevo;
    }
    else
    {
        seg = *lista;

        while(seg->siguiente != NULL)
        {
            seg = seg->siguiente;
        }

        seg->siguiente = nuevo;
    }
}

Equipo* getEquipo(nodoGrupoEquipo* grupo, int indexEquipo)
{
    for (int i = 0; i < indexEquipo; i++)
    {
        grupo = grupo->siguiente;
    }

    return grupo->equipo;
}

void definirGolesVictoria(int* golesGanador, int probabilidadGanador, int* golesPerdedor, int posibilidadesPerdedor)
{
    int numeroRandom;

    if(probabilidadGanador <= 10)
    {
        numeroRandom = rand()%101;
        if(numeroRandom <= 55)
        {
            (*golesGanador) = 1;
        }
        if(55 < numeroRandom && numeroRandom <= 80)
        {
            (*golesGanador) = 2;
        }
        if(80 < numeroRandom)
        {
            (*golesGanador) = 3;
        }

        numeroRandom = rand()%101;
        if(numeroRandom <= 68)
        {
            (*golesPerdedor) = 0;
        }
        if(68 < numeroRandom && numeroRandom <= 88)
        {
            (*golesPerdedor) = 1;
        }
        if(88 < numeroRandom)
        {
            (*golesPerdedor) = 2;
        }
    }

    if(10 < probabilidadGanador && probabilidadGanador <= 60)
    {
        numeroRandom = rand()%101;
        if(numeroRandom <= 35)
        {
            (*golesGanador) = 1;
        }
        if(35 < numeroRandom && numeroRandom <= 80)
        {
            (*golesGanador) = 2;
        }
        if(80 < numeroRandom && numeroRandom <= 90)
        {
            (*golesGanador) = 3;
        }
        if(90 < numeroRandom && numeroRandom <= 97)
        {
            (*golesGanador) = 4;
        }
        if(97 < numeroRandom)
        {
            (*golesGanador) = 5;
        }

        numeroRandom = rand()%101;
        if(numeroRandom <= 30)
        {
            (*golesPerdedor) = 0;
        }
        if(30 < numeroRandom && numeroRandom <= 60)
        {
            (*golesPerdedor) = 1;
        }
        if(60 < numeroRandom && numeroRandom <= 90)
        {
            (*golesPerdedor) = 2;
        }
        if(90 < numeroRandom)
        {
            (*golesPerdedor) = 3;
        }
    }

    if(probabilidadGanador > 60)
    {
        numeroRandom = rand()%101;
        if(numeroRandom <= 30)
        {
            (*golesGanador) = 1;
        }
        if(30 < numeroRandom && numeroRandom <= 65)
        {
            (*golesGanador) = 2;
        }
        if(65 < numeroRandom && numeroRandom <= 80)
        {
            (*golesGanador) = 3;
        }
        if(80 < numeroRandom && numeroRandom <= 95)
        {
            (*golesGanador) = 4;
        }
        if(95 < numeroRandom)
        {
            (*golesGanador) = 5;
        }

        numeroRandom = rand()%101;
        if(numeroRandom <= 30)
        {
            (*golesPerdedor) = 0;
        }
        if(30 < numeroRandom && numeroRandom <= 60)
        {
            (*golesPerdedor) = 1;
        }
        if(60 < numeroRandom && numeroRandom <= 90)
        {
            (*golesPerdedor) = 2;
        }
        if(90 < numeroRandom)
        {
            (*golesPerdedor) = 3;
        }
    }
}

void resultadoDelEmpate(int* goles1, int* goles2)
{
    int numeroRandom;
    numeroRandom = rand()%101;

    if(numeroRandom <= 30)
    {
         (*goles1) = 0;
         (*goles2) = 0;
    }
    if(30 < numeroRandom && numeroRandom <= 70)
    {
        (*goles1) = 1;
        (*goles2) = 1;
    }
    if(70 < numeroRandom && numeroRandom <= 95)
    {
        (*goles1) = 2;
        (*goles2) = 2;
    }
    if(95 < numeroRandom)
    {
        (*goles1) = 3;
        (*goles2) = 3;
    }
}

void agregarPartido(nodoPartido** lista, Equipo* eq1, Equipo* eq2, char fechas[][30])
{
    Partido aInsertar;
    aInsertar.equipo1 = eq1;
    aInsertar.equipo2 = eq2;

    int i = 0;
    while(strcmpi(fechas[i],"NULO") == 0)
    {
        i++;
    }

    strcpy(aInsertar.fecha,fechas[i]);
    strcpy(fechas[i],"NULO");

    int probabilidadPrimero = eq1->probabilidad * 1000;
    int probabilidadSegundo = eq2->probabilidad * 1000;
    int probabilidadEmpate;
    int probabilidadTotal;
    int resultado;

    if(probabilidadPrimero >= probabilidadSegundo)
    {
        if(probabilidadPrimero > probabilidadSegundo * 12 && probabilidadSegundo != 0 && probabilidadPrimero != 1000000)
        {
            probabilidadSegundo = probabilidadPrimero / 12;
        }

        probabilidadEmpate = probabilidadSegundo * 1.6;
    }
    else
    {
        if(probabilidadSegundo > probabilidadPrimero * 12 && (probabilidadPrimero != 0) && probabilidadSegundo != 1000000)
        {
            probabilidadPrimero = probabilidadSegundo / 12;
        }

        probabilidadEmpate = probabilidadPrimero * 1.6;
    }

    probabilidadTotal = probabilidadEmpate + probabilidadPrimero + probabilidadSegundo;

    resultado = rand()%(probabilidadTotal+1);

    //printf("======================== P %s = %d ============\n", eq1->nomEquipo, probabilidadPrimero);
    //printf("======================== P %s = %d ============\n", eq2->nomEquipo, probabilidadSegundo);
    if (resultado < probabilidadPrimero)   //gana el primero
    {
        do
        {
            definirGolesVictoria(&(aInsertar.golesEq1), probabilidadPrimero, &(aInsertar.golesEq2), probabilidadSegundo);

        }while(aInsertar.golesEq1 <= aInsertar.golesEq2);

        eq1->ga = eq1->ga + aInsertar.golesEq2;
        eq1->gf = eq1->gf + aInsertar.golesEq1;
        eq1->mp = eq1->mp + 1;
        eq1->win = eq1->win + 1;
        eq1->pts = eq1->pts + 3;

        eq2->ga = eq2->ga + aInsertar.golesEq1;
        eq2->gf = eq2->gf + aInsertar.golesEq2;
        eq2->mp = eq2->mp + 1;
        eq2->loss = eq2->loss + 1;
    }

    if ((resultado >= probabilidadPrimero) && (resultado < probabilidadSegundo + probabilidadPrimero))      //gana el segundo
    {
        do
        {
            definirGolesVictoria(&(aInsertar.golesEq2), probabilidadSegundo, &(aInsertar.golesEq1), probabilidadPrimero);

        }while(aInsertar.golesEq2 <= aInsertar.golesEq1);

        eq2->ga = eq2->ga + aInsertar.golesEq1;
        eq2->gf = eq2->gf + aInsertar.golesEq2;
        eq2->mp = eq2->mp + 1;
        eq2->win = eq2->win + 1;
        eq2->pts = eq2->pts +3;

        eq1->ga = eq1->ga + aInsertar.golesEq2;
        eq1->gf = eq1->gf + aInsertar.golesEq1;
        eq1->mp = eq1->mp + 1;
        eq1->loss = eq1->loss + 1;
    }

    if (resultado >= (probabilidadSegundo + probabilidadPrimero))      //empate
    {
        resultadoDelEmpate(&(aInsertar.golesEq1),&(aInsertar.golesEq2));

        eq1->ga = eq1->ga + aInsertar.golesEq1;
        eq1->gf = eq1->gf + aInsertar.golesEq2;
        eq1->mp = eq1->mp + 1;
        eq1->pts = eq1->pts + 1;

        eq2->ga = eq2->ga + aInsertar.golesEq1;
        eq2->gf = eq2->gf + aInsertar.golesEq2;
        eq2->mp = eq2->mp + 1;
        eq2->pts = eq2->pts + 1;
    }

    ///printf("%s\n",aInsertar.fecha);
    ///printf("%s %i - %i %s\n\n",aInsertar.equipo1, aInsertar.golesEq1, aInsertar.golesEq2, aInsertar.equipo2);

    insertarAlFinalNodoPartido(lista,aInsertar);
}

void cargarPartidosGrupos(nodoPartido** lista, Grupo grupo, char fechas[63][30])
{
    /*printf("%s vs %s\n", getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 1));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 2), getEquipo(grupo.equipos, 3));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 2));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 1));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 1), getEquipo(grupo.equipos, 2));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 0));*/
    agregarPartido(lista, getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 1), fechas);
    agregarPartido(lista, getEquipo(grupo.equipos, 2), getEquipo(grupo.equipos, 3), fechas);
    agregarPartido(lista, getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 2), fechas);
    agregarPartido(lista, getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 1), fechas);
    agregarPartido(lista, getEquipo(grupo.equipos, 1), getEquipo(grupo.equipos, 2), fechas);
    agregarPartido(lista, getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 0), fechas);
}

bool comparar_puntos(Equipo *a, Equipo *b)
{
    if (a->pts < b->pts) return true;
    if (a->pts > b->pts) return false;
    if ((a->gf - a->ga) < (b->gf - b->ga)) return true;
    if ((a->gf - a->ga) > (b->gf - b->ga)) return false;
    if (a->gf < b->gf) return true;
    if (a->gf > b->gf) return false;
}

void insertar_equipo_ordenado(nodoGrupoEquipo **list, Equipo *e)
{
    if (*list == NULL || comparar_puntos((*list)->equipo, e)) {
        nodoGrupoEquipo *aux = crearNodoGrupoEquipo(e);
        aux->siguiente = *list;
        *list = aux;
    } else insertar_equipo_ordenado(&(*list)->siguiente, e);
}

void ordenar_grupo_por_puntos(nodoGrupoEquipo **list)
{
    nodoGrupoEquipo *temp = *list;
    *list = NULL;
    while (temp != NULL) {
        insertar_equipo_ordenado(list, temp->equipo);
        temp = temp->siguiente;
    }
}

bool is_in_grupo(nodoGrupoEquipo *list, char *equipo)
{
    if (list == NULL) return false;
    if (strcmpi(equipo, list->equipo->nomEquipo) == 0) return true;
    return is_in_grupo(list->siguiente, equipo);
}

bool clasifica(nodoGrupoEquipo *list, char *equipo)
{
    if (strcmpi(list->equipo->nomEquipo, equipo) == 0 || strcmpi(list->siguiente->equipo->nomEquipo, equipo) == 0) return true;
    return false;
}

void crearArregloGrupoPartidosRandom(GrupoPartido* partidosGrupo, Grupo* grupos,char fechas[][30])
{
    for (int i = 0; i < VALIDOS_GRUPO; i++)
    {
        partidosGrupo[i].letra = grupos[i].letra;
        partidosGrupo[i].partidos = NULL;
        cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i], fechas);
        ordenar_grupo_por_puntos(&grupos[i].equipos);
    }
}

Equipo* get_equipo_por_nombre(nodoGrupoEquipo *list, char *equipo)
{
    if (strcmpi(list->equipo->nomEquipo, equipo) == 0) return list->equipo;
    return get_equipo_por_nombre(list->siguiente, equipo);
}

void crearArregloGrupoPartidosManipulado(GrupoPartido partidosGrupo[VALIDOS_GRUPO], Grupo grupos[VALIDOS_GRUPO], char* equipoElegido, bool opcion, char fechas[][30])
{
    bool flag;

    for (int i = 0; i < VALIDOS_GRUPO; i++)
    {
        if(!is_in_grupo(grupos[i].equipos, equipoElegido))       /// HACEMOS RANDOM NORMAL
        {
            partidosGrupo[i].letra = grupos[i].letra;
            partidosGrupo[i].partidos = NULL;
            cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i], fechas);
            ordenar_grupo_por_puntos(&grupos[i].equipos);
        }
        else
        {
            partidosGrupo[i].letra = grupos[i].letra;
            partidosGrupo[i].partidos = NULL;
            ///printf("Esta en el grupo %c",grupos[i].letra);
            float probabilidad = grupos[i].equipos->equipo->probabilidad; // guardo la probabilidad
            Equipo *e = get_equipo_por_nombre(grupos[i].equipos, equipoElegido); // busco el equipo
            if (opcion == true) e->probabilidad = 1000; // modifico la probabilidad segun la opcion
            if (opcion == false) e->probabilidad = 0;
            cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i], fechas); // cargo los equipos con la probabilidad modificada
            e->probabilidad = probabilidad; // vuelvo a poner la probabilidad original
            ordenar_grupo_por_puntos(&grupos[i].equipos);
        }
    }
}

void mostrarPartidosGrupos(GrupoPartido* partidosGrupo)
{
    for(int i = 0; i < VALIDOS_GRUPO; i++)
    {
        printf("Resultados grupo %c:\n\n",partidosGrupo[i].letra);
        for(int j = 0; j < 6; j++)
        {
            printf("%s\n",partidosGrupo->partidos->partido.fecha);
            printf("%s %i - %i %s\n\n",partidosGrupo->partidos->partido.equipo1, partidosGrupo->partidos->partido.golesEq1, partidosGrupo->partidos->partido.golesEq2, partidosGrupo->partidos->partido.equipo2);
            partidosGrupo->partidos = partidosGrupo->partidos->siguiente;
        }
        printf("\n");
    }
}

void muestraClasificados(fase fases[])
{
    nodoPartido* seg = fases[0].partidos;
    printf("CLASIFICADOS A OCTAVOS DE FINAL\n");

    for(int i = 0; i < 8; i++)
    {
        printf("\n%s\n", seg->partido.fecha);
        printf("%s\n",seg->partido.equipo1);
        printf("%s\n",seg->partido.equipo2);
        seg = seg->siguiente;
    }
}

void penales(Partido* partido)
{
    int definicion;
    partido->penales1 = rand()%6;
    partido->penales2 = rand()%6;

    if(partido->penales1 == partido->penales2)
    {
        definicion = rand()%2;
        if(definicion == 0)
        {
            (partido->penales1)++;
        }
        else
        {
            (partido->penales2)++;
        }
    }
}

void organizarOctavos(fase fases[], Grupo grupos[], char fechas [][30])
{
    fases[0].idFase = 0;
    nodoGrupoEquipo* seg;
    nodoGrupoEquipo* seg2;
    fases[0].partidos = NULL;

    /// O1
    seg = grupos[0].equipos->equipo;
    seg2 = grupos[1].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O2
    seg = grupos[2].equipos->equipo;
    seg2 = grupos[3].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O3
    seg = grupos[4].equipos->equipo;
    seg2 = grupos[5].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O4
    seg = grupos[6].equipos->equipo;
    seg2 = grupos[7].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O5
    seg = grupos[1].equipos->equipo;
    seg2 = grupos[0].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O6
    seg = grupos[3].equipos->equipo;
    seg2 = grupos[2].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O7
    seg = grupos[5].equipos->equipo;
    seg2 = grupos[4].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }

    /// O8
    seg = grupos[7].equipos->equipo;
    seg2 = grupos[6].equipos->siguiente->equipo;

    agregarPartido(&(fases[0].partidos), seg, seg2, fechas);

    if(fases[0].partidos->partido.golesEq1 == fases[0].partidos->partido.golesEq2)
    {
        penales(&(fases[0].partidos->partido));
    }
    else
    {
        fases[0].partidos->partido.penales1 = 0;
        fases[0].partidos->partido.penales2 = 0;
    }
}

Equipo* ganadorLlave(nodoPartido* partidos)
{
    if(partidos->partido.golesEq1 > partidos->partido.golesEq2)
    {
        return partidos->partido.equipo1;
    }
    if(partidos->partido.golesEq1 < partidos->partido.golesEq2)
    {
        return partidos->partido.equipo2;
    }
    if(partidos->partido.golesEq1 == partidos->partido.golesEq2)
    {
        if(partidos->partido.penales1 > partidos->partido.penales2)
        {
            return partidos->partido.equipo1;
        }
        else
        {
            return partidos->partido.equipo1;
        }
    }
}

void organizarCuartos(fase fases[], char fechas [][30])
{
    fases[1].idFase = 1;
    fases[1].partidos = NULL;
    nodoPartido* seg = fases[0].partidos;

    ///C1 O1vsO2

    Equipo* eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    Equipo* eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[1].partidos), eq1, eq2, fechas);

    if(fases[1].partidos->partido.golesEq1 == fases[1].partidos->partido.golesEq2)
    {
        penales(&(fases[1].partidos->partido));
    }
    else
    {
        fases[1].partidos->partido.penales1 = 0;
        fases[1].partidos->partido.penales2 = 0;
    }

    ///C2 O3vsO4

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[1].partidos), eq1, eq2, fechas);

    if(fases[1].partidos->partido.golesEq1 == fases[1].partidos->partido.golesEq2)
    {
        penales(&(fases[1].partidos->partido));
    }
    else
    {
        fases[1].partidos->partido.penales1 = 0;
        fases[1].partidos->partido.penales2 = 0;
    }

    ///C3 O5vsO6

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[1].partidos), eq1, eq2, fechas);

    if(fases[1].partidos->partido.golesEq1 == fases[1].partidos->partido.golesEq2)
    {
        penales(&(fases[1].partidos->partido));
    }
    else
    {
        fases[1].partidos->partido.penales1 = 0;
        fases[1].partidos->partido.penales2 = 0;
    }

    ///C4 O7vsO8

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[1].partidos), eq1, eq2, fechas);

    if(fases[1].partidos->partido.golesEq1 == fases[1].partidos->partido.golesEq2)
    {
        penales(&(fases[1].partidos->partido));
    }
    else
    {
        fases[1].partidos->partido.penales1 = 0;
        fases[1].partidos->partido.penales2 = 0;
    }
}

void organizarSemis(fase fases[], char fechas [][30])
{
    fases[2].idFase = 2;
    fases[2].partidos = NULL;
    nodoPartido* seg = fases[1].partidos;

    ///S1 C1vsC2

    Equipo* eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    Equipo* eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[2].partidos), eq1, eq2, fechas);

    if(fases[2].partidos->partido.golesEq1 == fases[2].partidos->partido.golesEq2)
    {
        penales(&(fases[2].partidos->partido));
    }
    else
    {
        fases[2].partidos->partido.penales1 = 0;
        fases[2].partidos->partido.penales2 = 0;
    }

    ///S2 C2VSC3

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[2].partidos), eq1, eq2, fechas);

    if(fases[2].partidos->partido.golesEq1 == fases[2].partidos->partido.golesEq2)
    {
        penales(&(fases[2].partidos->partido));
    }
    else
    {
        fases[2].partidos->partido.penales1 = 0;
        fases[2].partidos->partido.penales2 = 0;
    }
}

void organizarFinal(fase fases[], char fechas [][30])
{
    fases[3].idFase = 3;
    fases[3].partidos = NULL;
    nodoPartido* seg = fases[2].partidos;

    ///F S1vsS2

    Equipo* eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    Equipo* eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartido(&(fases[3].partidos), eq1, eq2, fechas);

    if(fases[3].partidos->partido.golesEq1 == fases[3].partidos->partido.golesEq2)
    {
        penales(&(fases[3].partidos->partido));
    }
    else
    {
        fases[3].partidos->partido.penales1 = 0;
        fases[3].partidos->partido.penales2 = 0;
    }

}

void simulacion(GrupoPartido* partidosGrupo, Grupo* grupos, char fechas[][30], nodoEquipo* listaDeEquipos, fase fases[])
{
    ///FASE DE GRUPOS

    int opcion;
    int opcion2;
    bool opc2;
    int flag;
    char equipoElegido[20];

    printf("Seleccione una opcion:\n");
    printf("1) Decidir todos los resultados por probabilidad.\n");
    printf("2) Elegir el resultado de un equipo.\n");
    fflush(stdin);
    scanf("%i",&opcion);
    while(opcion != 1 && opcion !=2)
    {
        printf("\nOpcion incorrecta, vuelva a intentarlo.\n");
        fflush(stdin);
        scanf("%i",&opcion);
    }
    switch(opcion)
    {
    case 1:/// OPCION TODO RANDOM
        system("cls");
        crearArregloGrupoPartidosRandom(partidosGrupo, grupos, fechas);
        ///mostrarResultadosPartidosGrupos(partidosGrupo);
        mostrarPartidosGrupos(partidosGrupo);
        printf("POSICIONES FINALES:\n");
        mostrarGrupos(grupos, VALIDOS_GRUPO);
        break;

    case 2:/// OPCION ELEGIR UN EQUIPO Y SU RESULTADO
        printf("\nElija el equipo: ");
        fflush(stdin);
        gets(equipoElegido);
        flag = existeEquipo(equipoElegido, listaDeEquipos);

        while(flag == 0)
        {
            printf("El equipo no existe, vuelva a intentarlo: ");
            fflush(stdin);
            gets(equipoElegido);
            flag = existeEquipo(equipoElegido, listaDeEquipos);
        }

        printf("\nSeleccione una opcion:\n");
        printf("1) Clasifica.\n");
        printf("2) No clasifica.\n");
        fflush(stdin);
        scanf("%i",&opcion2);

        while(opcion2 != 1 && opcion2 !=2)
        {
            printf("\nOpcion incorrecta, vuelva a intentarlo.\n");
            fflush(stdin);
            scanf("%i", &opcion2);
        }
        if (opcion2 == 1) opc2 = true; // true
        if (opcion2 == 2) opc2 = false; // false
        system("cls");
        crearArregloGrupoPartidosManipulado(partidosGrupo, grupos, equipoElegido, opc2, fechas);  ///OPCION ELEGIR RESULTADO (1= clasifica) (2= no clasifica)
        mostrarPartidosGrupos(partidosGrupo);
        printf("POSICIONES FINALES:\n");
        mostrarGrupos(grupos, VALIDOS_GRUPO);
        break;
    }

    ///ORGANIZAR OCTAVOS Y MUESTRA CLASIFICADOS
    organizarOctavos(fases, grupos, fechas);
    muestraClasificados(fases);
    printf("\n");
    system("pause");

    ///DEMAS FASES FINALES
    organizarCuartos(fases, fechas);
    organizarSemis(fases, fechas);
    organizarFinal(fases, fechas);
}

///MAIN

int main()
{
    srand(time(NULL));
    printf("SIMULADOR DEL MUNDIAL QATAR 2022:\n\n");
    Grupo grupos[VALIDOS_GRUPO];                                          ///ARREGLO DE CADA GRUPO (TIENE UNA LETRA Y UNA LISTA DE 4 PUNTEROS A EQUIPOS)
    char fechas[63][30];                                                  ///ARREGLO CON TODAS LAS FECHAS EN FORMATO STRING (p.ej. "25 de noviembre 10:00")
    nodoEquipo* listaDeEquipos = NULL;                                    ///LISTA DE TODOS LOS EQUIPOS
    GrupoPartido partidosGrupo[VALIDOS_GRUPO];                            ///ARREGLO DE PARTIDOS, CADA CELDA ES UN GRUPO CON 6 PARTIDOS (LISTA)
    fase fases[4];                                                        ///ARREGLO DE FASES

    /// LECTURA DE ARCHIVOS
    leerArchivo(&listaDeEquipos);
    leerFechas(fechas);

    /// INICIALIZACIONES
    inicializarGrupos(grupos, VALIDOS_GRUPO);
    cargarGrupos(grupos, VALIDOS_GRUPO, listaDeEquipos);
    mostrarGrupos(grupos, VALIDOS_GRUPO);

    /// SIMULADOR
    simulacion(partidosGrupo, grupos, fechas, listaDeEquipos, fases);

    ///VISUALIZAR RESULTADOS


    return 0;
}
