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
	return 0;
}

