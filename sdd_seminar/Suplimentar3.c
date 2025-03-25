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

int main()
{
	Birou* birouri = NULL;
	int nrBirouri = 0;
	birouri = CitireVectorBirouriDinFisier("birouri.txt", &nrBirouri);
	AfisareVectorBirouri(birouri, nrBirouri);

	return 0;
}