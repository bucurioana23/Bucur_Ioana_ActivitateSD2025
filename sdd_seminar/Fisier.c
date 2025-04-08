#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Fisier {
	char* nume;
	char* cale;
	char* extensie;
	unsigned int dimensiuneBytes;
};
typedef struct Fisier Fisier;

struct Nod
{
	Fisier info;
	struct Nod* next;
};
typedef struct Nod Nod;

struct Hashtable {
	Nod** vector;
	int dim;
};
typedef struct Hashtable Hashtable;

Fisier initializare(const char* nume, const char* cale, const char* extensie, int dimensiuneBytes)
{
	Fisier f;
	if (nume != NULL)
	{
		f.nume = (char*)malloc(strlen(nume) + 1);
		strcpy(f.nume, nume);
	}

	if (cale != NULL)
	{
		f.cale = (char*)malloc(strlen(cale) + 1);
		strcpy(f.cale, cale);
	}

	if (extensie != NULL)
	{
		f.extensie = (char*)malloc(strlen(extensie) + 1);
		strcpy(f.extensie, extensie);
	}

	f.dimensiuneBytes = dimensiuneBytes;

	return f;
}

void afisareFisier(Fisier f)
{
	printf("Nume fisier: %s\n", f.nume);
	printf("Cale absoluta: %s\n", f.cale);
	printf("Extensie: %s\n", f.extensie);
	printf("Dimensiune fisier in bytes: %d\n\n", f.dimensiuneBytes);
}

void adaugaFisierInLista(Nod** lista, Fisier f)
{
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = initializare(f.nume, f.cale, f.extensie, f.dimensiuneBytes);
	nou->next = NULL;
	if ((*lista)!=NULL)
	{
		Nod* p = (*lista);
		while(p->next)
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

void afisareListaFisiere(Nod* lista)
{
	while (lista)
	{
		afisareFisier(lista->info);
		lista = lista->next;
	}
}

Hashtable initializareHashtable(int dimensiune)
{
	Hashtable ht;
	ht.dim = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < ht.dim; i++)
	{
		ht.vector[i] = NULL;
	}
	return ht;
}

int calculeazaHash(int dimensiune, const char* nume)
{
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++)
	{
		suma += nume[i];
	}
	return (dimensiune > 0 ? (suma % dimensiune) : 0);
}

void inserareFisierInTabela(Hashtable ht, Fisier f)
{
	int pozitie = calculeazaHash(ht.dim, f.nume);
	adaugaFisierInLista(&(ht.vector[pozitie]),f);
}

void afisareTabela(Hashtable ht)
{
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.vector[i] != NULL)
		{
			printf("\nSunteti in clusterul %d:\n", i);
			afisareListaFisiere(ht.vector[i]);
		}
		else
		{
			printf("\nIn clusterul %d nu sunt fisiere!!\n",i);
		}
	}
}



void dezalocareLista(Nod** lista)
{
	if (*lista)
	{
		Nod* p = (*lista);
		while (p)
		{
			Nod* temp = p;
			p = p->next;
			if (temp->info.nume != NULL)
			{
				free(temp->info.nume);
			}
			if (temp->info.extensie != NULL)
			{
				free(temp->info.extensie);
			}
			if (temp->info.cale != NULL)
			{
				free(temp->info.cale);
			}
			free(temp);
		}
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
	ht->dim = 0;
	free(ht->vector);
	ht->vector = NULL;
}

Fisier cautaFisierInListaDupaNume(Nod* lista, const char* nume)
{
	Nod* p = lista;
	while (p && strcmp(p->info.nume, nume) != 0)
	{
		p = p->next;
	}
	if (p)
	{
		return p->info;
	}
	return initializare("", "", "", 0);
}

Fisier cautaFisierInTabelaDupaNume(Hashtable ht, const char* nume)
{
	int getHash = calculeazaHash(ht.dim, nume);
	Fisier f = cautaFisierInListaDupaNume(ht.vector[getHash], nume);
	return f;

}

void modificaExtensiaUnuiFisier(Hashtable ht, const char* nume, const char* extensieNoua)
{
	Fisier f = cautaFisierInTabelaDupaNume(ht, nume);
	if (strcmp(f.nume, "") != 0)
	{

		strcpy(f.extensie, extensieNoua);
		const char* caleNoua = strtok(f.cale, ".");
		strcat(caleNoua, ".");
		strcat(caleNoua, extensieNoua);
		strcpy(f.cale, caleNoua);

		const char* numeNou = strtok(f.nume, ".");
		strcat(numeNou, ".");
		strcat(numeNou, extensieNoua);
		strcpy(f.nume, numeNou);
	}
	else
	{
		printf("Fisierul nu a fost gasit!!!");
	}
}

int main()
{
	Fisier f1 = initializare("birou.txt", "C:/rebecca/Documente/birou.txt", "txt", 2);
	Fisier f2 = initializare("masini.txt", "C:/rebecca/Documente/masini.txt", "txt", 4);
	Fisier f3 = initializare("fisier.bin", "C:/rebecca/Documente/fisier.bin", "bin", 1);
	Fisier f4 = initializare("caini.jpg", "C:/rebecca/Documente/caini.jpg", "jpg", 8);

	Hashtable ht = initializareHashtable(5);
	inserareFisierInTabela(ht, f1);
	inserareFisierInTabela(ht, f2);
	inserareFisierInTabela(ht, f3);
	inserareFisierInTabela(ht, f4);
	
	afisareTabela(ht);

	Fisier fisCautat = cautaFisierInTabelaDupaNume(ht, "caini.jpg");
	printf("\nFisier cautat: \n");
	afisareFisier(fisCautat);
	
	modificaExtensiaUnuiFisier(ht, "caini.jpg", "txt");
	printf("\nTabela dupa modificare: \n");
	afisareTabela(ht);

	dezalocareTabela(&ht);

	return 0;
}