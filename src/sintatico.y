%output "sintatico.tab.c"
%defines "sintatico.tab.h"

%{
#define YYMAXDEPTH 1000000000000000000
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.yy.h"
#include "Alocador.h"
#include "Grafo.h"
#include "Lista.h"
void adicionaVertices(Grafo graph,int vertex,Lista list);
void startAloc(Grafo graph);
extern char* yytext;
int yyerror(char *s);
extern int yylex();
int print_teste(int test);
int vertex;
int registers;
int id;
int n;
int lgc=0;
Grafo graph;
Lista list;

%}

%union{
  int int_t;
}

%token END_LINE
%token END_FILE
%token GRAPH
%token <int_t> NUM
%token COLON
%token ASSIGN
%token K
%token EDGE
%token MOVE


%start inicio

%type <int_t> K
%type <int_t> registradores
%type <int_t> GRAPH
%type <int_t> programa

%%

  inicio: END_FILE {exit(0);}
        | programa inicio {}
  ;

  programa: graph registradores programa2 { startAloc(graph); destroiGrafo(graph);}
  ;

  programa2: declaracoes programa2 {  }
           |  {}
  ;

  graph: GRAPH NUM COLON END_LINE{id = $2; }
  ;

  registradores: K ASSIGN NUM END_LINE{registers = $3; graph = criaGrafo(registers,id);}
  ;

  declaracoes: NUM EDGE declaracoes2 {vertex = $1; addVertice(graph,vertex); adicionaVertices(graph,vertex,list); liberaLista(list); }
             | NUM MOVE move {}
  ;

  declaracoes2: NUM declaracoes2  {int *n = (int *)malloc(sizeof(int)); *n=$1; insert(list,n);}
              | NUM END_LINE {int *n = (int *)malloc(sizeof(int));*n=$1;list=createList(); insert(list,n); }
              | NUM END_FILE {int *n = (int *)malloc(sizeof(int));*n=$1;list=createList(); insert(list,n); }
  ;

  move: NUM move2{}
  ;

  move2:  NUM MOVE NUM move2 {}
       |  END_LINE {}

%%

void startAloc(Grafo graph){
  int *graphStatus;

  graphStatus = (int*) malloc(sizeof(int) * getKGrafo(graph));
  if(lgc==1) printf("\n");
  printf("Graph %d -> Physical Registers: %d\n", getIdGrafo(graph), getKGrafo(graph));
  printf("----------------------------------------\n");
  printf("----------------------------------------\n");



  // n = getKGrafo(graph);
  for(int n = getKGrafo(graph) ; n > 1 ; n--){
    printf("K = %d\n\n", n);
    // imprimeGrafo(graph);
    if(alocar(graph, n)) graphStatus[n] = 1;

    printf("----------------------------------------\n");

    resetaGrausGrafo(graph);
  }

  printf("----------------------------------------");
  if(getKGrafo(graph) > 0) printf("\n");

  for(int n = getKGrafo(graph) ; n > 1 ; n--){
    if( n > 9 )
      printf("Graph %d -> K = %d", getIdGrafo(graph), n);
    else
      printf("Graph %d -> K =  %d", getIdGrafo(graph), n);
    if(graphStatus[n])
      printf(": Successful Allocation");
    else 
      printf(": SPILL");
    if(n != 2){
      printf("\n");
    }
  }
    
  lgc=1; 
}

void adicionaVertices(Grafo graph,int vertex,Lista lista){
  Posic item;
  item = getLast(lista);
  int *n;
  while (item != NULL) {
    n = (int *)get(lista,item);
    addVertice(graph,*n);
    addAresta(graph,vertex,*n);
    item = getPrevious(lista,item);
  }
}

int yyerror(char *s){
  printf("Teve algum erro %s",s);
}

int main(int argc, char **argv){

  yyparse();

  return 0;
}
