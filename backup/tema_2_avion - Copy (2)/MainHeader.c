#include "MainHeader.h"

// parametrii comune functiilor sunt fisierul de input si/sau cel de output

void fSolvePart1(FILE* input, FILE* output,int* adrNrIslands)
{
	//numarul de insule transmis ca referinta pentru a putea fi identificat in partea a doua 
	Island* vIslands = NULL;
	fReadIslands(input, adrNrIslands, &vIslands);			//citim insulele, resursele, etc
	GraphMat* graphMat = NULL;
	fReadConnections(input, *adrNrIslands, &graphMat);			//citim conexiunile; facem un graf pe baza lor
	while (1)
	{
		char query[NAMELENGTH];
		query[0] = '\0';
		fscanf(input, "%s", query);
		if (query[0] == '\0' || feof(input))
		{
			*adrNrIslands = -1;
			break; // testele 1->8, nu se va continua cu un nou numar de insule
		}
		else if (query[0] >= '0' && query[0] <= '9')
		{
			// daca la citirea cerintelor am gasit un numar, este chiar numarul de insule pentru partea a doua
			*adrNrIslands = atoi(query); 
			break;
		}
		else if (!strcmp(query, "conexiune"))			// se afla ce functie sa se execute in  functie de numele cerintei
		{
			fSolveConexiune(input, output, graphMat);
		}
		else if (!strcmp(query, "legatura"))
		{
			fSolveLegatura(input, output, graphMat);
		}
		else if (!strcmp(query, "adauga_zbor"))
		{
			fSolveAdaugaZbor(input, graphMat);
		}
		else if (!strcmp(query, "anulare_zbor"))
		{
			fSolveAnulareZbor(input, graphMat);
		}
		else if (!strcmp(query, "max_resurse"))
		{
			fSolveMaxResurse(input, output, *adrNrIslands, vIslands);
		}
		else if (!strcmp(query, "max_cantitate"))
		{
			fSolveMaxCantitate(input, output, *adrNrIslands, vIslands);
		}
		else if (!strcmp(query, "drum_zbor"))
		{
			fSolveDrumZbor(input, output, graphMat);
		}
		else if (!strcmp(query, "timp_zbor"))
		{
			fSolveTimpZbor(input, output, graphMat);
		}
		else if (!strcmp(query, "min_zbor"))
		{
			fSolveMinZbor(input, output, graphMat);
		}
		else
		{
			exit(1); // ar fi greseala la parsare
		}
	}
	if (vIslands != NULL)		//  se elibereaza memoria din vectorul de insule
	{
		for (int idIsland = 1; idIsland <= graphMat->nrVertexes; ++idIsland)
		{
			if (vIslands[idIsland].vResources != NULL)
			{
				free(vIslands[idIsland].vResources);
				vIslands[idIsland].vResources = NULL;
			}
			if (vIslands[idIsland].name != NULL)
			{
				free(vIslands[idIsland].name);
				vIslands[idIsland].name = NULL;
			}
		}
		free(vIslands);
		vIslands = NULL;
	}
	fEraseGraphMat(graphMat);		// se sterge graful
	graphMat = NULL;

}

void fReadIslands(FILE* input, int* adrNrIslands, Island** adrVectorIslands)
{
	int nrIslands = 0;
	fscanf(input, "%d", &nrIslands);
	*adrNrIslands = nrIslands;
	Island* vIslands = malloc(sizeof(Island) * (nrIslands + 1)); //indexarea insulelor in input e de la 1
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		vIslands[idIsland].vResources = NULL;
		vIslands[idIsland].vPlanes = NULL;
		vIslands[idIsland].vResources = NULL;
	}
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		char vChar[8]; // pentru islandXY a insulei XY
		Island island;
		fscanf(input, "%s", vChar);
		fStrAlloc(&island.name, vChar);
		fscanf(input, "%d", &island.nrResources);
		island.vResources = malloc(sizeof(Resource) * island.nrResources);
		for (int idResource = 0; idResource < island.nrResources; ++idResource)
		{
			Resource resource;
			fscanf(input, "%s", vChar);
			fStrAlloc(&resource.name, vChar);
			fscanf(input, "%d", &resource.quantity);
			island.vResources[idResource] = resource;

		}
		vIslands[idIsland] = island;
	}
	*adrNrIslands = nrIslands;
	*adrVectorIslands = vIslands;
}

