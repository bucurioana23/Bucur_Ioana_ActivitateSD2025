#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

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

//creare structura arbore binar de cautare
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
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
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


void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
	if (!(*arbore))
	{
		Nod* radacina = (Nod*)malloc(sizeof(Nod));
		radacina->info = masinaNoua;
		radacina->dr = NULL;
		radacina->st = NULL;
		(*arbore) = radacina;
	}
	else 
	{
		if ((*arbore)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
		}
		else if ((*arbore)->info.id < masinaNoua.id) 
		{
			adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
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

void afisareInOrdine(Nod* arbore) //SRD - INORDINE
{
	if (arbore)
	{
		afisareInOrdine(arbore->st); // S
		afisareMasina(arbore->info); // R
		afisareInOrdine(arbore->dr); // D
	}
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

void afisarePostOrdine(Nod* arbore) //SRD - INORDINE
{
	if (arbore)
	{
		afisarePreOrdine(arbore->st); // S
		afisarePreOrdine(arbore->dr); // D
		afisareMasina(arbore->info); // R
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if ((*arbore))
	{
		dezalocareArboreDeMasini(&((*arbore)->st));
		dezalocareArboreDeMasini(&((*arbore)->dr));
		if ((*arbore)->info.numeSofer != NULL)
		{
			free((*arbore)->info.numeSofer);
		}
		if ((*arbore)->info.model != NULL)
		{
			free((*arbore)->info.model);
		}
		free(*arbore);
		(*arbore) = NULL;
	}
}

Masina getMasinaByID(Nod* arbore, int id) 
{
	Masina m;
	m.id = -1;
	if (arbore)
	{
		if (arbore->info.id < id)
		{
			return getMasinaByID(arbore->dr, id);
		}
		else if (arbore->info.id > id)
		{
			return getMasinaByID(arbore->st, id);
		}
		else
		{
			return arbore->info;
		}
	}

	return m;
}

int determinaNumarNoduri(/*arborele de masini*/) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	return 0;
}

int calculeazaInaltimeArbore(/*arbore de masini*/) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	return 0;
}

void calculeazaPretTotal1(Nod* arbore, float* suma) {

	if (arbore)
	{
		calculeazaPretTotal1(arbore->st, suma);
		calculeazaPretTotal1(arbore->dr, suma);
		(*suma) += arbore->info.pret;
	}
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore == NULL)
		return 0;

	float totalStanga = calculeazaPretTotal(arbore->st);
	float totalDreapta = calculeazaPretTotal(arbore->dr);
	return arbore->info.pret + totalStanga + totalDreapta;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main() {
	Nod* radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisarePreOrdine(radacina);

	Masina m = getMasinaByID(radacina, 7);
	printf("\nMasina cautata: \n");
	afisareMasina(m);

	float suma1 = 0;
	calculeazaPretTotal1(radacina,&suma1);
	printf("\nSuma 1 totala preturi: %.2f",suma1);

	float suma = calculeazaPretTotal(radacina);
	printf("\nSuma totala preturi: %.2f", suma);

	dezalocareArboreDeMasini(&radacina);
	return 0;
}