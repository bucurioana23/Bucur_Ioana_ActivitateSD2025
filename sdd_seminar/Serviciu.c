#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Serviciu {
	unsigned int id;
	char* tipServiciu;
	char* numeClient;
	float sumaDePlata;
	unsigned int timpMin;
};
typedef struct Serviciu Serviciu;

void afiseazaServciu(Serviciu s)
{
	printf("Id: %d\n", s.id);
	printf("Tip serviciu: %s\n", s.tipServiciu);
	printf("Nume client: %s\n", s.numeClient);
	printf("Suma de plata: %.2f\n", s.sumaDePlata);
	printf("Timp in minute: %d\n\n", s.timpMin);

}

struct Nod
{
	Serviciu info;
	struct Nod* next;
};
typedef struct Nod Nod;

void afiseazaListaServicii(Nod* lista)
{
	if (lista)
	{
		while (lista)
		{
			afiseazaServciu(lista->info);
			lista = lista->next;
		}
	}
}

struct HashTable {
	Nod** vector;
	int dim;
};
typedef struct HashTable Hashtable;

Serviciu citireServiciuDinFisier(FILE* f)
{
	Serviciu s;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, f);
	aux = strtok(buffer, sep);
	s.id = atoi(aux);
	aux = strtok(NULL, sep);
	
	s.tipServiciu = (char*)malloc(strlen(aux) + 1);
	strcpy(s.tipServiciu, aux);
	
	aux = strtok(NULL, sep);
	
	s.numeClient = (char*)malloc(strlen(aux) + 1);
	strcpy(s.numeClient, aux);

	s.sumaDePlata = atof(strtok(NULL, sep));
	s.timpMin = atoi(strtok(NULL, sep));

	return s;
}

void adaugaServiciuInLista(Nod** lista, Serviciu s)
{
	Nod* nou = malloc(sizeof(Nod));
	nou->info = s; //shallow copy
	nou->next = NULL;

	if ((*lista)!=NULL)
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

Hashtable initializareHashTable(int dimensiune)
{
	Hashtable ht;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	ht.dim = dimensiune;

	for (int i = 0; i < dimensiune; i++)
	{
		ht.vector[i] = NULL;
	}

	return ht;
}

int calculeazaHash(const char* numeClient, int dimensiune)
{
	int suma = 0;
	for (int i = 0; i < strlen(numeClient); i++)
	{
		suma += numeClient[i];
	}

	return (dimensiune > 0 ? (suma % dimensiune) : 0);
}

void adaugaServiciuInTabela(Hashtable ht, Serviciu s)
{
	int pozitie = calculeazaHash(s.numeClient, ht.dim);
	if (pozitie < ht.dim)
	{
		adaugaServiciuInLista(&(ht.vector[pozitie]), s);
	}
}

Hashtable citireServiciiDinFisier(const char* numeFisier, int dimensiune)
{
	FILE* f = fopen(numeFisier, "r");
	Hashtable ht = initializareHashTable(5);
	while (!feof(f))
	{
		Serviciu s = citireServiciuDinFisier(f);
		adaugaServiciuInTabela(ht, s);
		
	}
	fclose(f);
	return ht;
}

void afisareServiciiDinTabela(Hashtable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("\nIn clusterul %d: \n", i);
			afiseazaListaServicii(ht.vector[i]); 
		}
		else
		{
			printf("\nIn clusterul %d nu avem servicii!!\n", i);
		}
	}
}

int cautaSevciiClientInLista(Nod* lista, const char* nume)
{
	int contor = 0;
	while (lista)
	{
		if (strcmp(lista->info.numeClient, nume) == 0)
		{
			contor++;
		}
		lista = lista->next;
	}
	return contor;
}


int getNumarServiciiCeruteDeCatreUnAnumitClient(Hashtable ht, const char* numeClient)
{
	int numar = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			numar += cautaSevciiClientInLista(ht.vector[i], numeClient);
		}
	}
	return numar;
}