void fReadConnections(FILE* input, int nrIslands, GraphMat** adrGraphMat)
{
	GraphMat* graphMat = *adrGraphMat;
	int nrEdges;
	fscanf(input, "%d", &nrEdges);
	if (graphMat != NULL)
	{
		free(graphMat);
	}
	fInitGraphMat(&graphMat, nrIslands, nrEdges);
	fCreateGraphMat(graphMat, input);
	*adrGraphMat = graphMat;
}

void fSolveConexiune(FILE* input, FILE* output, GraphMat* graphMat)
{
	char leftName[NAMELENGTH], rightName[NAMELENGTH];
	fscanf(input, "%s", leftName);
	fscanf(input, "%s", rightName);
	int idLeft, idRight;
	fNameToIndex(leftName, &idLeft);
	fNameToIndex(rightName, &idRight);
	if (fTestEdgeMat(graphMat, idLeft, idRight))
	{
		fprintf(output, "OK\n");
	}
	else
	{
		fprintf(output, "NO\n");
	}
}

void fSolveLegatura(FILE* input, FILE* output, GraphMat* graphMat)
{
	char islandName[NAMELENGTH];
	fscanf(input, "%s", islandName);
	int islandIndex = islandName[strlen(islandName) - 1] - '0';
	fFindNeighbourMat(output, graphMat, islandIndex);
}

void fSolveMaxResurse(FILE* input, FILE* output, int nrIslands, Island* vIslands)
{
	TrieNode* trieRoot;
	fInitTrieNode(&trieRoot);

	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		Island island = vIslands[idIsland];
		for (int idResource = 0; idResource < island.nrResources; ++idResource)
		{
			Resource resource = island.vResources[idResource];
			fInsertInTrie(resource.name, trieRoot);
		}
	}
	char* word = malloc(NAMELENGTH);
	int nrResources = 0;
	fCountTrie(trieRoot, 0, &nrResources);
	fprintf(output, "%d ", nrResources);
	fPrintTrie(output, trieRoot, word, 0);
	fprintf(output, "\n");
	fEraseTree(trieRoot);
}

void fSolveAdaugaZbor(FILE* input, GraphMat* graphMat) //move to graphheader??
{
	char leftName[NAMELENGTH], rightName[NAMELENGTH];
	int cost;
	fscanf(input, "%s%s%d", leftName, rightName, &cost);
	int idLeft, idRight;
	fNameToIndex(leftName, &idLeft);
	fNameToIndex(rightName, &idRight);
	graphMat->mat[idLeft][idRight] = graphMat->mat[idRight][idLeft] = cost;
	graphMat->nrEdges++;
}

void fSolveAnulareZbor(FILE* input, GraphMat* graphMat)
{
	char leftName[NAMELENGTH], rightName[NAMELENGTH];
	int cost;
	fscanf(input, "%s%s%d", leftName, rightName, &cost);
	int idLeft, idRight;
	fNameToIndex(leftName, &idLeft);
	fNameToIndex(rightName, &idRight);
	graphMat->mat[idLeft][idRight] = graphMat->mat[idRight][idLeft] = 0;
}

void fSolveMaxCantitate(FILE* input, FILE* output, int nrIslands, Island* vIslands)
{
	char searched[NAMELENGTH];
	int maxim = -1;
	fscanf(input, "%s", searched);
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		Island island = vIslands[idIsland];

		int idResource;
		for (idResource = 0; idResource < island.nrResources; ++idResource)
		{
			Resource resource = island.vResources[idResource];
			if (!strcmp(resource.name, searched))
			{
				if (resource.quantity > maxim)
				{
					maxim = resource.quantity;
				}
			}
		}
	}
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		Island island = vIslands[idIsland];

		int idResource;
		for (idResource = 0; idResource < island.nrResources; ++idResource)
		{
			Resource resource = island.vResources[idResource];
			if (!strcmp(resource.name, searched))
			{
				if (resource.quantity == maxim)
				{
					fprintf(output, "%s ", island.name);
					break;
				}
			}
		}
	}
	fprintf(output, "\n");
}

void fSolveDrumZbor(FILE* input, FILE* output, GraphMat* graphMat) /// lipeste !!
{
	char leftName[NAMELENGTH], rightName[NAMELENGTH];
	fscanf(input, "%s%s", leftName, rightName);
	int idLeft, idRight;
	fNameToIndex(leftName, &idLeft);
	fNameToIndex(rightName, &idRight);
	int time = INT_MAX;
	int* vPrev = malloc(sizeof(int) * (graphMat->nrVertexes + 1));
	fDijkstra(graphMat, idLeft, idRight, &time, vPrev, 0);
	//printf("\nde la nodul sursa %d la nodurile:", source);
	if (time != INT_MAX)
	{
		fPrintPath(output, vPrev, idLeft, idRight);
	}
	else
	{
		fprintf(output, "NO");
	}
	fprintf(output, "\n");
}

