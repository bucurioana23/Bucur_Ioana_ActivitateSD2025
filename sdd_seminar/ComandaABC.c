#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Comanda {
	unsigned int id;
	char* dataLansareComanda;
	char* dataLivrareProgramata;
	char* numeClient;
	float valoare;
};
typedef struct Comanda Comanda;

struct Nod {
	Comanda info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Comanda citireComandaDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Comanda c;
	fgets(buffer, 99, f);
	aux = strtok(buffer, sep);
	c.id = atoi(aux);
	aux = strtok(NULL, sep);
	c.dataLansareComanda = malloc(strlen(aux) + 1);
	strcpy(c.dataLansareComanda, aux);

	aux = strtok(NULL, sep);
	c.dataLivrareProgramata = malloc(strlen(aux) + 1);
	strcpy(c.dataLivrareProgramata, aux);

	aux = strtok(NULL, sep);
	c.numeClient = malloc(strlen(aux) + 1);
	strcpy(c.numeClient, aux);

	c.valoare = atof(strtok(NULL, sep));

	return c;
}

void afisareComanda(Comanda c) {
	printf("\nId comanda: %d\n", c.id);
	printf("Data lansare comanda: %s\n", c.dataLansareComanda);
	printf("Data livrare programata: %s\n", c.dataLivrareProgramata);
	printf("Nume client: %s\n", c.numeClient);
	printf("Valoare comanda: %.2f\n", c.valoare);
}

void adaugaComandaInArbore(Nod** arbore, Comanda c)
{
	if (*arbore == NULL)
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = c;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
	else {
		if ((*arbore)->info.id > c.id)
		{
			adaugaComandaInArbore(&((*arbore)->st), c);
		}
		else {
			adaugaComandaInArbore(&((*arbore)->dr), c);
		}
	}
}

Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Comanda c = citireComandaDinFisier(f);
		adaugaComandaInArbore(&arbore, c);
	}
	fclose(f);
	return arbore;
}

void afisareArborePost(Nod* arbore)
{
	if (arbore)
	{
		afisareArborePost(arbore->st);
		afisareArborePost(arbore->dr);
		afisareComanda(arbore->info);
	}
}

float getValoareDupaId(Nod* arbore, int id)
{
	if (arbore)
	{
		if (arbore->info.id > id)
		{
			return getValoareDupaId(arbore->st, id);
		}
		else if (arbore->info.id < id)
		{
			return getValoareDupaId(arbore->dr, id);
		}
		else if (arbore->info.id == id)
		{
			return arbore->info.valoare;
		}
	}
	else return 0;
}

float getValoareMedie(Nod* arbore, int id1, int id2, int id3)
{
	if (arbore)
	{
		return (getValoareDupaId(arbore, id1) + getValoareDupaId(arbore, id2) + getValoareDupaId(arbore, id3)) / 3;
	}
	else return 0;
}

int getNumarComenziDupaNume(Nod* arbore, const char* nume)
{
	if (arbore)
	{
		if (strcmp(arbore->info.numeClient, nume) == 0)
		{
			return 1 + getNumarComenziDupaNume(arbore->st, nume) + getNumarComenziDupaNume(arbore->dr, nume);
		}
		else
			return getNumarComenziDupaNume(arbore->st, nume) + getNumarComenziDupaNume(arbore->dr, nume);
	}
	else return 0;
}

void adaugaComenziInVector(Nod* arbore, const char* nume, Comanda* vector, int* index) {
	if (arbore)
	{
		if (strcmp(arbore->info.numeClient, nume) == 0) {
			vector[*index] = arbore->info;
			(*index)++;
		}
		adaugaComenziInVector(arbore->st, nume, vector, index);
		adaugaComenziInVector(arbore->dr, nume, vector, index);
	}
}

Comanda* vectorComenziDupaNume(Nod* arbore, const char* nume, int* dimensiuneVector)
{
	int numarCom = getNumarComenziDupaNume(arbore, nume);
	*dimensiuneVector = numarCom;

	if (numarCom == 0)
		return NULL;

	Comanda* vector = (Comanda*)malloc(sizeof(Comanda) * numarCom);
	int index = 0;
	adaugaComenziInVector(arbore, nume, vector, &index);

	return vector;
}

void dezalocareArbore(Nod** arbore)
{
	if (*arbore)
	{
		dezalocareArbore(&((*arbore)->st));
		dezalocareArbore(&((*arbore)->dr));
		if ((*arbore)->info.dataLansareComanda)
		{
			free((*arbore)->info.dataLansareComanda);
		}
		if ((*arbore)->info.dataLivrareProgramata)
		{
			free((*arbore)->info.dataLivrareProgramata);
		}
		if ((*arbore)->info.numeClient)
		{
			free((*arbore)->info.numeClient);
		}
		free(*arbore);
		(*arbore) = NULL;
	}
}

int main()
{
	Nod* arbore = citireArboreDinFisier("comenzi.txt");
	afisareArborePost(arbore);

	float valoare = getValoareMedie(arbore, 102, 107, 105);

	printf("\nValoare medie: %.2f\n\n", valoare);

	int dimensiune;
	Comanda* vector = vectorComenziDupaNume(arbore, "Ion Popescu", &dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		afisareComanda(vector[i]);
	}

	dezalocareArbore(&arbore);

	return 0;
}
