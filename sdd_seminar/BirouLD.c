#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraBirou {
	int cod;
	char* material;
	float dimensiuni[3];
	char initialaCuloare;
};
typedef struct StructuraBirou Birou;

typedef struct Nod Nod;
struct Nod {
	Birou info;
	Nod* prev;
	Nod* next;
};

struct ListaDubla {
	Nod* first;
	Nod* last;
};
typedef struct ListaDubla LD;

Birou initializare(int cod, const char* material, float dimensiuni[3], char initialaCuloare)
{
	Birou b;
	b.cod = cod;
	if (material != NULL)
	{
		b.material = malloc(strlen(material) + 1);
		strcpy(b.material, material);
	}
	for (int i = 0; i < 3; i++)
	{
		b.dimensiuni[i] = dimensiuni[i];
	}
	b.initialaCuloare = initialaCuloare;
	return b;
}

void afisareBirou(Birou b)
{
	printf("\n\nBIROUL CERUT: \n");
	printf("Cod: %d\n", b.cod);
	printf("Material: %s\n", b.material);
	for (int i = 0; i < 3; i++)
	{
		printf("Dimensiunea %d: %.2f\n", i, b.dimensiuni[i]);
	}
	printf("Initiala culoare: %c\n", b.initialaCuloare);
}

void afisareListaBirouriDeLaInceput(LD lista)
{
	Nod* cap = lista.first;
	while (cap)
	{
		afisareBirou(cap->info);
		cap = cap->next;
	}
}

void afisareListaBirouriDeLaSfarsit(LD lista)
{
	Nod* coada = lista.last;
	while (coada)
	{
		afisareBirou(coada->info);
		coada = coada->prev;
	}
}


Birou citireBirouDinFisier(FILE* f)
{
	Birou b;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);
	char* aux;
	aux = strtok(buffer, sep);
	b.cod = atoi(aux);
	aux = strtok(NULL, sep);
	b.material = malloc(strlen(aux) + 1);
	strcpy(b.material, aux);
	for (int i = 0; i < 3; i++)
	{
		b.dimensiuni[i] = atof(strtok(NULL, sep));
	}
	b.initialaCuloare = strtok(NULL, sep)[0];
	return b;
}

void adaugaBirouInListaLaFinal(LD* lista, Birou b)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = b;
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

void adaugaBirouInListaLaInceput(LD* lista, Birou b)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = b;
	if (b.material != NULL)
	{
		nou->info.material = malloc(strlen(b.material) + 1);
		strcpy(nou->info.material, b.material);
	}
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

LD citireListaDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	LD lista;
	lista.first = NULL;
	lista.last = NULL;
	while (!feof(f))
	{
		Birou b = citireBirouDinFisier(f);
		adaugaBirouInListaLaFinal(&lista, b);
	}
	fclose(f);
	return lista;
}

void dezalocare(LD* lista)
{
	while (lista->first)
	{
		if (lista->first->info.material)
		{
			free(lista->first->info.material);
		}
		Nod* p = lista->first;
		lista->first = p->next;
		free(p);
	}
	lista->last = NULL;

}

void stergeBirouDupaCod(LD* lista, int cod)
{
	if (lista)
	{
		Nod* p = lista->first;
		if (p->info.cod == cod)
		{
			lista->first = p->next;
			p->next->prev = NULL;
			if (p->info.material != NULL)
			{
				free(p->info.material);
			}
			free(p);

		}
		else
		{
			while (p->next && p->next->info.cod != cod)
			{
				p = p->next;
			}
			if (p->next)
			{
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.material != NULL)
				{
					free(aux->info.material);
				}
				free(aux);
				if (p->next == NULL)
				{
					lista->last = p;
				}
			}
		}
	}
}

char* getMaterialBirouCodMaxim(LD lista)
{
	if (lista.first != NULL)
	{
		Nod* max = lista.first;
		while (lista.first)
		{
			if (lista.first->info.cod >= max->info.cod)
			{
				max = lista.first;
			}
			lista.first = lista.first->next;
		}
		return max->info.material;
	}
	return "";
}

int main()
{
	LD lista = citireListaDinFisier("birouri.txt");
	Birou b1 = initializare(1, "lemn", (float[]) { 140.0, 60.0, 40.0 }, 'n');
	adaugaBirouInListaLaInceput(&lista, b1);
	afisareListaBirouriDeLaSfarsit(lista);

	stergeBirouDupaCod(&lista, 50);
	printf("\n LISTA DUPA STERGERE: \n");
	afisareListaBirouriDeLaInceput(lista);

	char* material = getMaterialBirouCodMaxim(lista);
	printf(" \n\nMaterialul biroului cu cod maxim: %s\n\n", material);
	dezalocare(&lista);
	return 0;
}