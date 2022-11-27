#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <conio.h>
#include <Windows.h>
#define VALIDOS_GRUPO 8
#define MAX_OPTION_LEN 40


#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75
#define RIGHT_KEY 77
#define ENTER_KEY 13
#define X_KEY 120
#define ESC_KEY 27

#define DEFAULT_COLOR 7
#define HOVER_COLOR 127
#define SELECTED_COLOR 14

#define clear() printf("\033[H\033[J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

int back_color(int num)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), num);
    return 0;
}


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

typedef struct menu
{
    char opciones[5][MAX_OPTION_LEN];
    int cantOpciones;
} menu;


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

void muestraClasificados(fase[]);                                                                 ///MUESTRA A LAS SELECCIONES CLASIFICADAS A OCTAVOS

void resultadoDelEmpateConPenales(int*, int*, int*, int*);                                        ///DEFINE EL RESULTADO DEL PARTIDO EN CASO DE EMPATE, AGREGANDO PENALES POR SER FASE DE ELIMINACION

void agregarPartidoConPenales(nodoPartido**, Equipo*, Equipo*, char[][30]);                       ///FUNCION QUE AGREGA UN PARTIDO DE FASE DE ELIMINACION DIRECTA

void organizarOctavos(fase[], Grupo[], char[][30]);                                               ///ORGANIZA LOS CRUCES DE OCTAVOS DE FINAL

Equipo* ganadorLlave(nodoPartido*);                                                               ///DEVUELVE UN PUNTERO AL EQUIPO QUE GANO UNA FASE

void organizarCuartos(fase[], char[][30]);                                                        ///ORGANIZA LOS CRUCES DE CUARTOS DE FINAL

void organizarSemis(fase[], char[][30]);                                                          ///ORGANIZA LOS CRUCES DE SEMIFINALES

void organizarFinal(fase[], char[][30]);                                                          ///ORGANIZA LOS EQUIPOS GANADORES DE SEMIS EN LA FINAL

void print_partido(Partido*);                                                                     ///IMPRIME UN PARTIDO CON SU FECHA EN PANTALLA


///FUNCIONES MAIN

void clrscr();                                                                                    ///LIMPIA LA PANTALLA

void hidecursor();                                                                                ///ESCONDE EL CURSOR DE LA CONSOLA

void showcursor();                                                                                ///HACE REAPARECER EL CURSOR DE LA CONSOLA

void load_menus();                                                                                ///CARGA LOS MENUS EN MEMORIA    

void print_menu(menu, int);                                                                       ///IMPRIME LAS OPCIONES DEL MENU EN PANTALLA Y RESALTA LA OPCION SELECCIONADA

void mostrarTablaGrupo(Grupo);                                                                    ///MUESTRA LA TABLA DE POSICIONES DE TODOS LOS GRUPOS

void mostrarTablasGrupos(Grupo[8]);                                                               ///MUESTRA LAS TABLAS DE POSICIONES DE UN GRUPO

void mostrarPartidosGrupos(GrupoPartido[8]);                                                      ///MUESTRA LOS PARTIDOS DE UN GRUPO

char get_equipo(nodoEquipo);                                                                      ///DEVUELVE UN EQUIPO INGRESADO POR EL USUARIO QUE EXISTA EN EL MUNDIAL

void print_title();                                                                               ///IMPRIME EL TITULO DEL PROGRAMA

void print_partidos(Grupo);                                                                       ///IMPRIME LOS PARTIDOS DE UN GRUPO

void volver_button();                                                                              ///BOTON PARA VOLVER AL MENU ANTERIOR


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

    insertarAlFinalNodoPartido(lista,aInsertar);
}

void cargarPartidosGrupos(nodoPartido** lista, Grupo grupo, char fechas[63][30])
{
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
        cargarPartidosGrupos(&(partidosGrupo[i].partidos), grupos[i], fechas);
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
            cargarPartidosGrupos(&(partidosGrupo[i].partidos), grupos[i], fechas);
            ordenar_grupo_por_puntos(&grupos[i].equipos);
        }
        else
        {
            partidosGrupo[i].letra = grupos[i].letra;
            partidosGrupo[i].partidos = NULL;
            float probabilidad = grupos[i].equipos->equipo->probabilidad; // guardo la probabilidad
            Equipo *e = get_equipo_por_nombre(grupos[i].equipos, equipoElegido); // busco el equipo
            if (opcion == true) e->probabilidad = 1000; // modifico la probabilidad segun la opcion
            if (opcion == false) e->probabilidad = 0;
            cargarPartidosGrupos(&(partidosGrupo[i].partidos), grupos[i], fechas); // cargo los equipos con la probabilidad modificada
            e->probabilidad = probabilidad; // vuelvo a poner la probabilidad original
            ordenar_grupo_por_puntos(&grupos[i].equipos);
        }
    }
}

