
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

///1-

nodoEquipo* crearNodoEquipo(Equipo equipo);                                               ///CREA UN NODO EQUIPO (PARA LA LISTA DE EQUIPOS GENERAL)

void insertarAlFinalEquipo(nodoEquipo**,Equipo aInsertar);                                ///INSERTA UN NODO EQUIPO A LA LISTA GENERAL DE EQUIPOS

void leerArchivo(nodoEquipo** listaDeEquipos);                                            ///LEE NUESTRO ARCHIVO CON LOS EQUIPOS Y PROBABILIDADES Y LO CARGA A LA LISTA GENERAL DE EQUIPOS

///2-

void inicializarGrupos(Grupo*, int);                                                      ///LE PONE LAS LETRAS(NOMBRE) A CADA GRUPO DEL ARREGLO DE GRUPOS E INICIALIZA LA LISTA DENTRO DEL ARREGLO

nodoGrupoEquipo* crearNodoGrupoEquipo(Equipo* equipo);                                    ///CREA UN NODO GRUPOEQUIPO (PARA LA LISTA DE EQUIPOS QUE ESTA EN CADA CELDA DEL ARREGLO DE GRUPOS)

void insertarEnElGrupo(Grupo*, int, Equipo*);                                             ///INSERTA EL NODO GRUPOEQUIPO CREADO EN LA LISTA DE CADA CELDA DEL ARREGLO)

void cargarGrupos(Grupo*, int, nodoEquipo*);                                              ///FUNCION DE LLAMADA EN EL MAIN QUE INVOCA LAS 3 FUNCIONES ANTERIORES

void mostrarGrupos(Grupo*, int);                                                          ///MUESTRA (DESPUES TENDRIAMOS QUE HACER UNA PARA MOSTRAR LAS POSICIONES EN ORDEN)

///3-
///4-

int existeEquipo(char*, nodoEquipo*);

nodoPartido* crearNodoPartido(Partido);                                                   ///CREA UN NODO CON UNA ESTRUCTURA PARTIDO Y PUNTERO AL SIGUIENTE

void insertarAlFinalNodoPartido(nodoPartido**,Partido);

Equipo* getEquipo(nodoGrupoEquipo*,int*);                                                 ///DEVUELVE UN EQUIPO DE UN GRUPO SEGUN SU POSICION EN LA SUBLISTA

void definirGolesVictoria(int*, int, int*, int);                                           ///EN CASO DE VICTORIA, ESTA FUNCION ELIGE UN RESULTADO CONSIDERANDO EL DESEMPEÑO DE LOS EQUIPOS

void resultadoDelEmpate(int*, int*);                                                      ///EN CASO DE EMPATE, ESTA FUNCION ELIGE UN RESULTADO

void agregarPartido(nodoPartido**, Equipo*, Equipo*);

void cargarPartidosGrupos(nodoPartido**, Grupo);

void crearArregloGrupoPartidosRandom(GrupoPartido*, Grupo*);

void crearArregloGrupoPartidosManipulado(GrupoPartido*, Grupo*, char*, int);


///IMPLEMENTACION DE FUNCIONES

///1-

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

///2-

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

///3-
///4-

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

Equipo* getEquipo(nodoGrupoEquipo* grupo, int* indexEquipo)
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
        if(numeroRandom <= 65)
        {
            *golesGanador = 1;
        }
        if(65 < numeroRandom && numeroRandom <= 85)
        {
            *golesGanador = 2;
        }
        if(85 < numeroRandom)
        {
            *golesGanador = 3;
        }

        numeroRandom = rand()%101;
        if(numeroRandom <= 75)
        {
            *golesPerdedor = 0;
        }
        if(75 < numeroRandom && numeroRandom <= 95)
        {
            *golesPerdedor = 1;
        }
        if(95 < numeroRandom)
        {
            *golesPerdedor = 3;
        }
    }

    if(10 < probabilidadGanador && probabilidadGanador <= 60)
    {
        numeroRandom = rand()%101;
        if(numeroRandom <= 40)
        {
            *golesGanador = 1;
        }
        if(40 < numeroRandom && numeroRandom <= 85)
        {
            *golesGanador = 2;
        }
        if(85 < numeroRandom && numeroRandom <= 92)
        {
            *golesGanador = 3;
        }
        if(92 < numeroRandom && numeroRandom <= 97)
        {
            *golesGanador = 4;
        }
        if(97 < numeroRandom)
        {
            *golesGanador = 5;
        }

        numeroRandom = rand()%101;
        if(numeroRandom <= 30)
        {
            *golesPerdedor = 0;
        }
        if(30 < numeroRandom && numeroRandom <= 60)
        {
            *golesPerdedor = 1;
        }
        if(60 < numeroRandom && numeroRandom <= 90)
        {
            *golesPerdedor = 2;
        }
        if(90 < numeroRandom)
        {
            *golesPerdedor = 3;
        }
    }

    if(probabilidadGanador > 60)
    {
        numeroRandom = rand()%101;
        if(numeroRandom <= 40)
        {
            *golesGanador = 1;
        }
        if(40 < numeroRandom && numeroRandom <= 70)
        {
            *golesGanador = 2;
        }
        if(70 < numeroRandom && numeroRandom <= 90)
        {
            *golesGanador = 3;
        }
        if(85 < numeroRandom && numeroRandom <= 95)
        {
            *golesGanador = 4;
        }
        if(95 < numeroRandom)
        {
            *golesGanador = 5;
        }

        numeroRandom = rand()%101;
        if(numeroRandom <= 30)
        {
            *golesPerdedor = 0;
        }
        if(30 < numeroRandom && numeroRandom <= 60)
        {
            *golesPerdedor = 1;
        }
        if(60 < numeroRandom && numeroRandom <= 90)
        {
            *golesPerdedor = 2;
        }
        if(90 < numeroRandom)
        {
            *golesPerdedor = 3;
        }
    }
}

