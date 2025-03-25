#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

//Bucur Ioana -> articolul Birou
struct StructuraBirou {
	int cod;
	char* material;
	float dimensiuni[3];
	char initialaCuloare;
};
typedef struct StructuraBirou Birou;

void AfisareBirou(Birou b)
{
	printf("BIROUL CERUT: \n");
	printf("Cod: %d\n", b.cod);
	printf("Material: %s\n", b.material);
	for (int i = 0; i < 3; i++)
	{
		printf("Dimensiunea %d: %.2f\n", i, b.dimensiuni[i]);
	}
	printf("Initiala culoare: %c\n", b.initialaCuloare);
}

void AfisareVectorBirouri(Birou* vector, int numarBirouri)
{
	if (vector != NULL && numarBirouri > 0)
	{
		for (int i = 0; i < numarBirouri; i++)
		{
			AfisareBirou(vector[i]);
			printf("\n");
		}
	}
}

void AdaugaBirouInVector(Birou** birouri, int* nrBirouri, Birou birouNou)
{
	Birou* aux = malloc(sizeof(Birou) * ((*nrBirouri) + 1));
	for (int i = 0; i < (*nrBirouri); i++)
	{
		aux[i] = (*birouri)[i];
	}
	aux[(*nrBirouri)] = birouNou;
	free(*birouri);
	(*birouri) = aux;
	(*nrBirouri)++;
}

Birou CitireBirouDinFisier(FILE* fisier)
{
	Birou b;
	char buffer[100];
	char sep[4] = ",;\n";
	fgets(buffer, 100, fisier);
	b.cod = atoi(strtok(buffer, sep));
	char* aux;
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

Birou* CitireVectorBirouriDinFisier(const char* numeFisier, int* nrBirouriCitite)
{
	FILE* f = fopen(numeFisier, "r");
	Birou* birouri = NULL;
	while (!feof(f))
	{
		AdaugaBirouInVector(&birouri, nrBirouriCitite, CitireBirouDinFisier(f));
	}
	fclose(f);
	return birouri;
}


//functie de formare matrice dinamica le linii in functie de material
Birou** MatriceInFunctieDeMaterial(Birou** matrice, int numarLinii, int* numarColoane, Birou* birouri, int nrBirouri)
{
	int numarElLemn = 0, numarElMetal = 0, numarElPlastic = 0;
	for (int i = 0; i < nrBirouri; i++)
	{
		if (strcmp(birouri[i].material, "lemn") == 0)
		{
			numarElLemn++;
		}
		else if (strcmp(birouri[i].material, "metal") == 0)
		{
			numarElMetal++;
		}
		else
		{
			numarElPlastic++;
		}
	}

	numarColoane[0] = numarElLemn;
	numarColoane[1] = numarElMetal;
	numarColoane[2] = numarElPlastic;

	matrice[0] = malloc(sizeof(Birou) * numarColoane[0]);
	matrice[1] = malloc(sizeof(Birou) * numarColoane[1]);
	matrice[2] = malloc(sizeof(Birou) * numarColoane[2]);

	int k = 0,j=0,l=0;
	for (int i = 0; i < nrBirouri; i++)
	{
		if (strcmp(birouri[i].material, "lemn") == 0)
		{
			matrice[0][k] = birouri[i];
			matrice[0][k].material = malloc(strlen(birouri[i].material) + 1);
			strcpy(matrice[0][k].material, birouri[i].material);
			k++;
		}
		else if (strcmp(birouri[i].material, "metal") == 0)
		{
			matrice[1][j] = birouri[i];
			matrice[1][j].material = malloc(strlen(birouri[i].material) + 1);
			strcpy(matrice[1][j].material, birouri[i].material);
			j++;
		}
		else
		{
			matrice[2][l] = birouri[i];
			matrice[2][l].material = malloc(strlen(birouri[i].material) + 1);
			strcpy(matrice[2][l].material, birouri[i].material);
			l++;
		}
	}

	return matrice;
}

void AfisareMatrice(Birou** matrice, int nr_linii, int* nr_coloane)
{
	for (int i = 0; i < nr_linii; i++)
	{
		printf("Linia %d a matricei: \n", i);
		AfisareVectorBirouri(matrice[i], nr_coloane[i]);
		printf("\n");
		
	}
}


int main()
{
	Birou* birouri = NULL;
	int nrBirouri = 0;
	birouri = CitireVectorBirouriDinFisier("birouri.txt", &nrBirouri);
	AfisareVectorBirouri(birouri, nrBirouri);

	int nr_linii = 3;
	int* nr_coloane = malloc(sizeof(int) * nr_linii);
	Birou** matrice = malloc(sizeof(Birou*) * nr_linii);
	for (int i = 0; i < nr_linii; i++)
	{
		matrice[i] = NULL;
		nr_coloane[i] = 0;
	}

	MatriceInFunctieDeMaterial(matrice, nr_linii, nr_coloane, birouri, nrBirouri);
	AfisareMatrice(matrice, nr_linii, nr_coloane);

	return 0;
}