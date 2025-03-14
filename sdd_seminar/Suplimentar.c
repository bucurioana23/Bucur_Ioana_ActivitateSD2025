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

void AfisareBirou(Birou b)
{
	printf("Cod: %d\n", b.cod);
	printf("Material: %s\n", b.material);
	for (int i = 0; i < 3; i++)
	{
		printf("Dimensiunea %d: %.2f\n", i, b.dimensiuni[i]);
	}
	printf("Initiala culoare: %c\n", b.initialaCuloare);
}

int main()
{
	Birou b1 = CitireBirouTastatura();
	AfisareBirou(b1);
	return 0;
}

