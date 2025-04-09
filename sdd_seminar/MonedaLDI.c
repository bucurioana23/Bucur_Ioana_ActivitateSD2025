#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Moneda {
	char* taraEmitenta;
	int valNominala;
	float greutate;
	int anEmitere;
};
typedef struct Moneda Moneda;

struct Nod
{
	Moneda info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla
{
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla LD;

Moneda initializareMoneda(const char* taraEmitenta, int valoareNominala,float greutate,int anEmitere)
{
	Moneda m;
	if (taraEmitenta != NULL)
	{
		m.taraEmitenta = malloc(strlen(taraEmitenta) + 1);
		strcpy(m.taraEmitenta, taraEmitenta);
	}
	else
	{
		m.taraEmitenta = NULL;
	}
	m.valNominala = valoareNominala;
	m.greutate = greutate;
	m.anEmitere = anEmitere;

	return m;
}

void adaugaMonedaLaListaDublaSfarsit(LD* lista, Moneda m)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = m; //shallow copy
	nou->next = NULL;
	nou->prev = lista->last;
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

void adaugaMonedaInListaLaInceput(LD* lista, Moneda m)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = m;//shallow copy
	nou->next = lista->first;
	nou->prev = NULL;
	if (lista->first != NULL)
	{
		lista->first->prev = nou;
	}
	else
	{
		lista->last = nou;
	}
	lista->first = nou;
}

void afisareMoneda(Moneda m)
{
	printf("Tara emitenta: %s\n", m.taraEmitenta);
	printf("Valoare nominala: %d\n", m.valNominala);
	printf("Greutate: %.2f\n", m.greutate);
	printf("An emitere: %d\n\n", m.anEmitere);
}

void afisareListaDublaDeLaInceput(LD lista)
{
	Nod* p = lista.first;
	while (p)
	{
		afisareMoneda(p->info);
		p = p->next;
	}
}

void afisareListaDublaDeLaSfarsit(LD lista)
{
	Nod* p = lista.last;
	while (p)
	{
		afisareMoneda(p->info);
		p = p->prev;
	}
}

void dezalocareListaDubla(LD* lista)
{
	while (lista->first)
	{
		if (lista->first->info.taraEmitenta != NULL)
		{
			free(lista->first->info.taraEmitenta);
		}
		Nod* p = lista->first;
		lista->first = p->next;
		free(p);
	}
	lista->last = NULL;
}

float calculeazaGreutateaMedie(LD lista)
{
	float suma = 0;
	int contor = 0;
	while (lista.first)
	{
		suma += lista.first->info.greutate;
		contor += 1;
		lista.first = lista.first->next;
	}

	return (contor > 0 ? (suma / contor) : 0);
}

void stergeMonedaDupaValoareNominala(LD* lista, int valoare)
{
	if (lista->first && lista->last==lista->first && lista->first->info.valNominala == valoare)
	{
		dezalocareListaDubla(lista);
	}
	else if (lista->first && lista->first->info.valNominala == valoare)
	{
		Nod* p = lista->first;
		lista->first = p->next;
		lista->first->prev = NULL;
		if (p->info.taraEmitenta != NULL)
		{
			free(p->info.taraEmitenta);
		}
		free(p);
	}
	else
	{	
		Nod* p = lista->first;
		while (p->next && p->next->info.valNominala != valoare)
		{
			p = p->next;
		}
		if (p->next)
		{
			Nod* aux = p->next;
			if (aux->next)
			{
				p->next = aux->next;
				aux->next->prev = p;
				if (aux->info.taraEmitenta != NULL)
				{
					free(aux->info.taraEmitenta);
				}
				free(aux);
			}
			else
			{
				p->next = aux->next;
				if (aux->info.taraEmitenta != NULL)
				{
					free(aux->info.taraEmitenta);
				}
				free(aux);
				lista->last = p;
			}
		}
		
	}
}

struct ListaSimpla {
	Moneda info;
	struct ListaSimpla* next;
};
typedef struct ListaSimpla LS;

void adaugaInListaSimplaLaFinal(LS** lis, Moneda m)
{
	LS* nou = malloc(sizeof(LS));
	nou->info = m;
	if (m.taraEmitenta != NULL)
	{
		nou->info.taraEmitenta = malloc(strlen(m.taraEmitenta) + 1);
		strcpy(nou->info.taraEmitenta, m.taraEmitenta);
	}
	else
	{
		nou->info.taraEmitenta = NULL;
	}
	nou->next = NULL;

	if (*lis)
	{
		LS* p = (*lis);
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else
	{
		(*lis) = nou;
	}
}

void afisareLS(LS* lis)
{
	while (lis)
	{
		afisareMoneda(lis->info);
		lis = lis->next;
	}
}

LS* adaugaMonedeCuGreutateMaiMareInListaSimpla(LD* lista, float greutate)
{
	LS* lis = NULL;
	Nod* p = lista->first;
	while (p)
	{
		Nod* next = p->next;
		if (p->info.greutate > greutate)
		{
			adaugaInListaSimplaLaFinal(&lis, p->info);
			stergeMonedaDupaValoareNominala(lista, p->info.valNominala);
		}
		p = next;
	}
	return lis;
}

int main()
{
	Moneda m1 = initializareMoneda("Romania", 1, 0.01, 2020);
	Moneda m2 = initializareMoneda("Franta", 5, 0.45, 1920);
	Moneda m3 = initializareMoneda("Mongolia", 2, 0.65, 1990);
	Moneda m4 = initializareMoneda("China", 3, 0.12, 1987);
	Moneda m5 = initializareMoneda("Grecia", 4, 0.23, 1980);

	LD lista;
	lista.first = NULL;
	lista.last = NULL;
	adaugaMonedaLaListaDublaSfarsit(&lista, m1);
	adaugaMonedaLaListaDublaSfarsit(&lista, m2);
	adaugaMonedaLaListaDublaSfarsit(&lista, m3);
	adaugaMonedaLaListaDublaSfarsit(&lista, m4);
	adaugaMonedaInListaLaInceput(&lista, m5);

	afisareListaDublaDeLaInceput(lista);

	float greutate = calculeazaGreutateaMedie(lista);
	printf("\ngreutate medie : %.2f\n", greutate);

	LS* lis = adaugaMonedeCuGreutateMaiMareInListaSimpla(&lista, 0.2);
	printf("\nAfisare lista dupa stergere: \n");
	afisareListaDublaDeLaInceput(lista);
	printf("\nAfisare lista SIMPLA : \n");
	afisareLS(lis);

	dezalocareListaDubla(&lista);

	return 0;
}