void fSolveTimpZbor(FILE* input, FILE* output, GraphMat* graphMat)
{
	char leftName[NAMELENGTH], rightName[NAMELENGTH];
	fscanf(input, "%s%s", leftName, rightName);
	int idLeft, idRight;
	fNameToIndex(leftName, &idLeft);
	fNameToIndex(rightName, &idRight);
	int time = INT_MAX;
	int* vPrev = malloc(sizeof(int) * (graphMat->nrVertexes + 1));
	fDijkstra(graphMat, idLeft, idRight, &time, vPrev, 0);

	if (time == INT_MAX)
	{
		fprintf(output, "INF\n");

	}
	else
	{
		fprintf(output, "%d\n", time);
	}
}

void fSolveMinZbor(FILE* input, FILE* output, GraphMat* graphMat)
{
	char leftName[NAMELENGTH], rightName[NAMELENGTH];
	fscanf(input, "%s%s", leftName, rightName);
	int idLeft, idRight;
	fNameToIndex(leftName, &idLeft);
	fNameToIndex(rightName, &idRight);
	int time;
	int* vPrev = malloc(sizeof(int) * (graphMat->nrVertexes + 1));
	fDijkstra(graphMat, idLeft, idRight, &time, vPrev, 15);
	fprintf(output, "%d\n", time);
}

void fNameToIndex(char* name, int* adrIndex)
{
	int firstDigit = name[strlen(name) - 2] - '0';
	int secondDigit = name[strlen(name) - 1] - '0';
	*adrIndex = firstDigit * (firstDigit > 0 && firstDigit < 9) * 10 + secondDigit;
}

void fBack(BackParam backParam)
{
	int idIsland = backParam.idIsland;
	int nrIslands = backParam.nrIslands;
	Island* vIslands = backParam.vIslands;
	int tolerance = vIslands[idIsland].tolerance;

	if (backParam.level == backParam.excess)
	{
		int* vCode = calloc(nrIslands + 1, sizeof(int));
		int* vAdd = calloc(nrIslands + 1, sizeof(int));
		for (int idLevel = 0; idLevel < backParam.level; ++idLevel)
		{
			int idIsland = backParam.vLevel[idLevel];
			++vAdd[idIsland];
			vCode[idIsland] = vCode[idIsland] * 10 + (idLevel + 1);
		}
		int boolSkip = 0;
		for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
		{
			if (vIslands[idIsland].nrPlanes + vAdd[idIsland] > tolerance)
			{
				boolSkip = 1;
				break;
			}
		}
		if (boolSkip == 0)
		{
			for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
			{
				int foundPosition, time;
				fFindInHeap(backParam.vHeap[idIsland], vCode[idIsland], &foundPosition, &time);
				if (foundPosition != -1) continue;
				HeapNode* heapNode = malloc(sizeof(HeapNode));
				heapNode->dist = heapNode->index = vCode[idIsland];
				fInsertInHeap(backParam.vHeap[idIsland], heapNode);
			}
		}
		if (vCode != NULL) free(vCode);
		if (vAdd != NULL) free(vAdd);
		return;
	}
	else for (int idNeighbour = 1; idNeighbour <= nrIslands; ++idNeighbour)
	{

		if
			(
				idNeighbour == idIsland ||
				vIslands[idNeighbour].nrPlanes >= tolerance
				)
		{
			continue;
		}
		if (!fTestEdgeMat(backParam.graphMat, idIsland, idNeighbour))
		{
			continue;
		}

		if (backParam.vAlready[idNeighbour] == 1 && backParam.vLevel[backParam.level - 1] != idNeighbour)
		{
			continue;
			// daca deja i s-au atribuit avioane si nu se continua atribuirea
		}
		int oldValue = backParam.vLevel[backParam.level];
		backParam.vLevel[backParam.level++] = idNeighbour;
		int oldAlready = backParam.vAlready[idNeighbour];
		backParam.vAlready[idNeighbour] = 1;
		fBack(backParam);
		backParam.vLevel[--backParam.level] = oldValue;
		backParam.vAlready[idNeighbour] = oldAlready;
	}

}

