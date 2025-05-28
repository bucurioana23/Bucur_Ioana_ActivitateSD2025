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

//creare structura pentru un nod dintr-un arbore binar de cautare
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
		if (inaltimeSt > inaltimeDr) {
			return 1 + inaltimeSt;
		}
		else {
			return 1 + inaltimeDr;
		}
	}
}

//ALTE FUNCTII NECESARE:
// - aici veti adauga noile functii de care aveti nevoie.

void rotireStanga(Nod** arbore)
{
	Nod* aux = (*arbore)->dr;
	(*arbore)->dr = aux->st;
	aux->st = (*arbore);
	(*arbore) = aux;
}

void rotireDreapta(Nod** arbore)
{
	Nod* aux = (*arbore)->st;
	(*arbore)->st = aux->dr;
	aux->dr = (*arbore);
	(*arbore) = aux;
}

int gradEchilibru(Nod* arbore) {
	if (arbore) {
		return calculeazaInaltimeArbore(arbore->st) - calculeazaInaltimeArbore(arbore->dr);
	}
	else {
		return 0;
	}
}

void adaugaMasinaInArboreEchilibrat(Nod** arbore, Masina masinaNoua) {
	if (*arbore == NULL)
	{
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->st = NULL;
		nou->dr = NULL;
		(*arbore) = nou;
	}
	else {
		if ((*arbore)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArboreEchilibrat(&((*arbore)->st), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*arbore)->dr), masinaNoua);
		}

		int grad = gradEchilibru(*arbore);

		if (grad == 2) //dezechilibru stanga
		{
			if (gradEchilibru((*arbore)->st) == -1)
			{
				rotireStanga(&((*arbore)->st));
			}
			rotireDreapta(arbore);
		}
		else if (grad == -2) // dezechilibru dreapta
		{
			if (gradEchilibru((*arbore)->dr) == 1)
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
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArboreEchilibrat(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareMasiniDinArbore(/*arbore de masini*/) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
}

void afisareInOrdine(Nod* arbore) //srd
{
	if(arbore)
	{
		afisareInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void afisarePreOrdine(Nod* arbore) //rsd
{
	if (arbore)
	{
		afisareMasina(arbore->info);
		afisarePreOrdine(arbore->st);
		afisarePreOrdine(arbore->dr);
	}
}

void afisarePostOrdine(Nod* arbore) //sdr
{
	if (arbore)
	{
		afisarePostOrdine(arbore->st);
		afisarePostOrdine(arbore->dr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore)
	{
		dezalocareArboreDeMasini(&((*arbore)->st));
		dezalocareArboreDeMasini(&((*arbore)->dr));
		if ((*arbore)->info.model)
		{
			free((*arbore)->info.model);
		}
		if ((*arbore)->info.numeSofer)
		{
			free((*arbore)->info.numeSofer);
		}
		free(*arbore);
		(*arbore) = NULL;
	}
}

//Preluati urmatoarele functii din laboratorul precedent.
//Acestea ar trebuie sa functioneze pe noul arbore echilibrat.

Masina getMasinaByID(Nod* arbore, int id)
{
	if (arbore) {
		if (arbore->info.id > id) {
			return getMasinaByID(arbore->st,id);
		}
		else if (arbore->info.id < id) {
			return getMasinaByID(arbore->dr,id);
		}
		else {
			return arbore->info;
		}
	}
	else {
		return (Masina) { -1, -1, -1, NULL, NULL, '-' };
	}
}

int determinaNumarNoduri(Nod* arbore) {
	if (arbore == NULL)
		return 0;

	return 1 + determinaNumarNoduri(arbore->st) + determinaNumarNoduri(arbore->dr);
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore == NULL)
		return 0;

	float totalStanga = calculeazaPretTotal(arbore->st);
	float totalDreapta = calculeazaPretTotal(arbore->dr);

	return arbore->info.pret + totalStanga + totalDreapta;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if (arbore == NULL) {
		return 0;
	}

	float suma = 0;

	if (strcmp(arbore->info.numeSofer, numeSofer) == 0)
	{
		suma += arbore->info.pret;
	}

	return suma + calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer) + calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
}

int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisarePreOrdine(arbore);

	Masina m = getMasinaByID(arbore, 7);
	printf("\nMasina cautata: \n");
	afisareMasina(m);

	float pret = calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu");
	printf("Pret masini Ionescu: %.2f\n\n", pret);

	int numarNoduri = determinaNumarNoduri(arbore);
	printf("Numar de noduri din arbore: %d\n\n", numarNoduri);

	float suma = calculeazaPretTotal(arbore);
	printf("\nSuma totala preturi: %.2f", suma);

	dezalocareArboreDeMasini(&arbore);
	return 0;
}