#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


struct Competitor {
	char* nume;
	float scorTotal;
	int numarProbe;
	char* nationalitate;
};
typedef struct Competitor Competitor;

Competitor citireCompetitorDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	Competitor c;
	char* aux;
	aux = strtok(buffer, sep);
	c.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(c.nume, aux);
	c.scorTotal = atof(strtok(NULL, sep));
	c.numarProbe = atoi(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	c.nationalitate = (char*)malloc(strlen(aux) + 1);
	strcpy(c.nationalitate, aux);

	return c;
}

void afisareCompetitor(Competitor c)
{
	printf("\nNume: %s\n", c.nume);
	printf("Scor: %.2f\n", c.scorTotal);
	printf("Numar probe: %d\n", c.numarProbe);
	printf("Nationalitate: %s\n", c.nationalitate);
}

struct Heap {
	int numarCompetitori;
	int lungime;
	Competitor* vector;
};
typedef struct Heap Heap;

Heap initializareHeap(int lungime)
{
	Heap heap;
	heap.lungime = lungime;
	heap.numarCompetitori = 0;
	heap.vector = (Competitor*)malloc(sizeof(Competitor) * lungime);
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod)
{
	int fs = 2 * pozitieNod + 1;
	int fd = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (fs < heap.numarCompetitori && heap.vector[fs].scorTotal > heap.vector[pozMax].scorTotal) {
		pozMax = fs;
	}
	if (fd < heap.numarCompetitori && heap.vector[fd].scorTotal > heap.vector[pozMax].scorTotal)
	{
		pozMax = fd;
	}
	if (pozMax != pozitieNod)
	{
		Competitor aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax < (heap.numarCompetitori - 2) / 2)
		{
			filtreazaHeap(heap, pozMax);
		}
	}

}

Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Heap heap = initializareHeap(10);
	while (!feof(f))
	{
		Competitor c = citireCompetitorDinFisier(f);
		heap.vector[heap.numarCompetitori] = c;
		heap.numarCompetitori++;
	}
	fclose(f);

	for (int i = (heap.numarCompetitori - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap)
{
	if (heap.vector)
	{
		for (int i = 0; i < heap.numarCompetitori; i++)
		{
			afisareCompetitor(heap.vector[i]);
		}
	}
}

void afisareHeapAscuns(Heap heap)
{
	for (int i = heap.numarCompetitori; i < heap.lungime; i++)
	{
		afisareCompetitor(heap.vector[i]);
	}
}

Competitor extrageCompetitor(Heap* heap)
{
	if (heap->lungime > 0)
	{
		Competitor aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->numarCompetitori - 1];
		heap->vector[heap->numarCompetitori - 1] = aux;
		heap->numarCompetitori--;

		for (int i = (heap->numarCompetitori - 2) / 2; i >= 0; i--)
		{
			filtreazaHeap(*heap, i);
		}

		return aux;
	}
	Competitor comp;
	comp.nationalitate = NULL;
	comp.nume = NULL;
	comp.scorTotal = -1;
	comp.numarProbe = -1;
	return comp;
}

void dezalocareHeap(Heap* heap)
{
	if (heap->vector)
	{
		for (int i = 0; i < heap->lungime; i++)
		{
			if (heap->vector[i].nume)
			{
				free(heap->vector[i].nume);
			}
			if (heap->vector[i].nationalitate)
			{
				free(heap->vector[i].nationalitate);
			}
		}
		free(heap->vector);
		heap->vector = NULL;
		heap->lungime = 0;
	}
}

//creati o lista simpla cu elementele din heap cu scor mai mare decat unul primit ca parametru
//competitorii respectivi vor fi extrasi din heap

struct Nod {
	Competitor info;
	struct Nod* next;
};
typedef struct Nod Nod;

void adaugaCompetitorInListaLaInceput(Nod** lista, Competitor c)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c; //shallow copy
	nou->next = (*lista);

	(*lista) = nou;
}

void afisareLista(Nod* lista)
{
	while (lista)
	{
		afisareCompetitor(lista->info);
		lista = lista->next;
	}
}

Nod* extrageCompetitoriDinHeapInLista(Heap* heap, int scor)
{
	Nod* lista = NULL;
	while (heap->vector[0].scorTotal >= scor)
	{
		Competitor c = extrageCompetitor(heap);
		adaugaCompetitorInListaLaInceput(&lista, c);
	}
	return lista;
}


int main()
{
	Heap heap = citireHeapDinFisier("competitori.txt");
	printf("Heap: ");
	afisareHeap(heap);

	printf("\nLISTA: \n");
	Nod* lista = extrageCompetitoriDinHeapInLista(&heap, 90);
	afisareLista(lista);

	extrageCompetitor(&heap);
	extrageCompetitor(&heap);

	printf("\nHeap ascuns: \n");
	afisareHeapAscuns(heap);

	printf("\nHeap dupa extragere: \n");
	afisareHeap(heap);

	dezalocareHeap(&heap);
	while (lista)
	{
		Nod* aux = lista;
		lista = lista->next;
		free(aux);
	}
	free(lista);
	return 0;
}
