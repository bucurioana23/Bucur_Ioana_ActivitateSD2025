#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

struct Moneda {
	char* taraEmitenta;
	int valNominala;
	float greutate;
	int anEmitere;
};
typedef struct Moneda Moneda;

typedef struct Nod Nod;
struct Nod {
	Moneda info;
	Nod* next;
};

Moneda initializare(const char* taraEmitenta, int valNominala, float greutate, int anEmitere)
{
	Moneda m;
	if (taraEmitenta != NULL)
	{
		m.taraEmitenta = (char*)malloc(strlen(taraEmitenta) + 1);
		strcpy(m.taraEmitenta, taraEmitenta);
	}
	else
	{
		taraEmitenta = NULL;
	}
	m.valNominala = valNominala;
	m.greutate = greutate;
	m.anEmitere = anEmitere;
	return m;
}

void afisareMoneda(Moneda m)
{
	printf("Tara emitenta: %s\n", m.taraEmitenta);
	printf("Valoare nominala: %d\n", m.valNominala);
	printf("Greutate: %.2f\n", m.greutate);
	printf("An emitere: %d\n\n", m.anEmitere);
}

void adaugaInListaLaFinal(Nod** lista, Moneda m)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = NULL;

	if ((*lista) == NULL)
	{
		(*lista) = nou;
	}
	else
	{
		Nod* p = (*lista);
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = nou;
	}

}

void afisareListaMonede(Nod* lista)
{
	while (lista)
	{
		afisareMoneda(lista->info);
		lista = lista->next;
	}
}

void dezalocareListaMonede(Nod** lista)
{
	while (*lista)
	{
		Nod* aux = (*lista);
		if (aux->info.taraEmitenta != NULL)
		{
			free(aux->info.taraEmitenta);
		}
		(*lista) = aux->next;
		free(aux);
	}
}

Moneda getMonedaGreutateMaxima(Nod* lista)
{
	if (lista) {
		Nod* max = lista;
		Nod* p = lista->next;
		while (p)
		{
			if (p->info.greutate > max->info.greutate)
			{
				max = p;
			}
			p = p->next;
		}
		Moneda m = initializare(max->info.taraEmitenta, max->info.valNominala, max->info.greutate, max->info.anEmitere);
		return m;
	}
	return initializare("",0,0,0);
}

void stergeMonedaDupaGreutate(Nod** lista, float greutate)
{
	if (*lista)
	{
		if ((*lista)->info.greutate == greutate && (*lista)->next == NULL)
		{
			dezalocareListaMonede(*lista);
			(*lista) = NULL;
		}
		Nod* p = (*lista);
		if (p->info.greutate == greutate)
		{
			(*lista) = p->next;
			if (p->info.taraEmitenta != NULL)
			{
				free(p->info.taraEmitenta);
			}
			free(p);
		}
		else {
			while (p)
			{
				while (p->next && p->next->info.greutate != greutate)
				{
					p = p->next;
				}
				if (p->next)
				{
					Nod* aux = p->next;
					p->next = aux->next;
					if (aux->info.taraEmitenta != NULL)
					{
						free(aux->info.taraEmitenta);
					}
					free(aux);
				}
				else
				{
					p = NULL;
				}
			}
		}
		
	
	}
}

void stergeMonedaIndex(Nod** lista, int index)
{
	if (*lista)
	{
		if (index == 0 && (*lista)->next == NULL)
		{
			if ((*lista)->info.taraEmitenta != NULL)
			{
				free((*lista)->info.taraEmitenta);
			}
			free(*lista);
			(*lista) = NULL;
		}
		else
		{
			int poz = 0;
			Nod* p = (*lista);
			if (index == 0)
			{
				(*lista) = p->next;
				if (p->info.taraEmitenta != NULL)
				{
					free(p->info.taraEmitenta);
				}
				free(p);
			}
			else {
				while (p && poz + 1 != index)
				{
					poz++;
					p = p->next;
				}

				if (p)
				{
					Nod* aux = p->next;
					p->next = aux->next;
					if (aux->info.taraEmitenta != NULL)
					{
						free(aux->info.taraEmitenta);
					}
					free(aux);
				}
			}
		}
	}
}

int main()
{
	Moneda m1 = initializare("Romania", 1, 0.01, 2020);
	Moneda m2 = initializare("Franta", 5, 0.45, 1920);
	Moneda m3 = initializare("Mongolia", 2, 0.65, 1990);
	Moneda m4 = initializare("China", 3, 0.12, 1987);
	Moneda m5 = initializare("Grecia", 4, 0.23, 1980);

	Nod* lista = NULL;
	adaugaInListaLaFinal(&lista, m1);
	adaugaInListaLaFinal(&lista, m2);
	adaugaInListaLaFinal(&lista, m3);
	adaugaInListaLaFinal(&lista, m4);
	adaugaInListaLaFinal(&lista, m5);

	afisareListaMonede(lista);

	Moneda m = getMonedaGreutateMaxima(lista);
	printf("\nMoneda cu cea mai mare greutate: \n");
	afisareMoneda(m);

	stergeMonedaDupaGreutate(&lista, 0.12);

	printf("\n Lista dupa stergere moneda: \n");
	afisareListaMonede(lista);

	stergeMonedaIndex(&lista, 3);
	printf("\n Lista dupa stergere index 0: \n");
	afisareListaMonede(lista);

	dezalocareListaMonede(&lista);
	if (m.taraEmitenta != NULL)
	{
		free(m.taraEmitenta);
	}

	return 0;
}
