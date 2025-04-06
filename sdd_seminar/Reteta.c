#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Reteta
{
	unsigned int numar;
	unsigned char numarMedicamente;
	char** listaMedicamente;
	unsigned char* cantitati;
	float* preturi;
	unsigned char* procente;
	char* numeMedic;
};
typedef struct Reteta Reteta;

typedef struct Nod Nod;
struct Nod {
	Reteta info;
	Nod* next;
	Nod* prev;
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla LD;

Reteta initializare(unsigned int numar, unsigned char numarMedicamente,
char** listaMedicamente,
const unsigned char* cantitati,
float* preturi,
const unsigned char* procente,
const char* numeMedic)
{
	Reteta r;
	r.numar = numar;
	r.numarMedicamente = numarMedicamente;
	r.listaMedicamente = malloc(sizeof(char*)*numarMedicamente);
	for (int i = 0; i < numarMedicamente; i++)
	{
		r.listaMedicamente[i] = malloc(strlen(listaMedicamente[i])+1);
		strcpy(r.listaMedicamente[i], listaMedicamente[i]);
	}
	r.cantitati = malloc(sizeof(unsigned char) * numarMedicamente);
	for (int i = 0; i < numarMedicamente; i++)
	{
		r.cantitati[i] = cantitati[i];
	}

	// Copiere preturi
	r.preturi = malloc(sizeof(float) * numarMedicamente);
	for (int i = 0; i < numarMedicamente; i++)
	{
		r.preturi[i] = preturi[i];
	}

	// Copiere procente
	r.procente = malloc(sizeof(unsigned char) * numarMedicamente);
	for (int i = 0; i < numarMedicamente; i++)
	{
		r.procente[i] = procente[i];
	}

	// Copiere nume medic
	r.numeMedic = malloc(strlen(numeMedic) + 1);
	strcpy(r.numeMedic, numeMedic);

	return r;

}

void afisareReteta(Reteta r)
{
	printf("Numarul retetei: %d\n", r.numar);
	printf("Numarul de medicamente: %d%\n", r.numarMedicamente);
	printf("Lista de medicamente: ");
	for (int i = 0; i < r.numarMedicamente; i++)
	{
		printf(" %s ", r.listaMedicamente[i]);
	}
	printf("\nCantitati: ");
	for (int i = 0; i < r.numarMedicamente; i++)
	{
		printf(" %d ", r.cantitati[i]);
	}
	printf("\nPreturi: ");
	for (int i = 0; i < r.numarMedicamente; i++)
	{
		printf(" %.2f ", r.preturi[i]);
	}
	printf("\nProcente: ");
	for (int i = 0; i <r.numarMedicamente; i++)
	{
		printf(" %d% ", r.procente[i]);
	}
	printf("\nNumele medicului: %s", r.numeMedic);
}

void adaugaRetetaInListaLaFinal(LD* lista, Reteta r)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = initializare(r.numar, r.numarMedicamente, r.listaMedicamente, r.cantitati, r.preturi, r.procente, r.numeMedic);
	nou->prev = lista->last;
	nou->next = NULL;
	if (lista->last != NULL)
	{
		lista->last->next = nou;
	}
	else
	{
		lista->first = nou;
	}
	lista->last = nou;
}

void afisareListaDeLaInceput(LD lista)
{
	Nod* p = lista.first;
	while (p)
	{
		afisareReteta(p->info);
		p = p->next;
	}
}

int main()
{
	char** c = malloc(sizeof(char*) * 3);
	c[0] = malloc(strlen("nurofen") + 1);
	strcpy(c[0], "nurofen");
	c[1] = malloc(strlen("paracetamol") + 1);
	strcpy(c[1], "paracetamol"); 
	c[2] = malloc(strlen("klacid") + 1);
	strcpy(c[2], "klacid");

	float* p = malloc(sizeof(float) * 3);
	p[0] = 34.56;
	p[1] = 55.56;
	p[2] = 23.96;

	Reteta r1 = initializare(1, 3, c, (unsigned char[]) { 2, 4, 6 }, p, (unsigned char[]) { 10, 20, 30 }, "Bucur");
	afisareReteta(r1);
	return 0;
}