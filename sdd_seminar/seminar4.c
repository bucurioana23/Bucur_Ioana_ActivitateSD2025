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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
	Masina info;
	struct Nod* next;
};
typedef struct Nod Nod;

Masina initializare(int id, int nrUsi, float pret, const char* model, const char* numeSofer, unsigned char serie) {
	Masina m;
	m.id = id;
	m.nrUsi = nrUsi;
	m.pret = pret;
	if (model != NULL)
	{
		m.model = (char*)malloc(strlen(model) + 1);
		strcpy_s(m.model, strlen(model) + 1, model);
	}
	else
	{
		m.model = NULL;
	}

	if (numeSofer != NULL)
	{
		m.numeSofer = (char*)malloc(strlen(numeSofer) + 1);
		strcpy_s(m.numeSofer, strlen(numeSofer) + 1, numeSofer);
	}
	else
	{
		m.numeSofer = NULL;
	}
		
	m.serie = serie;
	return m;
}

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

	m1.serie = strtok(NULL, sep)[0];
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

void afisareListaMasini(Nod* nod) {
	while (nod)
	{
		afisareMasina(nod->info); // -> pt ca nod e pointer si trebuie si dereferntiat si punct
		nod = nod->next;
	}
}

void adaugaMasinaInLista(Masina masinaNoua, Nod** lista) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;

	if ((*lista) == NULL)
	{
		(*lista) = nodNou;
	}
	else
	{
		Nod* aux = (*lista);
		while (aux->next)
		{
			aux = aux->next;
		}
		aux->next = nodNou;
	}
	
}

void adaugaLaInceputInLista(Nod** lista, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	Nod* nodNou = malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = (*lista);
	(*lista) = nodNou;

}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	
	FILE* f = fopen(numeFisier, "r");
	Nod* lista = NULL;
	while (!feof(f))
	{
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInLista(m, &lista);
	}
	fclose(f);
	return lista;
}

void dezalocareListaMasini(Nod** lista) {
	
	while ((*lista) != NULL)
	{
		Nod* aux = (*lista);
		free(aux->info.model);
		free(aux->info.numeSofer);
		(*lista) = (*lista)->next;
		free(aux);
	}
}

float calculeazaPretMediu(Nod* lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float suma = 0;
	int contor = 0;
	while (lista)
	{
		suma += lista->info.pret;
		contor++;
		lista = lista->next;
	}

	if (contor == 0)
	{
		return 0;
	}

	return suma/contor;
}

int getNrUsiMasinaScumpa(Nod* lista)
{
	if (lista)
	{
		int nrUsi = lista->info.nrUsi;
		float pretMaxim = lista->info.pret;
		lista = lista->next;
		while (lista)
		{
			if (lista->info.pret > pretMaxim)
			{
				nrUsi = lista->info.nrUsi;
				pretMaxim = lista->info.pret;
			}
			lista = lista->next;
		}
		return nrUsi;
	}
	return 0;
}

void stergeMasiniDinSeria(Nod** lista, char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	while ((*lista) && (serieCautata == (*lista)->info.serie))
	{
		Nod* aux = (*lista);
		*lista = (*lista)->next;
		if (aux->info.numeSofer)
		{
			free(aux->info.numeSofer);
		}
		if (aux->info.model)
		{
			free(aux->info.model);
		}
		free(aux);
	}

	if ((*lista))
	{
		Nod* p = (*lista);
		while (p)
		{
			while (p->next && p->next->info.serie != serieCautata)
			{
				p = p->next;
			}
			if (p->next)
			{
				Nod* aux = p->next;
				p->next = aux->next;
				if (aux->info.numeSofer)
				{
					free(aux->info.numeSofer);
				}
				if (aux->info.model)
				{
					free(aux->info.model);
				}
			}
			else
			{
				p = NULL;
			}
		}
	}
	
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) 
{
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	int contor = 0;
	while (lista)
	{
		if (strcmp(lista->info.numeSofer, numeSofer) == 0)
		{
			suma += lista->info.pret;
		}
		lista = lista->next;
	}

	return suma;
}

int main() 
{
	Nod* lista = citireListaMasiniDinFisier("masini.txt");
	printf("Lista initiala : \n");
	afisareListaMasini(lista);

	Masina masinaNoua = initializare(0,4,12000,"Tucson","Florin",'H');
	adaugaLaInceputInLista(&lista, masinaNoua);
	printf("Lista dupa modificare: \n");
	afisareListaMasini(lista);

	printf("Lista fara seria A: \n");
	stergeMasiniDinSeria(&lista, 'A');
	afisareListaMasini(lista);

	printf("Lista fara seria B: \n");
	stergeMasiniDinSeria(&lista, 'B');
	afisareListaMasini(lista);

	//float medie = calculeazaPretMediu(lista);
	//printf("Media este: %.2f\n", medie);
	//char* nume = malloc(strlen("Ionescu") + 1);
	//strcpy_s(nume, strlen("Ionescu") + 1, "Ionescu");
	//float sumaSofer = calculeazaPretulMasinilorUnuiSofer(lista, nume);
	//printf("Pretul masinilor lui %s este: %.2f\n",nume, sumaSofer);
	////int nrUsi = getNrUsiMasinaScumpa(lista);
	//printf("Nr usi masina cea mai scumpa: %d", nrUsi);
	dezalocareListaMasini(&lista);

	return 0;
}