void fChainTransfer(GraphMat* graphMat, Island* vIslands, int nrIslands, FILE* output)
{
	int chainTransfer = 1;
	int tolerance = vIslands[1].tolerance;
	int q[20];
	int* inQueue = NULL;
	while (chainTransfer)
	{
		chainTransfer = 0;
		if (inQueue != NULL) free(inQueue);
		inQueue = calloc(nrIslands + 1, sizeof(int));
		int p = 0, u = 0;
		int excess;

		for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
		{
			if (vIslands[idIsland].nrPlanes > tolerance)
			{
				q[p = u = 0] = idIsland;
				inQueue[idIsland] = 1;
				excess = vIslands[idIsland].nrPlanes - tolerance;
				chainTransfer = 1;
				break;
			}
		}

		if (!chainTransfer)
		{
			if (inQueue != NULL) free(inQueue);
			break;
		}

		while (p <= u)
		{
			int idIsland = q[p];
			Island island = vIslands[idIsland];
			for (int idNeighbour = 1; idNeighbour <= nrIslands; ++idNeighbour)
			{
				if (idNeighbour == idIsland || inQueue[idNeighbour] == 1 || !fTestEdgeMat(graphMat, idIsland, idNeighbour))
				{
					continue;
				}
				Island neighbour = vIslands[idNeighbour];
				int stop = vIslands[idIsland].nrPlanes - excess - 1;
				if (neighbour.nrPlanes + excess <= tolerance)
				{
					for (int idPlane = island.nrPlanes - 1; idPlane > stop; --idPlane)
					{
						vIslands[idNeighbour].vPlanes[vIslands[idNeighbour].nrPlanes++] = island.vPlanes[idPlane];
						vIslands[idIsland].vPlanes[--vIslands[idIsland].nrPlanes] = 0;
					}
					p = u + 1; // ca sa reia coada de la 0
					break;
				}
				else
				{
					q[++u] = idNeighbour;
					inQueue[idNeighbour] = 1;
				}

			}
			++p;
		}
	}
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		fCountSort(vIslands[idIsland].vPlanes, vIslands[idIsland].nrPlanes);
	}
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		fprintf(output, "Island%d\n", idIsland);
		for (int idPlane = 0; idPlane < vIslands[idIsland].nrPlanes; ++idPlane)
		{
			fprintf(output, "%d ", vIslands[idIsland].vPlanes[idPlane]);
		}
		fprintf(output, "\n");
	}

}

void fTryBack(FILE* output, GraphMat* graphMat, Island* vIslands, Heap** vHeap, int* adrChainTransfer)
{
	int tolerance = vIslands[1].tolerance;
	int nrIslands = graphMat->nrVertexes;
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		Island island = vIslands[idIsland];
		if (island.nrPlanes > tolerance)
		{
			int excess = island.nrPlanes - tolerance;
			int* vExcess = malloc(sizeof(int) * (excess + 1));
			int idExcess = 0;
			for (int idPlane = island.nrPlanes - excess; idPlane < island.nrPlanes; ++idPlane)
			{
				vExcess[idExcess] = island.vPlanes[idPlane]; // vExcess[1] = 22
				//should sort !!
				++idExcess;
			}

			int boolBreak = 1;
			for (int idNeighbour = 1; idNeighbour <= nrIslands; ++idNeighbour)
			{
				if (idNeighbour == idIsland || !fTestEdgeMat(graphMat, idIsland, idNeighbour)) continue;
				if (vIslands[idNeighbour].nrPlanes < tolerance)
				{
					boolBreak = 0;
					break;
				}
			}
			if (boolBreak)
			{
				*adrChainTransfer = 1;
				break;
			}
			BackParam backParam;
			backParam.excess = excess;
			backParam.graphMat = graphMat;
			backParam.idIsland = idIsland;
			backParam.nrIslands = nrIslands;
			backParam.vIslands = vIslands;
			backParam.vHeap = vHeap;
			backParam.level = 0;
			int* vLevel = calloc(excess + 1, sizeof(int));
			backParam.vLevel = vLevel;
			int* vAlready = calloc(nrIslands + 1, sizeof(int));
			backParam.vAlready = vAlready;
			backParam.graphMat = graphMat;
			vIslands[idIsland].nrPlanes = tolerance;
			fBack(backParam);
			vHeap = backParam.vHeap;
			for (int idIsland2 = 1; idIsland2 <= nrIslands; ++idIsland2)
			{
				if (vHeap[idIsland2]->size <= 1) continue;
				int size = vHeap[idIsland2]->size;
				fHeapSort(vHeap[idIsland2]);
				vHeap[idIsland2]->size = size; // sortarea inseamna mereu "stergerea" minimului => decrementare continua de heap->size => reinitializam
			}
			const int vPowers[] = { 1, 10, 100, 1000, 10000 };
			for (int idIsland2 = 1; idIsland2 <= nrIslands; ++idIsland2)
			{
				fprintf(output, "Island%d\n", idIsland2);
				for (int idSolution = 0; idSolution < vHeap[idIsland2]->size; ++idSolution)
				{
					int solution = vHeap[idIsland2]->arr[idSolution]->dist;
					int solutionBak = solution;
					int nrDigits = 0;

					while (solution)
					{
						solution /= 10;
						++nrDigits;
					}

					solution = solutionBak;

					for (int idPlane = 0; idPlane < vIslands[idIsland2].nrPlanes; ++idPlane)
					{
						fprintf(output, "%d ", vIslands[idIsland2].vPlanes[idPlane]);
					}

					if (!solution)
					{
						fprintf(output, "\n");
						break;
					}

					for (int idDigit = 0; idDigit < nrDigits; ++idDigit)
					{
						int digit = (solution / vPowers[nrDigits - idDigit - 1]) % 10;
						if (digit == 0)
						{
							continue;
						}
						else
						{
							fprintf(output, "%d ", vExcess[digit - 1]);
						}
					}
					fprintf(output, "\n");

				}
			}
			if (vExcess != NULL) free(vExcess);
			if (vLevel != NULL) free(vLevel);
			if (vAlready != NULL) free(vAlready);
		}
	}
}

