#include <stdio.h>
#include <malloc.h>
#include <string.h>

//Bucur Ioana -> articolul Birou
struct StructuraBirou {
	int cod;
	char* material;
	float dimensiuni[3];
	char initialaCuloare;
};
typedef struct StructuraBirou Birou;

Birou CitireBirouTastatura()
{
	Birou b;

	printf("Introdu codul biroului de tip int: ");
	scanf_s("%d", &b.cod);

	char* aux = malloc(40);
	printf("Introdu materialul: ");
	scanf_s("%39s", aux,(unsigned int)40);
	b.material = malloc(strlen(aux) + 1);
	strcpy_s(b.material, strlen(aux) + 1, aux);

	printf("Introdu dimensiunile: ");
	for (int i = 0; i < 3; i++)
	{
		printf("%d-> ", i);
		scanf_s("%f", &b.dimensiuni[i]);
	}

	printf("Introdu initiala culorii: ");
	scanf_s(" %c", &b.initialaCuloare,1);

	return b;
}

Birou initializare(int cod, const char* material, float dimensiuni[3], char initialaCuloare)
{
	Birou b;
	b.cod = cod;
	if (material != NULL)
	{
		b.material = malloc(strlen(material) + 1);
		strcpy_s(b.material, strlen(material) + 1, material);
	}
	else
	{
		b.material = NULL;
	}
	for (int i = 0; i < 3; i++)
	{
		b.dimensiuni[i] = dimensiuni[i];
	}
	b.initialaCuloare = initialaCuloare;
	return b;
}

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

void modificaMaterial(Birou* b, const char* material)
{
	if (strlen(material)>=3)
	{
		if (b->material != NULL)
		{
			free(b->material);
		}
		b->material = malloc(strlen(material) + 1);
		strcpy_s(b->material, strlen(material) + 1, material);
	}
}

float calculeazaArieBlat(Birou b)
{
	float arieBlat = 1;
	for (int i = 0; i < 2; i++)
	{
		arieBlat *= b.dimensiuni[i];
	}

	return arieBlat;
}


void copiazaBirouriMaterialulCerutCuVectorDat(Birou* vector, int numarBirouri, const char* materialCerut, Birou** vectorNou, int* dimensiune)
{
	if (vector != NULL && numarBirouri > 0)
	{
		(*dimensiune) = 0;
		for (int i = 0; i < numarBirouri; i++)
		{
			if (strcmp(vector[i].material, materialCerut) == 0)
			{
				(*dimensiune)++;
			}
		}

		(*vectorNou) = malloc(sizeof(Birou) * (*dimensiune));

		int k = 0;
		for (int i = 0; i < numarBirouri; i++)
		{
			if (strcmp(vector[i].material, materialCerut) == 0)
			{
				(*vectorNou)[k] = vector[i];
				(*vectorNou)[k].material = malloc(strlen(vector[i].material) + 1);
				strcpy_s((*vectorNou)[k].material, strlen(vector[i].material) + 1, vector[i].material);
				k++;
			}
		}
		
	}
}

//functia face acelasi lucru ca cea de mai sus doar ca aceasta creeaza un vector nou
Birou* copiazaBirouriMaterialulCerut(Birou* vector, int numarBirouri, const char* materialCerut, int* dimensiune)
{
	if (vector != NULL && numarBirouri > 0)
	{
		(*dimensiune) = 0;
		for (int i = 0; i < numarBirouri; i++)
		{
			if (strcmp(vector[i].material, materialCerut) == 0)
			{
				(*dimensiune)++;
			}
		}

		Birou* vectorNou = malloc(sizeof(Birou) * (*dimensiune));

		int k = 0;
		for (int i = 0; i < numarBirouri; i++)
		{
			if (strcmp(vector[i].material, materialCerut) == 0)
			{
				vectorNou[k] = vector[i];
				vectorNou[k].material = malloc(strlen(vector[i].material) + 1);
				strcpy_s(vectorNou[k].material, strlen(vector[i].material) + 1, vector[i].material);
				k++;
			}
		}
		
		return vectorNou;
	}
	return NULL;
}

