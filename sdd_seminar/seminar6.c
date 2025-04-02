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
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht /*valoarea pentru masina cautata*/) {
	Masina m;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	return m;
}

int main() 
{
	HashTable ht = citireMasiniDinFisier("masini.txt", 10);

	return 0;
}