
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

Equipo* getEquipo(nodoGrupoEquipo*,int*);                                                 ///DEVUELVE UN EQUIPO DE UN GRUPO SEGUN SU POSICION EN LA SUBLISTA

void agregarPartido(nodoPartido**, Equipo*, Equipo*);

void cargarPartidosGrupos(nodoPartido**, Grupo);

void crearArregloGrupoPartidosRandom(GrupoPartido*, Grupo*);

void crearArregloGrupoPartidosManipulado(partidosGrupo, grupos, equipoElegido, opcion2);


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
            printf("%i\t", seg->equipo->mp);                                                                       ///JUGADOS
            printf("%i\t", seg->equipo->win);                                                                      ///GANADOS
            printf("%i\t", seg->equipo->mp - grupos[i].equipos->equipo->win - grupos[i].equipos->equipo->loss);    ///EMPATADOS
            printf("%i\t", seg->equipo->loss);                                                                     ///PERDIDOS
            printf("%i\t", seg->equipo->gf);                                                                       ///GOLES A FAVOR
            printf("%i\t", seg->equipo->ga);                                                                       ///GOLES EN CONTRA
            printf("%i\t", seg->equipo->gf - grupos[i].equipos->equipo->ga);                                       ///DIFERENCIA DE GOL
            printf("%i\n", seg->equipo->pts);                                                                      ///PUNTOS
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

nodoPartido* crearNodoPartido(Partido insertar)
{
    nodoPartido* nuevo = (nodoPartido*)malloc(sizeof(nodoPartido));
    nuevo->partido = insertar;
    nuevo->siguiente = NULL;

    return nuevo;
}

Equipo* getEquipo(nodoGrupoEquipo* grupo, int* indexEquipo)
{
    for (int i = 0; i < indexEquipo; i++)
    {
        grupo = grupo->siguiente;
    }

    return grupo->equipo;
}

void agregarPartido(nodoPartido** lista, Equipo* eq1, Equipo* eq2)
{
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

    resultado = rand() % (probabilidadTotal+1);

    if (resultado <= probabilidadPrimero)   //gana el primero
    {
    //terminar




    }
    else
    {
        if (resultado > probabilidadPrimero && resultado <= probabilidadSegundo + probabilidadPrimero)      //gana el segundo
        {
                //terminar
        }
        if (resultado > probabilidadSegundo + probabilidadPrimero)      //empate
        {
                //terminar
        }
    }
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

void crearArregloGrupoPartidosRandom(GrupoPartido partidosGrupo[VALIDOS_GRUPO], Grupo grupos[VALIDOS_GRUPO])
{
    for (int i = 0; i < VALIDOS_GRUPO; i++)
    {
        partidosGrupo[i].letra = grupos[i].letra;
        partidosGrupo[i].partidos = NULL;
        cargarPartidosGrupos(&(partidosGrupo->partidos), grupos[i]);
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
    case 1:
        crearArregloGrupoPartidosRandom(partidosGrupo, grupos);                             /// OPCION TODO RANDOM
        break;

    case 2:
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

        //crearArregloGrupoPartidosManipulado(partidosGrupo, grupos, equipoElegido, opcion2)  ///OPCION ELEGIR RESULTADO (1= clasifica) (2= no clasifica)
    }
    return 0;
}