void fSolvePart2(FILE* input,FILE* output,int nrIslands)
{
	Island* vIslands = malloc(sizeof(Island) * (nrIslands + 1));
	int tolerance;
	int totalPlanes = 0;
	short boolContinue = 0;
	fscanf(input, "%d", &tolerance);
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		fscanf(input, "%d", &vIslands[idIsland].nrPlanes);
		vIslands[idIsland].tolerance = tolerance;
		int nrPlanes = vIslands[idIsland].nrPlanes;
		if (nrPlanes > tolerance)
		{
			boolContinue = 1;
		}
		totalPlanes += nrPlanes;
	}
	if (totalPlanes > tolerance * nrIslands)
	{
		fprintf(output, "Stack overflow!");
		return;
		//nrIslands = 0;
	}
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		vIslands[idIsland].vPlanes = malloc(sizeof(int) * fMax(vIslands[idIsland].nrPlanes, tolerance));
		for (int idPlane = 0; idPlane < vIslands[idIsland].nrPlanes; ++idPlane)
		{
			fscanf(input, "%d", &vIslands[idIsland].vPlanes[idPlane]);
		}
		vIslands[idIsland] = vIslands[idIsland];
	}
	if (!boolContinue)
	{
		for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
		{
			fprintf(output, "Island%d\n", idIsland);
			for (int idPlane = 0; idPlane < vIslands[idIsland].nrPlanes; ++idPlane)
			{
				fprintf(output, "%d ", vIslands[idIsland].vPlanes[idPlane]);
			}
			fprintf(output, "\n");
		}
		//nrIslands = 0;
		return;
	}
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		fCountSort(vIslands[idIsland].vPlanes, vIslands[idIsland].nrPlanes);
	}
	Heap** vHeap = malloc(sizeof(Heap*) * (nrIslands + 1));
	for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
	{
		fInitHeap(&vHeap[idIsland], 30);
	}
	GraphMat* graphMat = NULL;
	fInitGraphMat(&graphMat, nrIslands, 0);		 // function
	fReadIslandsMat(input, graphMat);

	int chainTransfer = 0;
	fTryBack(output, graphMat, vIslands, vHeap, &chainTransfer);
	if (chainTransfer)			// transfer in lant
	{
		fChainTransfer(graphMat, vIslands, nrIslands, output);
	}
	fEraseGraphMat(graphMat);
	if (vHeap)
	{
		for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
		{
			if (vHeap[idIsland])
			{
				fEraseHeap(vHeap[idIsland]);
			}

		}
		free(vHeap);
	}
	if (vIslands != NULL)
	{
		for (int idIsland = 1; idIsland <= nrIslands; ++idIsland)
		{
			if (vIslands[idIsland].vPlanes != NULL)
			{
				free(vIslands[idIsland].vPlanes);
			}
		}
		free(vIslands);
	}
}