void resultadoDelEmpate(int* goles1, int* goles2)
{
    int numeroRandom;
    numeroRandom = rand()%101;

    if(numeroRandom <= 30)
    {
         *goles1 = 0;
         *goles2 = 0;
    }
    if(30 < numeroRandom && numeroRandom <= 70)
    {
        *goles1 = 1;
        *goles2 = 1;
    }
    if(70 < numeroRandom && numeroRandom <= 95)
    {
        *goles1 = 2;
        *goles2 = 2;
    }
    if(95 < numeroRandom)
    {
        *goles1 = 3;
        *goles2 = 3;
    }
}

void agregarPartido(nodoPartido** lista, Equipo* eq1, Equipo* eq2)                                                                                  ///FALTA AGREGAR aInsertar.fecha
{
    Partido aInsertar;
    aInsertar.equipo1 = eq1;
    aInsertar.equipo2 = eq2;

    int probabilidadPrimero = eq1->probabilidad * 1000;
    int probabilidadSegundo = eq2->probabilidad * 1000;
    int probabilidadEmpate;
    int probabilidadTotal;
    int resultado;

    if(probabilidadPrimero >= probabilidadSegundo)
    {
        if(probabilidadPrimero > probabilidadSegundo * 12)
        {
            probabilidadSegundo = probabilidadPrimero / 12;
        }

        probabilidadEmpate = probabilidadSegundo * 1.33;
    }
    else
    {
        if(probabilidadSegundo > probabilidadPrimero * 12)
        {
            probabilidadPrimero = probabilidadSegundo / 12;
        }

        probabilidadEmpate = probabilidadPrimero * 1.33;
    }

    probabilidadTotal = probabilidadEmpate + probabilidadPrimero + probabilidadSegundo;
    ///printf("Probabiliad total = %i\n",probabilidadTotal);

    resultado = rand()%(probabilidadTotal+1);

    if (resultado <= probabilidadPrimero)   //gana el primero
    {
        do
        {
            definirGolesVictoria(&(aInsertar.golesEq1), probabilidadPrimero,&(aInsertar.golesEq2),probabilidadSegundo);
        }while(aInsertar.golesEq1 < aInsertar.golesEq2);

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
    else
    {
        if ((resultado > probabilidadPrimero) && (resultado <= probabilidadSegundo + probabilidadPrimero))      //gana el segundo
        {
            do
            {
                definirGolesVictoria(&(aInsertar.golesEq2),probabilidadSegundo,&(aInsertar.golesEq1),probabilidadPrimero);
            }while(aInsertar.golesEq2 < aInsertar.golesEq1);

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

        if (resultado > probabilidadSegundo + probabilidadPrimero)      //empate
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
    }
    insertarAlFinalNodoPartido(lista,aInsertar);
}

void cargarPartidosGrupos(nodoPartido** lista, Grupo grupo)
{
    /*printf("%s vs %s\n", getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 1));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 2), getEquipo(grupo.equipos, 3));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 2));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 1));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 1), getEquipo(grupo.equipos, 2));
    printf("%s vs %s\n", getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 0));*/
    agregarPartido(lista, getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 1));
    agregarPartido(lista, getEquipo(grupo.equipos, 2), getEquipo(grupo.equipos, 3));
    agregarPartido(lista, getEquipo(grupo.equipos, 0), getEquipo(grupo.equipos, 2));
    agregarPartido(lista, getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 1));
    agregarPartido(lista, getEquipo(grupo.equipos, 1), getEquipo(grupo.equipos, 2));
    agregarPartido(lista, getEquipo(grupo.equipos, 3), getEquipo(grupo.equipos, 0));
}