void muestraClasificados(fase fases[])
{
    nodoPartido* seg = fases[0].partidos;
    printf("CLASIFICADOS A OCTAVOS DE FINAL:\n\n");

    for(int i = 0; i < 8; i++)
    {
        printf("%s vs.",seg->partido.equipo1);
        printf("%s\n",seg->partido.equipo2);
        seg = seg->siguiente;
    }
}

void resultadoDelEmpateConPenales(int* goles1, int* goles2, int* penales1, int* penales2)
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

    int definicion;
    (*penales1) = rand()%6;
    (*penales2) = rand()%6;

    while((*penales1) == (*penales2))
    {
        definicion = rand()%2;
        if(definicion == 0)
        {
            (*penales1)++;
        }
        else
        {
            (*penales2)++;
        }
    }
}

void agregarPartidoConPenales(nodoPartido** lista, Equipo* eq1, Equipo* eq2, char fechas[][30])
{
    Partido aInsertar;
    aInsertar.equipo1 = eq1;
    aInsertar.equipo2 = eq2;
    aInsertar.penales1 = 0;
    aInsertar.penales2 = 0;

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

    if (resultado < probabilidadPrimero)   //gana el primero
    {
        do
        {
            definirGolesVictoria(&(aInsertar.golesEq1), probabilidadPrimero, &(aInsertar.golesEq2), probabilidadSegundo);

        }while(aInsertar.golesEq1 <= aInsertar.golesEq2);
    }

    if ((resultado >= probabilidadPrimero) && (resultado < probabilidadSegundo + probabilidadPrimero))      //gana el segundo
    {
        do
        {
            definirGolesVictoria(&(aInsertar.golesEq2), probabilidadSegundo, &(aInsertar.golesEq1), probabilidadPrimero);

        }while(aInsertar.golesEq2 <= aInsertar.golesEq1);
    }

    if (resultado >= (probabilidadSegundo + probabilidadPrimero))      //empate
    {
        resultadoDelEmpateConPenales(&(aInsertar.golesEq1),&(aInsertar.golesEq2),&(aInsertar.penales1),&(aInsertar.penales2));
    }


    insertarAlFinalNodoPartido(lista,aInsertar);
}

void organizarOctavos(fase fases[], Grupo grupos[], char fechas [][30])
{
    fases[0].idFase = 0;
    Equipo *seg;
    Equipo *seg2;
    fases[0].partidos = NULL;

    /// O1
    seg = grupos[0].equipos->equipo;
    seg2 = grupos[1].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O2
    seg = grupos[2].equipos->equipo;
    seg2 = grupos[3].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O3
    seg = grupos[4].equipos->equipo;
    seg2 = grupos[5].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O4
    seg = grupos[6].equipos->equipo;
    seg2 = grupos[7].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O5
    seg = grupos[1].equipos->equipo;
    seg2 = grupos[0].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O6
    seg = grupos[3].equipos->equipo;
    seg2 = grupos[2].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O7
    seg = grupos[5].equipos->equipo;
    seg2 = grupos[4].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);

    /// O8
    seg = grupos[7].equipos->equipo;
    seg2 = grupos[6].equipos->siguiente->equipo;

    agregarPartidoConPenales(&(fases[0].partidos), seg, seg2, fechas);
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
            return partidos->partido.equipo2;
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

    agregarPartidoConPenales(&(fases[1].partidos), eq1, eq2, fechas);

    ///C2 O3vsO4

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartidoConPenales(&(fases[1].partidos), eq1, eq2, fechas);

    ///C3 O5vsO6

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartidoConPenales(&(fases[1].partidos), eq1, eq2, fechas);

    ///C4 O7vsO8

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartidoConPenales(&(fases[1].partidos), eq1, eq2, fechas);
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

    agregarPartidoConPenales(&(fases[2].partidos), eq1, eq2, fechas);

    ///S2 C2VSC3

    eq1 = ganadorLlave(seg);
    seg = seg->siguiente;
    eq2 = ganadorLlave(seg);
    seg = seg->siguiente;

    agregarPartidoConPenales(&(fases[2].partidos), eq1, eq2, fechas);
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

    agregarPartidoConPenales(&(fases[3].partidos), eq1, eq2, fechas);
}