void mutaBirouriDupaInitialaCuloare(Birou** vector, int* nrBirouri, char initialaCuloare,Birou** vectorNou, int* nrBirouriNou)
{
	if ((*vector) != NULL && (*nrBirouri) > 0)
	{
		if ((*vectorNou) != NULL)
		{
			free(*vectorNou);
		}
		(*nrBirouriNou) = 0;

		for (int i = 0; i < *nrBirouri; i++)
		{
			if ((*vector)[i].initialaCuloare == initialaCuloare)
			{
				(*nrBirouriNou)++;
			}
		}

		if ((*nrBirouriNou) > 0)
		{
			int numarVIM = (*nrBirouri) - (*nrBirouriNou);
			Birou* vectorInitialMutat = malloc(sizeof(Birou) * numarVIM);
			*vectorNou = malloc(sizeof(Birou) * (*nrBirouriNou));

			int k = 0, j = 0;
			for (int i = 0; i < *nrBirouri; i++)
			{
				if ((*vector)[i].initialaCuloare == initialaCuloare)
				{
					(*vectorNou)[k] = (*vector)[i];
					k++;
				}
				else
				{
					vectorInitialMutat[j] = (*vector)[i];
					j++;
				}
			}
			
			free(*vector);
			*vector = vectorInitialMutat;
			(*nrBirouri) = numarVIM;
		}
		else
		{
			(*vectorNou) = NULL;
		}
	}
}

int main()
{
	//Birou b1 = CitireBirouTastatura();
	char* material = malloc(strlen("lemn") + 1);
	strcpy_s(material, strlen("lemn") + 1, "lemn");
	float dimensiuni[3] = { 120,30,70 };
	Birou b1 = initializare(12, material, dimensiuni, 'a');
	AfisareBirou(b1);
	float arieBlat = calculeazaArieBlat(b1);
	printf(" Aria blatului pt b1: %.2f", arieBlat);

	char* materialNou = malloc(strlen("metal") + 1);
	strcpy_s(materialNou, strlen("metal") + 1, "metal");
	modificaMaterial(&b1, materialNou);
	AfisareBirou(b1);
	Birou b2 = initializare(13,material,dimensiuni,'n');
	Birou b3 = initializare(14,materialNou,dimensiuni,'m');
	Birou b4 = initializare(15,material,dimensiuni,'b');
	Birou b5 = initializare(16,materialNou,dimensiuni,'a');
	Birou b6 = initializare(17,materialNou,dimensiuni,'n');
	
	int numarElemente = 6;
	Birou* vectorBirouri = malloc(sizeof(Birou) * numarElemente);
	vectorBirouri[0] = b1;
	vectorBirouri[1] = b2;
	vectorBirouri[2] = b3;
	vectorBirouri[3] = b4;
	vectorBirouri[4] = b5;
	vectorBirouri[5] = b6;

	printf("\n Afisare vector birouri: \n");
	AfisareVectorBirouri(vectorBirouri,numarElemente);

	
	/*int numarElementeVN1;
	Birou* vectorNou1 = copiazaBirouriMaterialulCerut(vectorBirouri, numarElemente, material, &numarElementeVN1);
	printf("\nVectorul nou de birouri: \n");
	AfisareVectorBirouri(vectorNou1, numarElementeVN1);*/

	int numarElementeVN2;
	Birou* vectorNou2 = NULL;
	mutaBirouriDupaInitialaCuloare(&vectorBirouri, &numarElemente, 'n', &vectorNou2, &numarElementeVN2);
	printf("\n Afisare vector birouri initial dupa mutare care are numarul de elemente actual %d: ",numarElemente);
	AfisareVectorBirouri(vectorBirouri, numarElemente);
	printf("\n Afisare vector birouri nou dupa mutare care are numarul de elemente actual %d: ", numarElementeVN2);
	AfisareVectorBirouri(vectorNou2, numarElementeVN2);

	return 0;
}

