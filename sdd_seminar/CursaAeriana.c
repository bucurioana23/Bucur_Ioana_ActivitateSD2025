#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CursaAeriana
{
	char* cod;
	char* destinatie;
	char* dataPlecare;
	char* oraPlecare;
	char status;
};
typedef struct CursaAeriana CursaAeriana;

struct Nod
{
	CursaAeriana info;
	struct Nod* next;
};
typedef struct Nod Nod;

CursaAeriana citesteCursaDinFisier(FILE* f)
{
	CursaAeriana c;
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, f);

	char* aux = strtok(buffer, sep);
	c.cod = (char*)malloc(strlen(aux) + 1);
	strcpy(c.cod, aux);

	aux = strtok(NULL, sep);
	c.destinatie = (char*)malloc(strlen(aux) + 1);
	strcpy(c.destinatie, aux);

	aux = strtok(NULL, sep);
	c.dataPlecare = (char*)malloc(strlen(aux) + 1);
	strcpy(c.dataPlecare, aux);

	aux = strtok(NULL, sep);
	c.oraPlecare = (char*)malloc(strlen(aux) + 1);
	strcpy(c.oraPlecare, aux);

	c.status = strtok(NULL, sep)[0];

	return c;
}

void afisareCursa(CursaAeriana c)
{
	printf("\nCod: %s\n", c.cod);
	printf("Destinatie: %s\n", c.destinatie);
	printf("Data plecare: %s\n", c.dataPlecare);
	printf("Ora plecare: %s\n", c.oraPlecare);
	printf("Status: %c\n", c.status);
}

void adaugaCursaInListaLaFinal(Nod** lista, CursaAeriana c)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	if (*lista)
	{
		Nod* p = (*lista);
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else
	{
		(*lista) = nou;
	}
}

void afisareLista(Nod* lista)
{
	while (lista)
	{
		afisareCursa(lista->info);
		lista = lista->next;
	}
}

Nod* citireListaDinFisier(const char* numeFisier)
{
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f))
	{
		CursaAeriana c = citesteCursaDinFisier(f);
		adaugaCursaInListaLaFinal(&lista, c);
	}
	return lista;
}

void dezalocareCursa(CursaAeriana c)
{
	if (c.cod != NULL)
	{
		free(c.cod);
	}

	if (c.dataPlecare != NULL)
	{
		free(c.dataPlecare);
	}

	if (c.destinatie != NULL)
	{
		free(c.destinatie);
	}

	if (c.oraPlecare != NULL)
	{
		free(c.oraPlecare);
	}
}

void stergeCurseDinListaCuStatusCautat(Nod** lista, char statusCautat)
{
	while ((*lista) != NULL && (*lista)->info.status == statusCautat)
	{
		Nod* p = (*lista);
		(*lista) = p->next;
		dezalocareCursa(p->info);
		free(p);
	}
	if (*lista)
	{
		Nod* p = (*lista);
		while (p)
		{
			while (p->next && p->next->info.status != statusCautat)
			{
				p = p->next;
			}
			if (p->next)
			{
				Nod* aux = p->next;
				p->next = aux->next;
				dezalocareCursa(aux->info);
				free(aux);
			}
			else
			{
				p = NULL;
			}
		}
	}
}

void stergeCursaDinListaCodCautat(Nod** lista, const char* codCautat)
{
	if (*lista)
	{
		if ((*lista)->next == NULL && strcmp((*lista)->info.cod,codCautat) == 0)
		{
			dezalocareCursa((*lista)->info);
			(*lista) = NULL;
		}
		else
		{
			Nod* p = (*lista);
			if (strcmp(p->info.cod,codCautat) == 0)
			{
				(*lista) = p->next;
				dezalocareCursa(p->info);
				free(p);
			}
			else
			{
				while (p->next && strcmp(p->next->info.cod,codCautat)!=0)
				{
					p = p->next;
				}
				if (p->next)
				{
					Nod* aux = p->next;
					p->next = aux->next;
					dezalocareCursa(aux->info);
					free(aux);
				}
			}
		}
	}
}


void dezalocareLista(Nod** lista)
{
	while ((*lista) != NULL)
	{
		Nod* p = (*lista);
		(*lista) = p->next;
		dezalocareCursa(p->info);
		free(p);
	}
	(*lista) = NULL;
}

int main()
{
	Nod* lista = citireListaDinFisier("curse.txt");
	afisareLista(lista);


	stergeCurseDinListaCuStatusCautat(&lista, 'n');
	stergeCursaDinListaCodCautat(&lista, "4D1");

	printf("\nLista dupa stergere: \n");
	afisareLista(lista);

	dezalocareLista(&lista);
	return 0;
}