void stergeUltimulElementDinLista(Nod** lista)
{
	if (*lista)
	{
		Nod* p = (*lista);
		if (p->next == NULL)
		{
			if (p->info.numeClient != NULL)
			{
				free(p->info.numeClient);
			}
			if (p->info.tipServiciu != NULL)
			{
				free(p->info.tipServiciu);
			}
			free(p);
			(*lista) = NULL;
		}
		else
		{
			Nod* aux = p->next;
			while (aux->next != NULL && p->next)
			{
				p = p->next;
				aux = p->next;
			}
			if (aux->next == NULL)
			{
				p->next = NULL;
				if (aux->info.numeClient != NULL)
				{
					free(aux->info.numeClient);
				}
				if (aux->info.tipServiciu != NULL)
				{
					free(aux->info.tipServiciu);
				}
				free(aux);
			}
		}

	}
}

void stergeElementDinTabela(Hashtable ht, int pozitie)
{
	if (ht.vector[pozitie] != NULL)
	{
		stergeUltimulElementDinLista(&(ht.vector[pozitie]));
	}
}

int cautaServiciiCuOAnumitaDurataInLista(Nod* lista, int durata)
{
	int numar = 0;
	while (lista)
	{
		if (lista->info.timpMin > durata)
		{
			numar++;
		}
		lista = lista->next;
	}
	return numar;
}

int numarElementeDeCopiatInVector(Hashtable ht, int durata)
{
	int contor = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			contor += cautaServiciiCuOAnumitaDurataInLista(ht.vector[i], durata);
		}
	}
}

Serviciu* copiazaInVectorServiciiCareDepasescOAnumitaDurataDeTimp(Hashtable ht, int durata, int contor)
{
	Serviciu* servicii = (Serviciu*)malloc(sizeof(Serviciu) * contor);
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			Nod* p = ht.vector[i];
			while (p)
			{
				if (p->info.timpMin > durata)
				{
					servicii[k] = p->info;
					if (p->info.numeClient != NULL)
					{
						servicii[k].numeClient = malloc(strlen(p->info.numeClient) + 1);
						strcpy(servicii[k].numeClient, p->info.numeClient);
					}
					else
					{
						servicii[k].numeClient = NULL;
					}

					if (p->info.tipServiciu != NULL)
					{
						servicii[k].tipServiciu = malloc(strlen(p->info.tipServiciu) + 1);
						strcpy(servicii[k].tipServiciu, p->info.tipServiciu);
					}
					else
					{
						servicii[k].tipServiciu= NULL;
					}
					k++;
				}
				p = p->next;
			}
		}
	}

	return servicii;
}

void dezalocareLista(Nod** lista)
{
	while (*lista)
	{
		Nod* p = (*lista);
		(*lista) = (*lista)->next;

		if (p->info.numeClient != NULL)
		{
			free(p->info.numeClient);
		}
		if (p->info.tipServiciu != NULL)
		{
			free(p->info.tipServiciu);
		}
		free(p);
	}
	(*lista) = NULL;
}

void dezalocareTabela(Hashtable* ht)
{
	for (int i = 0; i < ht->dim; i++)
	{
		if (ht->vector[i] != NULL)
		{
			dezalocareLista(&(ht->vector[i]));
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dim = 0;
}

int main()
{
	Hashtable ht = citireServiciiDinFisier("servicii.txt", 5);
	afisareServiciiDinTabela(ht);

	int numar = getNumarServiciiCeruteDeCatreUnAnumitClient(ht, "Rebecca");
	printf("\n\nNumar servicii cerute de clientul : %d\n", numar);

	stergeElementDinTabela(ht, 2);
	printf("\n\nLista dupa ce am sters ultimul element din clusterul 2:\n");
	afisareServiciiDinTabela(ht);

	int contor = numarElementeDeCopiatInVector(ht, 20);
	Serviciu* servicii = copiazaInVectorServiciiCareDepasescOAnumitaDurataDeTimp(ht, 20,contor);
	printf("\n\nVector servicii cu durata mai mare de 20 de minute:\n");
	for (int i = 0; i < contor; i++)
	{
		afiseazaServciu(servicii[i]);
	}
	
	dezalocareTabela(&ht);
	for (int i = 0; i < contor; i++)
	{
		if (servicii[i].numeClient != NULL)
		{
			free(servicii[i].numeClient);
		}

		if (servicii[i].tipServiciu != NULL)
		{
			free(servicii[i].tipServiciu);
		}
	}
	free(servicii);
	servicii = NULL;

	return 0;
}