void crearArregloGrupoPartidosRandom(GrupoPartido* partidosGrupo, Grupo* grupos)
{
    for (int i = 0; i < VALIDOS_GRUPO; i++)
    {
        partidosGrupo[i].letra = grupos[i].letra;
        partidosGrupo[i].partidos = NULL;
        cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i]);
    }
}

void insertar_equipo_ordenado(nodoGrupoEquipo **list, Equipo *e) {
    if (*list == NULL || (*list)->equipo->pts <= e->pts) {
        if ((*list)->equipo->pts == e->pts) { // si empatan en puntos
            if (((*list)->equipo->gf - (*list)->equipo->ga) <= (e->gf - e->ga)) {
                if (((*list)->equipo->gf - (*list)->equipo->ga) == (e->gf - e->ga)) {
                    if ((*list)->equipo->gf < e->gf) {
                        nodoGrupoEquipo *aux = crearNodoGrupoEquipo(e);
                        aux->siguiente = *list;
                        *list = aux;
                    } else insertar_equipo_ordenado(&(*list)->siguiente, e);
                } else {
                    nodoGrupoEquipo *aux = crearNodoGrupoEquipo(e);
                    aux->siguiente = *list;
                    *list = aux;
                }
            } else insertar_equipo_ordenado(&(*list)->siguiente, e);
        } else {
            nodoGrupoEquipo *aux = crearNodoGrupoEquipo(e);
            aux->siguiente = *list;
            *list = aux;
        }
    } else insertar_equipo_ordenado(&(*list)->siguiente, e);
}

void ordenar_grupo_por_puntos(nodoGrupoEquipo **list) {
    nodoGrupoEquipo *aux = *list;
    nodoGrupoEquipo *temp;
    while (aux != NULL) { // copio la lista
        temp = aux;
        temp = temp->siguiente;
        aux = aux->siguiente;
    }
    while (*list != NULL) {
        aux = *list;
        *list = (*list)->siguiente;
        free(aux);
    }
    *list = NULL;
    while (aux != NULL) {
        insertar_equipo_ordenado(list, aux->equipo);
        aux = aux->siguiente;
    }
}

void crearArregloGrupoPartidosManipulado(GrupoPartido partidosGrupo[VALIDOS_GRUPO], Grupo grupos[VALIDOS_GRUPO], char* equipoElegido, int opcion)   ////////PSEUDOCODIGO----FALTA HACER
{
    int flag;

    for (int i = 0; i < VALIDOS_GRUPO; i++)
    {
        if('EL EQUIPO DESEADO NO ESTA EN ESTE GRUPO')       /// HACEMOS RANDOM NORMAL
        {
            partidosGrupo[i].letra = grupos[i].letra;
            partidosGrupo[i].partidos = NULL;
            cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i]);
        }
        else
        {
            do
            {
                cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i]);
                flag = ('ES EL RESULTADO DESEADO?');  ///   0 = NO         1 = SI
            }while(flag == 0);
        }
    }
}

///MAIN

int main()
{
    srand(time(NULL));
    printf("SIMULADOR DEL MUNDIAL QATAR 2022:\n\n");
    int opcion;
    int opcion2;
    int flag;
    char equipoElegido[20];
    Grupo grupos[VALIDOS_GRUPO];
    nodoEquipo* listaDeEquipos = NULL;

    ///1-
    leerArchivo(&listaDeEquipos);

    ///2-
    inicializarGrupos(grupos, VALIDOS_GRUPO);
    cargarGrupos(grupos, VALIDOS_GRUPO, listaDeEquipos);
    mostrarGrupos(grupos, VALIDOS_GRUPO);

    ///3-
    ///4-
    GrupoPartido partidosGrupo[VALIDOS_GRUPO];

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
        crearArregloGrupoPartidosRandom(partidosGrupo, grupos);
        printf("\n");
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
            scanf("%i",&opcion2);
        }
        crearArregloGrupoPartidosManipulado(partidosGrupo, grupos, equipoElegido, opcion2);  ///OPCION ELEGIR RESULTADO (1= clasifica) (2= no clasifica)
    }
    return 0;
}
