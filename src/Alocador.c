//
// Created by guilherme on 11/17/18.
//

#include "Alocador.h"


#define TRUE 1
#define FALSE 0

int alocar(Grafo grafo, int n){
	int cond;
	Pilha pilha;
	Lista teste;
	cond = 0;

	// teste = getVertices(grafo);
	// printf("CHEGOU  EM ALOCAR, GRAFO: %d num. de vertices: %d\n", getIdGrafo(grafo), length(teste));

	// imprimeGrafo(grafo);
	pilha = simplify(grafo, n);
	// printf("TAMANHO DA PILHA NO ALOCAR: %d\n", getSizePilha(pilha));

	cond = assign(grafo, pilha, n);
	if(cond == -1)
		return 0;
	return 1;
}

Vertice potencialSpill(Grafo grafo, int n);

Pilha simplify(Grafo gra, int n){
	int i, removeu, k, ps;
	Pilha pilha;
	Vertice vertice, verticePS, verticeRemover;
	Lista vertices;
	Posic item;
	pilha = createPilha();
	k = getKGrafo(gra);

	vertices = getVertices(gra);

	removeu = TRUE;
	while(removeu){
		verticeRemover = NULL;
		removeu = FALSE;
		item = getFirst(vertices);
		while(item != NULL){
			vertice = get(vertices,item);
			if(getGrauVertice(gra, vertice) < n && verificaVerticeValido(gra,vertice) && getIdVertice(gra,vertice) > k){
				if(verticeRemover == NULL)
					verticeRemover = vertice;
				// Remove o vertice de menor grau
				else if(getGrauVertice(gra,vertice) < getGrauVertice(gra,verticeRemover))
					verticeRemover = vertice;
				// Caso graus sejam iguais, pega o de menor id
				else if (getGrauVertice(gra, vertice) == getGrauVertice(gra, verticeRemover)){
					if (getIdVertice(gra,vertice) < getIdVertice(gra,verticeRemover))
						verticeRemover = vertice;
				}

				removeu = TRUE;
			}
			item = getNext(vertices,item);
		}
		// printf("Removeu: %d, existeVerticeValido: %d\n", removeu, existeVerticeValido(gra));
		if(removeu){
			printf("Push: %d\n", getIdVertice(gra, verticeRemover));
			removeVertice(gra, verticeRemover);
			push(pilha,verticeRemover);
		}
		else if(!removeu && existeVerticeValido(gra)){
			verticePS = potencialSpill(gra, n);
			removeVertice(gra, verticePS);
			push(pilha, verticePS);
			printf("Push: %d *\n", getIdVertice(gra, verticePS));
			removeu = TRUE;
		}
	}

	// printf("TAMANHO DA PILHA NO SIMPLIFY: %d\n", getSizePilha(pilha));
	return pilha;
}



Vertice potencialSpill(Grafo gra, int n){
	int i, maior, k;
	Vertice vertice, aux, bla;
	Lista vList;
	Posic item;
	// vertice = buscaVertice(gra,0);
	// maior = getGrauVertice(gra,vertice);
	vertice = NULL;
	maior = 0;
	
	k = getKGrafo(gra);

	vList = getVertices(gra);
	item = getFirst(vList);
	while(item != NULL){
		aux = get(vList,item);
		if(getIdVertice(gra,aux) > k && verificaVerticeValido(gra,aux) && getGrauVertice(gra,aux) > maior){
			maior = getGrauVertice(gra,aux);
			vertice = aux;
		}
		else if (getIdVertice(gra,aux) > k && verificaVerticeValido(gra,aux) && getGrauVertice(gra,aux) == maior){
			if(getIdVertice(gra, aux) < getIdVertice(gra, vertice)){
				maior = getGrauVertice(gra,aux);
				vertice = aux;
			}
		}
		item = getNext(vList,item);
	}

	return vertice;
}

int assign(Grafo grafo, Pilha pilha, int n){
	Posic item;
	int k, color, x;
	Vertice v;
	k = getKGrafo(grafo);

	// printf("TAMANHO DA PILHA NO ASSIGN: %d\n", getSizePilha(pilha));
	item = pop(pilha);

	while(item != NULL){
		v = item;
		addVertice(grafo, getIdVertice(grafo,v));
		color = verificaCoresVertice(grafo, v, n);
		// printf("COLOR: %d\n", color);
		if(color == -1){
			printf("Pop: %d -> NO COLOR AVAILABLE\n", getIdVertice(grafo, v));
			return -1;
		}
		else{
			pintaVertice(grafo, v, color);
			printf("Pop: %d -> %d\n", getIdVertice(grafo, v), color);
		}
		item = pop(pilha);
	}

	return 1;
}
