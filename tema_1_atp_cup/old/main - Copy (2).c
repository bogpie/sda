#define _CRT_SECURE_NO_WARNINGS
#define NAMELENGTH 50
//#pragma warning( push )
//#pragma warning( disable : 6054 )

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Player Player;
struct Player
{
	char* lastName;
	char* firstName;
	int score;
};

typedef struct Country Country;
struct Country
{
	char* name;
	int nrPlayers;
	int globalScore;
	Player* vPlayers;
};

typedef struct Node Node;
struct Node
{
	Node* prev;
	Country country;
	Node* next;
};

void fAdd(Node* start, Node* node, Node* prev)
{
	prev->next = node;
	node->next = NULL;
}

void fParseDate(FILE* dateIn,int * adrNrCountries,Node * start)
{	
	char countryName[NAMELENGTH];
	int score;

	int nrCountries;
	fscanf(dateIn, "%d", &nrCountries);
	*adrNrCountries = nrCountries;

	//vCountries = malloc(sizeof(Country) * nrCountries);

	Node* prev = start;
	start->next = start;
	start->prev = start;

	for (int iCountry = 0; iCountry < nrCountries; ++iCountry)
	{
		Country country;
		fscanf(dateIn, "%s" , country.name);
		fscanf(dateIn, "%d", &country.nrPlayers);
		country.vPlayers = malloc(sizeof(Player) * country.nrPlayers);
		for (int iPlayer = 0; iPlayer < country.nrPlayers; ++iPlayer)
		{
			Player player;
			char firstName[NAMELENGTH], lastName[NAMELENGTH];
			fscanf(dateIn, "%s%s%d", firstName, lastName, player.score);
			player.firstName = malloc(sizeof(char) * strlen(firstName));
			player.lastName = malloc(sizeof(char) * strlen(lastName));
			strcpy(player.firstName, firstName);
			strcpy(player.lastName, lastName);
			country.vPlayers[iPlayer] = player;
		}
		Node* node = malloc(sizeof(Country));
		node->country = country;
		
		fAdd(start,node,prev);
		if (iCountry == nrCountries)
		{
			node->next = start;
			start->prev = node;
		}
	}

}

void fParseCerinte(FILE * cerinteIn,int vCerinte[5],Player vTwoPlayers[2])
{	
	//cerinteIn = fopen("cerinte.in", "r");
	for (int i = 0; i < 5; ++i)
	{
		fscanf(cerinteIn, "%d", &vCerinte[i]);
	}

	if (vCerinte[4] == 1)
	{
		char firstName[NAMELENGTH], lastName[NAMELENGTH];
		int score;

		for (int i = 0; i < 2; ++i)
		{
			fscanf(cerinteIn, "%s%s%d", firstName, lastName, &vTwoPlayers[i].score);
			vTwoPlayers[i].firstName = malloc(strlen(firstName) * sizeof(char));
			vTwoPlayers[i].lastName = malloc(strlen(lastName) * sizeof(char));
			strcpy(vTwoPlayers[i].firstName, firstName);
			strcpy(vTwoPlayers[i].lastName, lastName);
		}
	}
}

int main()
{
	FILE* cerinteOut = fopen("date.out", "w");
	FILE* rezultateOut = fopen("rezultate.out", "w");
	
	FILE* dateIn = fopen("date.in", "r");
	
	
	//Country* vCountries;
	
	Node* start = malloc(sizeof(Node));
	start->next = NULL;

	int nrCountries;
	fParseDate(dateIn,&nrCountries,start);

	FILE* cerinteIn = fopen("cerinte.in", "r");
	int vCerinte[5];
	Player vTwoPlayers[2]; // se memoreaza?
	fParseCerinte(cerinteIn,vCerinte,vTwoPlayers);

	return 0;
}
//#pragma warning( pop )