#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL)
	{
		return 0;
	}
	else {
		int inaltimeSt = calculeazaInaltimeArbore(arbore->st);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);
		if (inaltimeSt > inaltimeDr)
			return 1 + inaltimeSt;
		else return 1 + inaltimeDr;
	}
}

int calculeazaNumarNoduri(Nod* arbore)
{
	if (arbore)
	{
		return 1 + calculeazaNumarNoduri(arbore->st) + calculeazaNumarNoduri(arbore->dr);
	}
	else return 0;
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.
void rotireStanga(Nod** arbore)
{
	Nod* aux = (*arbore)->dr; // golim in dreapta, si apoi umplem
	(*arbore)->dr = aux->st;
	aux->st = (*arbore);
	(*arbore) = aux;

}

void rotireDreapta(Nod** arbore) //dezechilibru in stanga
{
	Nod* aux = (*arbore)->st;
	(*arbore)->st = aux->dr;
	aux->dr = (*arbore);
	(*arbore) = aux;
}

int gradEchilibru(Nod* radacina) {
	if (radacina != NULL) {
		return(calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr));
	}
	else {
		return 0;
	}
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina masinaNoua) {
	if ((*arbore) == NULL)
	{
		Nod* nou = malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->dr = NULL;
		nou->st = NULL;
		(*arbore) = nou;
	}
	else
	{
		if ((*arbore)->info.id <= masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dr), masinaNoua);
		}
		else //if ((*arbore)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->st), masinaNoua);
		}

		int grad = gradEchilibru((*arbore));

		if (grad == 2)
		{
			//avem dezechilibru in stanga
			if (gradEchilibru((*arbore)->st) == 1) //varianta simpla, au aceleasi semne, rotire la dreapta
			{
				rotireDreapta(arbore);
			}
			else //GE = -1 -> SITUATIE COMPLEXA
			{
				rotireStanga(&((*arbore)->st));
				rotireDreapta(arbore);
			}
		}
		else if (grad == -2) // dezechilibru in dreapta, rotire la stanga
		{
			if (gradEchilibru((*arbore)->dr) != -1)
			{
				rotireDreapta(&((*arbore)->dr));
			}
			rotireStanga(arbore);
		}
	}
}

void* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisarePreOrdine(Nod* arbore) //SRD - INORDINE
{
	if (arbore)
	{
		afisareMasina(arbore->info); // R
		afisarePreOrdine(arbore->st); // S
		afisarePreOrdine(arbore->dr); // D
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&((*arbore)->st));
		dezalocareArboreDeMasini(&((*arbore)->dr));
		if ((*arbore)->info.numeSofer) {
			free((*arbore)->info.numeSofer);
		}
		if ((*arbore)->info.model) {
			free((*arbore)->info.model);
		}
		free(*arbore);
		(*arbore) = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(/*arborele de masini*/int id);

int determinaNumarNoduri(/*arborele de masini*/);

float calculeazaPretTotal(/*arbore de masini*/);

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer);

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreOrdine(arbore);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}