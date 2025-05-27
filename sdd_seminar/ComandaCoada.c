#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

struct Comanda {
	int cod;
	char* numeClient;
	int numarProduse;
	float sumaPlata;
};
typedef struct Comanda Comanda;

Comanda citireComandaDinFisier(FILE* f)
{
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	Comanda c;
	fgets(buffer, 99, f);

	aux = strtok(buffer, sep);
	c.cod = atoi(aux);

	aux = strtok(NULL, sep);
	c.numeClient = (char*)malloc(strlen(aux) + 1);
	strcpy(c.numeClient, aux);

	c.numarProduse = atoi(strtok(NULL, sep));
	c.sumaPlata = atof(strtok(NULL, sep));

	return c;
}

void afisareComanda(Comanda c)
{
	printf("\nCod: %d\n", c.cod);
	printf("Nume client: %s\n", c.numeClient);
	printf("Numar produse: %d\n", c.numarProduse);
	printf("Suma plata: %.2f\n", c.sumaPlata);
}

struct Nod{
	Comanda info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct Coada {
	Nod* first;
	Nod* last;
};
typedef struct Coada Coada;

void enqueue(Coada* coada, Comanda c)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	nou->prev = coada->last;
	if (coada->last)
	{
		coada->last->next = nou;
	}
	else {
		coada->first = nou;
	}
	coada->last = nou;
}

void afiseazaCoada(Coada c)
{
	if (c.first)
	{
		Nod* aux = c.first;
		while (aux)
		{
			afisareComanda(aux->info);
			aux = aux->next;
		}
	}
}

Coada citireCoadaDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Coada coada;
	coada.first = NULL;
	coada.first = NULL;
	while (!feof(f))
	{
		Comanda c = citireComandaDinFisier(f);
		enqueue(&coada, c);
	}
	fclose(f);
	return coada;
}

Comanda dequeue(Coada* coada)
{
	if (coada->first)
	{
		Nod* aux = coada->first;
		Comanda m = aux->info;
		coada->first = aux->next;
		free(aux);
		if (coada->first)
		{
			coada->first->prev = NULL;
		}
		else
		{
			coada->last = NULL;
		}
		return m;
	}
	else return (Comanda) { -1, "", -1, -1.0 };
}

int getNumarComenziMaiMareDe100(Coada coada)
{
	int numar = 0;
	if (coada.first)
	{
		Nod* aux = coada.first;
		while (aux) {
			if (aux->info.sumaPlata > 100)
			{
				numar++;
			}
			aux = aux->next;
		}
	}
	return numar;
}

void adaugaComandaInVector(Coada coada, Comanda* vector)
{
	if (coada.first)
	{
		Nod* aux = coada.first;
		int index = 0;
		while (aux)
		{
			if (aux->info.sumaPlata > 100)
			{
				vector[index] = aux->info;
				index++;
			}
			aux = aux->next;
		}

	}
}

Comanda* getVectorComenzi(Coada coada, int* dimensiune)
{
	if (coada.first)
	{
		int dim = getNumarComenziMaiMareDe100(coada);
		*dimensiune = dim;
		if (dim == 0)
		{
			return NULL;
		}

		Comanda* vector = (Comanda*)malloc(sizeof(Comanda) * dim);
		adaugaComandaInVector(coada, vector);

		for (int i = 0; i < dim - 1; i++) {
			for (int j = i + 1; j < dim; j++) {
				if (vector[i].numarProduse < vector[j].numarProduse) {
					Comanda aux = vector[i];
					vector[i] = vector[j];
					vector[j] = aux;
				}
			}
		}
		return vector;
	}
	return NULL;
}

void dezalocareCoada(Coada* coada)
{
	while (coada->first)
	{
		Comanda c = dequeue(coada);
		if (c.numeClient)
		{
			free(c.numeClient);
		}
	}
}

int main()
{
	Coada coada = citireCoadaDinFisier("comenziCoada.txt");
	afiseazaCoada(coada);

	Comanda c = dequeue(&coada);
	printf("\nMasina luata cu dequeue: \n");
	afisareComanda(c);

	printf("\nLista dupa dequeue: \n");
	afiseazaCoada(coada);

	int dimensiune = 0;
	Comanda* vector = getVectorComenzi(coada, &dimensiune);

	printf("\nVector: \n");
	for (int i = 0; i < dimensiune; i++)
	{
		afisareComanda(vector[i]);
	}

	dezalocareCoada(&coada);
	free(vector);
	vector = NULL;
	return 0;
}