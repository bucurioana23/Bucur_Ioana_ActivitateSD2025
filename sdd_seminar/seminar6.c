#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	Nod** vector; // vector de pointeri
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(Nod* lista) {
	while (lista)
	{
		afisareMasina(lista->info);
		lista = lista->next;
	}
}

void adaugaMasinaInLista(Nod** lista,Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;

	if ((*lista) == NULL)
	{
		(*lista) = nou;
	}
	else
	{
		Nod* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nou;
	}
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	ht.dim = dimensiune;

	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}

	return ht;
}

int calculeazaHash(unsigned char serie, int dimensiune) {
	int hash;
	if (dimensiune != 0)
	{
		hash = serie % dimensiune;
		return hash;
	}
	else
	{
		return 0;
	}
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int pozitie = calculeazaHash(masina.serie, hash.dim);
	if (pozitie < hash.dim)
	{
		if (hash.vector[pozitie] != NULL) //avem coliziune?
		{
			adaugaMasinaInLista(&(hash.vector[pozitie]), masina);
		}
		else
		{
			hash.vector[pozitie] = (Nod*)malloc(sizeof(Nod));
			hash.vector[pozitie]->info = masina;
			hash.vector[pozitie]->next = NULL;
		}
	}

	//SAU APELAM DOAR adaugaMasinaInLista pentru ca tratam si cazul in care lista e goala

}



HashTable citireMasiniDinFisier(const char* numeFisier, int dimensiune) {
	FILE* f = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(dimensiune);

	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		inserareMasinaInTabela(ht, m);
	}
	fclose(f);
	return ht;
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("\nSuntem in clusterul %d\n", i);
			afisareListaMasini(ht.vector[i]);
		}
		else
		{
			printf("\nIn clusterul %d nu avem masini!!\n", i);
		}
	}
}

void dezalocareListaDeMasini(Nod** lista)
{
	Nod* p = (*lista);
	while (p)
	{
		Nod* temp = p;
		p = p->next;
		if (temp->info.model != NULL)
		{
			free(temp->info.model);
		}

		if (temp->info.numeSofer != NULL)
		{
			free(temp->info.numeSofer);
		}

		free(temp);
	}
	(*lista) = NULL;
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareListaDeMasini(&(ht->vector[i]));
	}
	ht->dim = 0;
	free(ht->vector);
	ht->vector = NULL;
}

float pretMediuPerLista(Nod* lista)
{
	int suma = 0;
	int contor = 0;
	while (lista)
	{
		suma += lista->info.pret;
		contor++;
		lista = lista->next;
	}

	return (contor > 0 ? (suma / contor) : 0);
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	float* preturi = NULL;
	(*nrClustere) = 0;

	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			(*nrClustere)++;
		}
	}

	preturi = (float*)malloc(sizeof(float) * (*nrClustere));
	int contor = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			preturi[contor] = pretMediuPerLista(ht.vector[i]);
			contor++;
		}
	}
	return preturi;
}

Masina getMasinaDupaCheie(HashTable ht , char serie) {
	Masina m;
	m.id = -1;
	int pozitie = calculeazaHash(serie, ht.dim);
	if (ht.vector[pozitie])
	{
		Nod* p = ht.vector[pozitie];
		while (p && p->info.serie!=serie)		
		{
			p = p->next;
		}
		if (p)
		{
			m = p->info;
			m.numeSofer = malloc(strlen(p->info.numeSofer) + 1);
			strcpy(m.numeSofer, p->info.numeSofer);
			m.model = malloc(strlen(p->info.model) + 1);
			strcpy(m.model, p->info.model);
		}
	}
	return m;
}

int main() 
{
	HashTable ht = citireMasiniDinFisier("masini.txt", 10);
	afisareTabelaDeMasini(ht);
	Masina m = getMasinaDupaCheie(ht, 'A');
	printf("\nMasina cu seria A: \n");
	if(m.id != NULL)
	{
		afisareMasina(m);
	}
	else
	{
		printf("Nu a fost gasita masina!");
	}

	int nrClustere = 0;
	float* p = calculeazaPreturiMediiPerClustere(ht, &nrClustere);

	printf("Preturi medii per clustere: ");
	for (int i = 0; i < nrClustere; i++)
	{
		printf(" %.2f ", p[i]);
	}
	printf("\n");

	free(m.numeSofer);
	free(m.model);
	dezalocareTabelaDeMasini(&ht);
	return 0;
}