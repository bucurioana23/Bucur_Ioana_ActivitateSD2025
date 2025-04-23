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

struct Heap {
	Masina* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	heap.lungime = lungime;
	heap.nrElemente = 0;
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int fs = pozitieNod * 2 + 1;
	int fd = pozitieNod * 2 + 2;
	int pozMax = pozitieNod;

	if (fs < heap.nrElemente && heap.vector[fs].id > heap.vector[pozMax].id)
	{
		pozMax = fs;
	}

	if (fd < heap.nrElemente && heap.vector[fd].id > heap.vector[pozMax].id)
	{
		pozMax = fd;
	}

	if (pozMax != pozitieNod)
	{
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		if (pozMax < (heap.nrElemente - 2) / 2) //2*pozMax - 1 <= heap.nrMasini -1
		{
			filtreazaHeap(heap, pozMax);
		}
	}

}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	Heap heap = initializareHeap(10);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		heap.vector[heap.nrElemente] = m;
		heap.nrElemente++;
	}
	fclose(f);

	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}

	return heap;
}

void afisareHeap(Heap heap) {
	if (heap.vector)
	{
		for (int i = 0; i < heap.nrElemente; i++)
		{
			afisareMasina(heap.vector[i]);
		}
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	if (heap->lungime > 0)
	{
		Masina aux = heap->vector[0];
		heap->vector[0] = heap->vector[heap->nrElemente - 1];
		heap->vector[heap->nrElemente - 1] = aux;
		heap->nrElemente--;

		for (int i = (heap->nrElemente - 2) / 2; i >= 0; i--)
		{
			filtreazaHeap(*heap, i);
		}

		return aux;
	}
	Masina m;
	m.numeSofer = NULL;
	m.model = NULL;
	return m;
}

//o lista cu masinile din heap care au id-ul mai mare de o valoare primita ca parametru
//masinile vor fi extrase din heap

struct Nod {
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

void afisareLista(Nod* lista)
{
	if (lista)
	{
		Nod* p = lista;
		while (p)
		{
			afisareMasina(p->info);
			p = p->next;
		}
	}
}

void inserareInLista(Nod** lista, Masina m)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = m;
	nou->next = *lista;

	(*lista) = nou;
}

Nod* masiniCuIdMare(Heap* heap, int idMinim)
{
	Nod* lista = NULL;

	while (heap->vector[0].id >= idMinim)
	{
		Masina m = extrageMasina(heap);
		inserareInLista(&lista, m);
	}

	return lista;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
}

int main() {

	Heap h = citireHeapDeMasiniDinFisier("masini_arbore.txt");
	printf("Elementele sunt:\n");
	afisareHeap(h);

	Nod* lista = masiniCuIdMare(&h, 4);
	printf("Lista:\n");
	afisareLista(lista);

	/*Masina m = extrageMasina(&h);
	printf("Prima masina extrasa este:\n");
	afisareMasina(m);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);
	extrageMasina(&h);*/

	printf("Elementele ascunse sunt:\n");
	afiseazaHeapAscuns(h);

	dezalocareHeap(&h);
	while (lista)
	{
		Nod* aux = lista;
		lista = lista->next;
		free(aux);
	}
	free(lista);
	return 0;
}