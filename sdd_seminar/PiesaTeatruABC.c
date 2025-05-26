#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct PiesaTeatru {
	char* titlu;
	int durataInMinute;
	int anPremiera;
	float popularitate;
};
typedef struct PiesaTeatru Piesa;

struct Nod {
	Piesa info;
	struct Nod* dr;
	struct Nod* st;

};
typedef struct Nod Nod;

Piesa citirePiesaDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	Piesa p;
	fgets(buffer, 100, f);
	char* aux;

	aux = strtok(buffer, sep);
	p.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy(p.titlu, aux);

	p.durataInMinute = atoi(strtok(NULL, sep));
	p.anPremiera = atoi(strtok(NULL, sep));
	p.popularitate = atof(strtok(NULL, sep));

	return p;
}

void afisarePiesa(Piesa p)
{
	printf("\nTitlu: %s\n", p.titlu);
	printf("Durata in minute: %d\n", p.durataInMinute);
	printf("An premiera: %d\n", p.anPremiera);
	printf("Popularitate: %.2f\n", p.popularitate);
}

void adaugaPiesaInArbore(Nod** arbore, Piesa p)
{
	if (*arbore) {
		if (strcmp((*arbore)->info.titlu, p.titlu) == 1) {
			adaugaPiesaInArbore(&((*arbore)->st), p);
		}
		else if (strcmp((*arbore)->info.titlu, p.titlu) == -1) {
			adaugaPiesaInArbore(&((*arbore)->dr), p);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = p;
		nou->dr = NULL;
		nou->st = NULL;
		(*arbore) = nou;
	}
}

Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f))
	{
		Piesa p = citirePiesaDinFisier(f);
		adaugaPiesaInArbore(&arbore, p);
	}
	fclose(f);
	return arbore;
}

void afisareInOrdine(Nod* arbore) //InOrdine SRD
{
	if (arbore)
	{
		afisareInOrdine(arbore->st);
		afisarePiesa(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void afisarePreOrdine(Nod* arbore) //PreOrdine RSD
{
	if (arbore)
	{
		afisarePiesa(arbore->info);
		afisarePreOrdine(arbore->st);
		afisarePreOrdine(arbore->dr);
	}
}

void afisarePostOrdine(Nod* arbore) //POSTOrdine sdr
{
	if (arbore)
	{
		afisarePostOrdine(arbore->st);
		afisarePostOrdine(arbore->dr);
		afisarePiesa(arbore->info);
	}
}

void dezalocareArbore(Nod** arbore)
{
	if (*arbore)
	{
		dezalocareArbore(&((*arbore)->st));
		dezalocareArbore(&((*arbore)->dr));
		if ((*arbore)->info.titlu) {
			free((*arbore)->info.titlu);
		}
		free(*arbore);
		(*arbore) = NULL;
	}
}

Piesa getPiesaCuCeaMaiMarePopularitate(Nod* arbore)
{
	if (arbore == NULL)
	{
		return (Piesa){NULL,-1,-1,-1.0 };
	}

	Piesa pStang = getPiesaCuCeaMaiMarePopularitate(arbore->st);
	Piesa pDrept = getPiesaCuCeaMaiMarePopularitate(arbore->dr);
	Piesa pMax = arbore->info;

	if (pMax.popularitate < pStang.popularitate)
	{
		pMax = pStang;
	}

	if (pMax.popularitate < pDrept.popularitate)
	{
		pMax = pDrept;
	}

	Piesa pDeepCopy = pMax;
	pDeepCopy.titlu = (char*)malloc(strlen(pMax.titlu) + 1);
	strcpy(pDeepCopy.titlu, pMax.titlu);

	return pDeepCopy;
}

Piesa getPiesaDupaTitlu(Nod* arbore, const char* titlu)
{
	if (arbore)
	{
		if (strcmp(arbore->info.titlu, titlu) == 1)
		{
			return getPiesaDupaTitlu(arbore->st, titlu);
		}
		else if (strcmp(arbore->info.titlu, titlu) == -1)
		{
			return getPiesaDupaTitlu(arbore->dr, titlu);
		}
		else {
			Piesa p = arbore->info;
			p.titlu = (char*)malloc(strlen(arbore->info.titlu) + 1);
			strcpy(p.titlu, arbore->info.titlu);
			return p;
		}
	}
	else {
		return (Piesa) { NULL, -1, -1, -1.0 };
	}
}

int calculeazaInaltimeArbore(Nod* arbore)
{
	if (arbore)
	{
		int inaltimeST = calculeazaInaltimeArbore(arbore->st);
		int inaltimeDr = calculeazaInaltimeArbore(arbore->dr);

		if (inaltimeST > inaltimeDr)
		{
			return 1 + inaltimeST;
		}
		else return 1 + inaltimeDr;
	}
	else return 0;
}

int calculeazaNumarNoduri(Nod* arbore)
{
	if (arbore == NULL)
		return 0;

	return 1 + calculeazaNumarNoduri(arbore->st) + calculeazaNumarNoduri(arbore->dr);
}

int main()
{
	Nod* arbore = citireArboreDinFisier("pieseTeatru.txt");
	afisareInOrdine(arbore);
	Piesa p = getPiesaCuCeaMaiMarePopularitate(arbore);
	printf("\n\nPiesa cu cea mia mare pop: \n");
	afisarePiesa(p);

	Piesa p2 = getPiesaDupaTitlu(arbore, "Furtuna");
	printf("\n\nPiesa dupa titlu: \n");
	afisarePiesa(p2);

	int inaltime = calculeazaInaltimeArbore(arbore);
	printf("\nInaltime; %d\n", inaltime);

	int numarNoduri = calculeazaNumarNoduri(arbore);
	printf("\nNumar noduri : %d\n", numarNoduri);

	dezalocareArbore(&arbore);
	free(p.titlu);
	free(p2.titlu);
	return 0;
}