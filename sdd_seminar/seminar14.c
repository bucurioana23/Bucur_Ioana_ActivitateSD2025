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


typedef struct NodLP NodLP;
typedef struct NodLS NodLS;

struct NodLP {
	Masina info;
	NodLP* next;
	NodLS* vecini;
};

struct NodLS {
	NodLS* next;
	NodLP* nodInfo;
};

//2.
//functii de inserare in liste
//si in principala si in secundara

void inserareLP(NodLP** cap, Masina m)
{
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->next = NULL;
	nou->info = m; //shallow copy
	nou->vecini = NULL;

	if (*cap)
	{
		NodLP* p = (*cap);
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		(*cap) = nou;
	}

}

void inserareLS(NodLS** cap, NodLP* nodInfo)
{
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->next = (*cap);
	nou->nodInfo = nodInfo;
	(*cap) = nou;
}

//3.
//functie de cautarea in lista principala dupa ID
NodLP* cautaNodDupaID(NodLP* cap, int id) {
	while (cap && cap->info.id != id)
	{
		cap = cap->next;
	}
	return cap;
}

//4.
//inserare muchie
void inserareMuchie(NodLP* cap, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(cap, idStart);
	NodLP* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop)
	{
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}


NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLP* graf = NULL;
	if (f) {
		while (!feof(f)) {
			Masina m = citireMasinaDinFisier(f);
			inserareLP(&graf, m);
		}
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodLP* graf) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f))
		{
			int idStart;
			int idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
	}
	fclose(f);
}

void stergereLS(NodLS** cap) {
	while (*cap) {
		NodLS* aux = (*cap)->next;
		free(*cap);
		(*cap) = aux;
	}
}

void dezalocareNoduriGraf(NodLP** cap) {
	while (*cap) {
		NodLP* aux = (*cap)->next;
		stergereLS(&((*cap)->vecini));
		if ((*cap)->info.numeSofer)
		{
			free((*cap)->info.numeSofer);
		}
		if ((*cap)->info.model)
		{
			free((*cap)->info.model);
		}
		free(*cap);
		(*cap) = aux;
	}
}



NodLP* citireGrafDinFisiere(const char* fisierNoduri, const char* fisierMUchii)
{
	NodLP* graf = citireNoduriMasiniDinFisier(fisierNoduri);
	citireMuchiiDinFisier(fisierMUchii, graf);
	return graf;
}

void afisareGraf(NodLP* graf) {
	while (graf) {
		printf("In nodul %d avem masina:\n", graf->info.id);
		afisareMasina(graf->info);
		printf("Aceasta are ca vecini nodurile urmatoare:\n");
		NodLS* temp = graf->vecini;
		while (temp) {
			printf("%d, ", temp->nodInfo->info.id);
			temp = temp->next;
		}
		printf("\n\n");
		graf = graf->next;
	}
}

//Parcurgere in adancime

//creare structura Stiva
//push si pop pentru stiva



struct NodStiva {
	int id;
	struct NodStiva* next;
};

typedef struct NodStiva NodStiva;

void push(NodStiva** cap, int id)
{
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = *cap;
	(*cap) = nou;
}

int pop(NodStiva** cap)
{
	if (*cap) {
		NodStiva* aux = (*cap);
		(*cap) = aux->next;
		int id = aux->id;
		free(aux);
		return id;
	}
	return -1;
}

int calculeazaNrNoduriGraf(NodLP* listaPrincipala) {
	int nrNoduri = 0;
	while (listaPrincipala)
	{
		nrNoduri++;
		listaPrincipala = listaPrincipala->next;
	}
	return nrNoduri;
}

void afisareGrafInAdancime(NodLP* graf, int idPlecare) {
	/*
	1. Este luata o STIVA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in STIVA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in STIVA
	4.1 Extragem un nod din STIVA
	4.2 Procesam nodul extras din STIVA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in STIVA
	4.4 reluam procesul
	*/
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	int* vizitate = (int*)malloc(sizeof(int) * nrNoduri);
	NodStiva* stiva = NULL;
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}

	vizitate[idPlecare-1] = 1;
	push(&stiva, idPlecare);

	while (stiva)
	{
		int id = pop(&stiva);
		NodLP* nod = cautaNodDupaID(graf, id);
		afisareMasina(nod->info);
		NodLS* aux = nod->vecini;
		while (aux)
		{
			if (vizitate[aux->nodInfo->info.id - 1] == 0)
			{
				vizitate[aux->nodInfo->info.id - 1] = 1;
				push(&stiva, aux->nodInfo->info.id);
			}
			aux = aux->next;
		}
	}

}


//Parcurgere in latime

//creare structura coada
//enqueue si dequeue pentru coada

typedef struct NodStiva NodCoada;

void enqueue(NodCoada** cap, int id)
{
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->next = NULL;
	nou->id = id;
	if (*cap)
	{
		NodCoada* p = (*cap);
		while (p->next)
		{
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

int dequeue(NodCoada** cap) {
	return pop(cap);
}

void afisareGrafInLatime(NodLP* graf, int idPlecare) {
	/*
	1. Este luata o COADA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in COADA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in COADA
	4.1 Extragem un nod din stiva
	4.2 Procesam nodul extras din COADA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in COADA
	4.4 reluam procesul
	*/

	int nrNoduri = calculeazaNrNoduriGraf(graf);
	int* vizitate = (int*)malloc(sizeof(int) * nrNoduri);
	NodCoada* coada = NULL;
	for (int i = 0; i < nrNoduri; i++)
	{
		vizitate[i] = 0;
	}

	vizitate[idPlecare - 1] = 1;
	enqueue(&coada, idPlecare);

	while (coada)
	{
		int id = dequeue(&coada);
		NodLP* nod = cautaNodDupaID(graf, id);
		afisareMasina(nod->info);
		NodLS* aux = nod->vecini;
		while (aux)
		{
			if (vizitate[aux->nodInfo->info.id - 1] == 0)
			{
				vizitate[aux->nodInfo->info.id - 1] = 1;
				enqueue(&coada, aux->nodInfo->info.id);
			}
			aux = aux->next;
		}
	}
}


int main() {
	NodLP* graf = citireGrafDinFisiere("masini.txt", "muchii.txt");
	afisareGrafInAdancime(graf, 1);
	printf("\n\nAfisare in latime: \n\n");
	afisareGrafInLatime(graf, 1);


	dezalocareNoduriGraf(&graf);

	return 0;
}