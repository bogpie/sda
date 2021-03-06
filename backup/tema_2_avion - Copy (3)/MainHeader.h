#pragma once

#ifndef MAINHEADER

#define MAINHEADER
#define _CRT_SECURE_NO_WARNINGS

#include "UtilHeader.h"

#include "GraphHeader.h"
#include "MinHeapHeader.h"
#include "DijkstraHeader.h"
#include "TrieHeader.h"

typedef struct Resource Resource;
struct Resource
{
	char* name;
	int quantity;
};

typedef struct Island Island;
struct Island
{
	char* name;
	int nrResources;
	Resource* vResources;
	int nrPlanes;
	int* vPlanes;
	int tolerance;
};

typedef struct BackParam BackParam;
struct BackParam
{
	Island* vIslands;
	int nrIslands;
	GraphMat* graphMat;
	int idIsland;
	int excess;
	int* vLevel;
	int level;
	int* vAlready;
	Heap** vHeap;
};

void fReadIslands(FILE* input, int* adrNrIslands, Island** adrVectorIslands);
void fReadConnections(FILE* input, int nrIslands, GraphMat** adrGraphMat);
void fSolveConexiune(FILE* input, FILE* output, GraphMat* graphMat);
void fSolveLegatura(FILE* input, FILE* output, GraphMat* graphMat);
void fSolveAnulareZbor(FILE* input, GraphMat* graphMat);
void fSolveAdaugaZbor(FILE* input, GraphMat* graphMat);
void fSolveMaxResurse(FILE* input, FILE* output, int nrIslands, Island* vIslands);
void fSolveMaxCantitate(FILE* input, FILE* output, int nrIslands, Island* vIslands);
void fSolveDrumZbor(FILE* input, FILE* output, GraphMat* graphMat);
void fSolveTimpZbor(FILE* input, FILE* output, GraphMat* graphMat);
void fSolveMinZbor(FILE* input, FILE* output, GraphMat* graphMat);
void fNameToIndex(char* name, int* adrIndex);
void fBack(BackParam backParam);
void fTryBack(FILE* output, GraphMat* graphMat, Island* vIslands, Heap** vHeap, int* adrChainTransfer);
void fChainTransfer(GraphMat* graphMat,Island* vIslands, int nrIslands, FILE* output);
void fReadIslandsMat(FILE* input, GraphMat* graphMat);
void fSolvePart1(FILE* input, FILE* output, int* adrNrIslands);
void fSolvePart2(FILE* input, FILE* output, int nrIslands);

#endif