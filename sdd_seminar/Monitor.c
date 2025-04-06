#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Monitor {
	int cod;
	char* producator;
	float diagonala;
	int numarPorturi;
};
typedef struct Monitor Monitor;

typedef struct Nod Nod;
struct Nod {
	Monitor info;
	Nod* next;
};

void afisareMonitor(Monitor m)
{
	printf("Cod: %d\n", m.cod);
	printf("Producator: %s\n", m.producator);
	printf("Diagonala: %.2f\n", m.diagonala);
	printf("Numar porturi: %d\n\n", m.numarPorturi);
}

Monitor initializare(int cod, const char* producator, float diagonala, int numarPorturi)
{
	Monitor m;
	m.cod = cod;
	if (producator != NULL)
	{
		m.producator = (char*)malloc(strlen(producator) + 1);
		strcpy(m.producator, producator);
	}
	else
	{
		m.producator = NULL;
	}
	m.diagonala = diagonala;
	m.numarPorturi = numarPorturi;
	return m;
}

void adaugaMonitorInListaLaSfarsit(Nod** lista, Monitor m)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	
	if ((*lista) != NULL)
	{
		Nod* aux = (*lista);
		while (aux->next != (*lista))
		{
			aux = aux->next;
		}
		nou->next = (*lista);
		aux->next = nou;
	}
	else
	{
		nou->next = nou;
		(*lista) = nou;
	}
	
}

void afiseazaLista(Nod* lista)
{
	if (lista)
	{
		Nod* aux = lista;
		do
		{
			afisareMonitor(aux->info);
			aux = aux->next;
		} while (aux != lista);
	}
}

void dezalocareLista(Nod** lista)
{
	if (*lista)
	{
		Nod* adresaInitiala = (*lista);
		do
		{
			Nod* aux = (*lista);
			if (aux->info.producator != NULL)
			{
				free(aux->info.producator);
			}
			(*lista) = aux->next;
			free(aux);
		} while (*lista != adresaInitiala);
	}
	(*lista) = NULL;
}

Monitor stergeMonitorDinListaDupaCod(Nod** lista, int cod)
{
	if (*lista)
	{
		
		Nod* aux = (*lista);
		if (aux->info.cod == cod)
		{
			if (aux->next == aux)
			{
				(*lista) = NULL;
				return aux->info;
			}
			else
			{
				Nod* p = (*lista);
				(*lista) = aux->next;
				while(p->next != aux)
				{
					p = p->next;
				}
				p->next = (*lista);
				return aux->info;
			}
		}
		else
		{
			do
			{
				aux = aux->next;
			} while (aux != (*lista) && aux->next->info.cod != cod);

			if (aux->next != (*lista) && aux->info.cod == cod)
			{
				Nod* p = aux->next;
				aux->next = p->next;
				return p->info;
			}

		}	
	}
	return initializare(0, "", 0, 0);
}

int main()
{
	Monitor m1 = initializare(1, "Asus", 34.6, 5);
	Monitor m2 = initializare(2, "Lenovo", 40.6,6);
	Monitor m3 = initializare(3, "Huawei", 24.6, 4);
	Monitor m4 = initializare(4, "Hp", 43.6, 5);
	Monitor m5 = initializare(5, "Apple", 37.3, 7);

	Nod* lista = NULL;
	adaugaMonitorInListaLaSfarsit(&lista, m1);
	adaugaMonitorInListaLaSfarsit(&lista, m2);
	adaugaMonitorInListaLaSfarsit(&lista, m3);
	adaugaMonitorInListaLaSfarsit(&lista, m4);
	adaugaMonitorInListaLaSfarsit(&lista, m5);

	afiseazaLista(lista);
	
	Monitor m = stergeMonitorDinListaDupaCod(&lista, 0);

	printf("\nMonitorul cu codul cautat : \n");
	afisareMonitor(m);

	printf("\nLista dupa stergere:\n");
	afiseazaLista(lista);

	dezalocareLista(&lista);

	return 0;
}