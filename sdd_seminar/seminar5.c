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
typedef struct Nod Nod;

struct Nod
{
	Masina info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla
{
	Nod* cap;
	Nod* coada;
};
typedef struct ListaDubla LD;

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

void afisareListaMasiniDeLaInceput(LD lista) {
	Nod* p = lista.cap;
	while (p)
	{
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniDeLaFinal(LD lista)
{
	Nod* p = lista.coada;
	while (p)
	{
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(LD* lista, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->coada;
	if(lista->coada != NULL)
	{
		lista->coada->next = nou;
	}
	else
	{
		lista->cap = nou;
	}
	lista->coada = nou;
}

void adaugaLaInceputInLista(LD* lista, Masina masinaNoua) {
	Nod* nou = malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->next = lista->cap;
	nou->prev = NULL;
	if (lista->cap)
	{
		lista->cap->prev = nou;
	}
	else
	{
		lista->coada = nou;
	}
	lista->cap = nou;
}

LD citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.cap = NULL;
	lista.coada = NULL;
	while (!feof(f))
	{
		Masina m;
		m = citireMasinaDinFisier(f);
		adaugaLaInceputInLista(&lista, m);
	}
	fclose(f);
	return lista;
}

void dezalocareLDMasini(LD* lista) {
	while (lista->cap)
	{
		if (lista->cap->info.model)
		{
			free(lista->cap->info.model);
		}
		if (lista->cap->info.numeSofer)
		{
			free(lista->cap->info.numeSofer);
		}
		Nod* p = lista->cap;
		lista->cap = p->next;
		free(p);
	}
	lista->coada = NULL;
}

float calculeazaPretMediu(LD lista) {
	float suma = 0;
	int contor = 0;
	Nod* temp = lista.cap;
	while (temp)
	{
		suma += temp->info.pret;
		contor++;
		temp = temp->next;
	}
	return (contor>0) ? suma/contor : 0;
}

void stergeMasinaDupaID(LD* lista, int id) //id unic la masini
{
	Nod* p = lista->cap;

	while (p && p->info.id != id) {
		p = p->next;
	}
	if(p){
		if (p->prev){
			p->prev->next = p->next;
			if (p->next){
				p->next->prev = p->prev;
			}
			else{
				lista->coada = p->prev;
			}
		}
		else {
			lista->cap = p->next;

			if (p->next)
			{
				p->next->prev = NULL;
			}
			else
			{
				lista->coada = NULL;
			}

		}
		if (p->info.numeSofer){
			free(p->info.numeSofer);
		}
		if (p->info.model){
			free(p->info.model);
		}
		free(p);
	}

}

char* getNumeSoferMasinaScumpa(LD lista) {
	Nod* p = lista.cap;
	Nod* max = p;
	if(lista.cap)
	{
		while (p)
		{
			if (p->info.pret > max->info.pret)
			{
				max = p;
			}
			p = p->next;
		}
		char* nume = malloc(strlen(max->info.numeSofer) + 1);
		strcpy_s(nume, strlen(max->info.numeSofer) + 1, max->info.numeSofer);
		return nume;
	}
	return NULL;
}

int main() {

	LD lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);
	stergeMasinaDupaID(&lista, 10);
	printf("Afisare lista de la final: \n");
	afisareListaMasiniDeLaFinal(lista);

	float suma = calculeazaPretMediu(lista);
	printf("Pretul mediu este : %.2f", suma);

	char* nume = getNumeSoferMasinaScumpa(lista);
	printf("\nNumele soferului cu cea mai scumpa masina este %s", nume);
	free(nume);

	dezalocareLDMasini(&lista);
	return 0;
}