void muestraFasesFinalesConResultados(fase fases[], int i)
{
    nodoPartido* auxiliar;
    auxiliar = fases[i].partidos;   //octavos

    i++;
    nodoPartido* auxiliarCuartos;   //cuartos
    auxiliarCuartos = fases[i].partidos;

    i++;
    nodoPartido* auxiliarSemis;
    auxiliarSemis = fases[i].partidos;

    i++;
    nodoPartido* auxiliarFinal;
    auxiliarFinal = fases[i].partidos;

    printf("\n   OCTAVOS DE FINAL \t       CUARTOS DE FINAL  \t\t    SEMIFINAL \t\t           FINAL");

    printf("\n\n%14s: %i(%i)__\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1);
    printf("%14s: %i(%i)  |\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2);

    printf("                      |____");
    printf("%14s: %i(%i)______\n",auxiliarCuartos->partido.equipo1,auxiliarCuartos->partido.golesEq1,auxiliarCuartos->partido.penales1);
    auxiliar = auxiliar->siguiente;
    printf("                      |    %14s: %i(%i)      |\n",auxiliarCuartos->partido.equipo2,auxiliarCuartos->partido.golesEq2,auxiliarCuartos->partido.penales2);

    printf("%14s: %i(%i)__|                              |\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1);
    printf("%14s: %i(%i)                                 |______%14s: %i(%i)________\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2,auxiliarSemis->partido.equipo1,auxiliarSemis->partido.golesEq1,auxiliarSemis->partido.penales1);






    auxiliarCuartos = auxiliarCuartos->siguiente;
    auxiliar = auxiliar->siguiente;
    printf("%14s: %i(%i)__                               |      %14s: %i(%i)        |\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1,auxiliarSemis->partido.equipo2,auxiliarSemis->partido.golesEq2,auxiliarSemis->partido.penales2);
    printf("%14s: %i(%i)  |                              |                                  |\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2);
    printf("                      |____");

    printf("%14s: %i(%i)______|                                  |\n",auxiliarCuartos->partido.equipo1,auxiliarCuartos->partido.golesEq1,auxiliarCuartos->partido.penales1);

    auxiliarSemis = auxiliarSemis->siguiente;
    auxiliar = auxiliar->siguiente;
    printf("                      |    %14s: %i(%i)                                         |\n",auxiliarCuartos->partido.equipo2,auxiliarCuartos->partido.golesEq2,auxiliarCuartos->partido.penales2);
    printf("%14s: %i(%i)__|                                                                 |\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1);

    printf("%14s: %i(%i)                                                                    |\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2);
    printf("                                                                                        |%14s: %i(%i)\n",auxiliarFinal->partido.equipo1,auxiliarFinal->partido.golesEq1,auxiliarFinal->partido.penales1);
    printf("                                                                                        |------\n");
    printf("                                                                                        |%14s: %i(%i)",auxiliarFinal->partido.equipo2,auxiliarFinal->partido.golesEq2,auxiliarFinal->partido.penales2);



    auxiliarCuartos = auxiliarCuartos->siguiente;
    auxiliar = auxiliar->siguiente;

    printf("\n%14s: %i(%i)__                                                                  |\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1);
    printf("%14s: %i(%i)  |                                                                 |\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2);
    printf("                      |____");
    printf("%14s: %i(%i)______                                   | \n",auxiliarCuartos->partido.equipo1,auxiliarCuartos->partido.golesEq1,auxiliarCuartos->partido.penales1);
    auxiliar = auxiliar->siguiente;
    printf("                      |    %14s: %i(%i)      |                                  |\n",auxiliarCuartos->partido.equipo2,auxiliarCuartos->partido.golesEq2,auxiliarCuartos->partido.penales2);

    printf("%14s: %i(%i)__|                              |                                  |\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1);
    printf("%14s: %i(%i)                                 |______%14s: %i(%i)________|\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2,auxiliarSemis->partido.equipo1,auxiliarSemis->partido.golesEq1,auxiliarSemis->partido.penales1);

    auxiliarCuartos = auxiliarCuartos->siguiente;
    auxiliar = auxiliar->siguiente;
    printf("%14s: %i(%i)__                               |      %14s: %i(%i)\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1,auxiliarSemis->partido.equipo2,auxiliarSemis->partido.golesEq2,auxiliarSemis->partido.penales2);
    printf("%14s: %i(%i)  |                              |\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2);
    printf("                      |____");
    printf("%14s: %i(%i)______|\n",auxiliarCuartos->partido.equipo1,auxiliarCuartos->partido.golesEq1,auxiliarCuartos->partido.penales1);
    auxiliar = auxiliar->siguiente;
    printf("                      |    %14s: %i(%i)\n",auxiliarCuartos->partido.equipo2,auxiliarCuartos->partido.golesEq2,auxiliarCuartos->partido.penales2);

    printf("%14s: %i(%i)__|\n",auxiliar->partido.equipo1,auxiliar->partido.golesEq1,auxiliar->partido.penales1);
    printf("%14s: %i(%i)\n\n",auxiliar->partido.equipo2,auxiliar->partido.golesEq2,auxiliar->partido.penales2);
}

void print_partido(Partido *p)
{
    printf("%s\n",p->fecha);
    printf("%s %d - %d %s\n\n", p->equipo1->nomEquipo, p->golesEq1, p->golesEq2, p->equipo2->nomEquipo);
}

void clrscr()
{
    system("@cls||clear");
}

menu menus[6];

char nomMenu[4][40] =
{
    "Generador de partidos",
    "Generador de grupos manipulados",
    "Resultados finales generados",
    "Buscar partido"
};

void hidecursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void showcursor()
{ 
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void load_menus()
{
    printf("Cargando menu 0...\n");
    // carga de menu 0
    menus[0].cantOpciones = 3;
    strcpy(menus[0].opciones[0], "Generar partidos por azar");
    strcpy(menus[0].opciones[1], "Generar partidos manipulados");
    strcpy(menus[0].opciones[2], "Salir");
    // carga de menu 1
    menus[1].cantOpciones = 3;
    strcpy(menus[1].opciones[0], "Clasifica");
    strcpy(menus[1].opciones[1], "No clasifica");
    strcpy(menus[1].opciones[2], "Volver");
    menus[2].cantOpciones = 5;
    strcpy(menus[2].opciones[0], "Mostrar partidos de finales");
    strcpy(menus[2].opciones[1], "Mostrar tabla de posiciones");
    strcpy(menus[2].opciones[2], "Mostrar partidos de fase de grupos");
    strcpy(menus[2].opciones[3], "Buscar partidos");
    strcpy(menus[2].opciones[4], "Salir");
    menus[3].cantOpciones = 3;
    strcpy(menus[3].opciones[0], "Buscar partidos de un equipo");
    strcpy(menus[3].opciones[1], "Volver");
    printf("Menus cargados\n");
}


///FUNCIONES MAIN
void print_menu(menu m, int option)
{
    for (int i = 0; i < m.cantOpciones; i++) {
        if (i == option) {
            back_color(HOVER_COLOR);
            printf("%s", m.opciones[i]);
            back_color(DEFAULT_COLOR);
            printf("\n");
        } else {
            printf("%s", m.opciones[i]);
            printf("\n");
        }
    }
}

char charGrupos[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

void mostrarTablaGrupo(Grupo g)
{
    printf("----------------------------------------------------------------------------\n");
    printf("GRUPO %c\n", g.letra);
    printf("\t\tMP\tW\tD\tL\tGF\tGA\tGD\tPts\n");

    nodoGrupoEquipo* seg = g.equipos;

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
}

void mostrarTablasGrupos(Grupo grupos[8])
{
    bool go = true;
    int grupo = 0;
    while (go) {
        printf("Tablas de posiciones\n");
        mostrarTablaGrupo(grupos[grupo]); printf("\n");
        printf("<- Grupo anterior     Grupo siguiente ->\n");
        printf("ESC para volver");
        switch (getch()) {
            case LEFT_KEY:
                grupo--;
                if (grupo < 0) grupo = 7;
                break;
            case RIGHT_KEY:
                grupo++;
                if (grupo > 7) grupo = 0;
                break;
            case ESC_KEY:
                go = false;
                break;
        }
        clrscr();
    }
}

void mostrarPartidosGrupos(GrupoPartido partidos[8])
{
    bool go = true;
    int grupo = 0;
    while (go) {
        nodoPartido *aux = partidos[grupo].partidos;
        printf("Partidos de grupos\n");
        printf("----------------------------------------------------------------------------\n");
        printf("GRUPO %c\n", charGrupos[grupo]);
        while (aux != NULL) {
            print_partido(&aux->partido);
            aux = aux->siguiente;
        }
        printf("<- Grupo anterior     Grupo siguiente -->\n");
        printf("ESC para volver");
        switch (getch()) {
            case LEFT_KEY:
                grupo--;
                if (grupo < 0) grupo = 7;
                break;
            case RIGHT_KEY:
                grupo++;
                if (grupo > 7) grupo = 0;
                break;
            case ESC_KEY:
                go = false;
                break;
        }
        clrscr();
    }
}

int getGrupoDeEquipo(char* equipo, Grupo grupos[8])
{
    for (int i = 0; i < 8; i++) {
        nodoGrupoEquipo *aux = grupos[i].equipos;
        while (aux != NULL) {
            if (strcmp(aux->equipo->nomEquipo, equipo) == 0) {
                return i;
            }
            aux = aux->siguiente;
        }
    }
    return -1;
}

char* get_equipo(nodoEquipo *listaDeEquipos)
{
    bool flag = true;
    char *equipoElegido;
    printf("Ingrese el equipo: ");
    fflush(stdin);
    showcursor();
    gets(equipoElegido);
    hidecursor();
    flag = existeEquipo(equipoElegido, listaDeEquipos);
    while(!flag)
    {
        printf("El equipo no existe, vuelva a intentarlo: ");
        fflush(stdin);
        gets(equipoElegido);
        flag = existeEquipo(equipoElegido, listaDeEquipos);
    }
    return equipoElegido;
}

void printPartidosDeEquipo(Grupo grupos[8], GrupoPartido *partidosGrupo, fase fases[4], nodoEquipo *listaDeEquipos)
{
    char equipoElegido[30];
    printf("copiando equipo\n"); clrscr();
    strcpy(equipoElegido, get_equipo(listaDeEquipos));
    int grupo = getGrupoDeEquipo(equipoElegido, grupos);
    nodoPartido *aux = partidosGrupo[grupo].partidos;
    while (aux != NULL) {
        if (strcmpi(aux->partido.equipo1->nomEquipo, equipoElegido) == 0 || strcmpi(aux->partido.equipo2->nomEquipo, equipoElegido) == 0) {
            printf("Fase de grupos\n");
            print_partido(&aux->partido);
        }
        aux = aux->siguiente;
    }
    char nomFases[4][30] = {"Octavos de final", "Cuartos de final", "Semifinales", "Final"};
    for (int i = 0; i < 4; i++) {
        aux = fases[i].partidos;
        while (aux != NULL) {
            if (strcmpi(aux->partido.equipo1->nomEquipo, equipoElegido) == 0 || strcmpi(aux->partido.equipo2->nomEquipo, equipoElegido) == 0) {
                printf("%s\n", nomFases[i]);
                print_partido(&aux->partido);
            }
            aux = aux->siguiente;
        }
    }
}

void volver_button()
{
    back_color(HOVER_COLOR);
    printf("Volver");
    back_color(DEFAULT_COLOR);
    while (getch() != ENTER_KEY);
}

void print_title()
{
    printf("   _____ _                 _           _                  _                                  _ _       _ \n");
    printf("  / ____(_)               | |         | |                | |                                | (_)     | |\n");
    printf(" | (___  _ _ __ ___  _   _| | __ _  __| | ___  _ __    __| | ___   _ __ ___  _   _ _ __   __| |_  __ _| |\n");
    printf("  \\___ \\| | '_ ` _ \\| | | | |/ _` |/ _` |/ _ \\| '__|  / _` |/ _ \\ | '_ ` _ \\| | | | '_ \\ / _` | |/ _` | |\n");
    printf("  ____) | | | | | | | |_| | | (_| | (_| | (_) | |    | (_| |  __/ | | | | | | |_| | | | | (_| | | (_| | |\n");
    printf(" |_____/|_|_| |_| |_|\\__,_|_|\\__,_|\\__,_|\\___/|_|     \\__,_|\\___| |_| |_| |_|\\__,_|_| |_|\\__,_|_|\\__,_|_|\n");
    printf("\n");
}

void main_menu()
{

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

    // INICIO DE MENU
    load_menus();
    clrscr();
    bool go = true;
    int SelectedMenu = 0;
    int SelectedOption = 0;
    char equipoElegido[30];
    bool flag;
    while (go) {
        gotoxy(0, 0);
        menu m = menus[SelectedMenu];
        print_title();
        printf("%s", nomMenu[SelectedMenu]); printf("\n\n");
        if (SelectedMenu == 1) printf("Equipo seleccionado: %s\n", equipoElegido);
        print_menu(m, SelectedOption);
        int c = getch();
        switch (c) {
            case UP_KEY:
                if (SelectedOption > 0) {
                    SelectedOption--;
                }
                break;
            case DOWN_KEY:
                if (SelectedOption < m.cantOpciones - 1) {
                    SelectedOption++;
                }
                break;
            case ENTER_KEY:
                switch (SelectedMenu) {
                    case 0:
                        switch (SelectedOption) {
                            case 0: // generar fase de grupo al azar
                                clrscr();
                                crearArregloGrupoPartidosRandom(partidosGrupo, grupos, fechas);
                                organizarOctavos(fases, grupos, fechas);
                                organizarCuartos(fases, fechas);
                                organizarSemis(fases, fechas);
                                organizarFinal(fases, fechas);
                                SelectedOption = 0;
                                SelectedMenu = 2;
                                break;
                            case 1: // Generar fase de grupos manipulada
                                strcpy(equipoElegido, get_equipo(listaDeEquipos));
                                clrscr();
                                SelectedMenu = 1;
                                SelectedOption = 0;
                                break;
                            case 2: // salir
                                go = false;
                                break;
                        }
                        break;
                    case 1:
                        clrscr();
                        switch (SelectedOption) {
                            case 0:
                                crearArregloGrupoPartidosManipulado(partidosGrupo, grupos, equipoElegido, true, fechas);
                                organizarOctavos(fases, grupos, fechas);
                                organizarCuartos(fases, fechas);
                                organizarSemis(fases, fechas);
                                organizarFinal(fases, fechas);
                                SelectedMenu = 2;
                                break;
                            case 1:
                                crearArregloGrupoPartidosManipulado(partidosGrupo, grupos, equipoElegido, false, fechas);
                                organizarOctavos(fases, grupos, fechas);
                                organizarCuartos(fases, fechas);
                                organizarSemis(fases, fechas);
                                organizarFinal(fases, fechas);
                                SelectedMenu = 2;
                                break;
                            case 2:
                                SelectedMenu = 0;
                                break;
                        }
                        break;
                    case 2: // menu de finales
                        clrscr();
                        switch (SelectedOption) {
                            case 0: // muestra resultados
                                muestraFasesFinalesConResultados(fases, 0);
                                printf("\nESC para volver");
                                while (getch() != ESC_KEY);
                                clrscr();
                                break;
                            case 1: // muestra tabla de fase de grupos
                                clrscr();
                                mostrarTablasGrupos(grupos);
                                break;
                            case 2:
                                mostrarPartidosGrupos(partidosGrupo);
                                break;
                            case 3: // buscar partidos
                                SelectedOption = 0;
                                SelectedMenu = 3;
                                break;
                            case 4: // cerrar
                                go = false;
                                break;
                        }
                        break;
                    case 3:
                        clrscr();
                        switch (SelectedOption) {
                            case 0: // buscar por equipo
                                clrscr();
                                listaDeEquipos = NULL;
                                leerArchivo(&listaDeEquipos);
                                printPartidosDeEquipo(grupos, partidosGrupo, fases, listaDeEquipos);
                                volver_button();
                                clrscr();
                                break;
                            case 1: // volver
                                SelectedMenu = 2;
                                break;
                        }
                        break;
                }
                break;
            case ESC_KEY:
                go = false;
                break;
        }
    }
}

///MAIN
int main()
{
    srand(time(NULL));
    hidecursor();
    main_menu();
    return 0;
}
