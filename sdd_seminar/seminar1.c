#include<stdio.h>
#include<stdlib.h>

struct Masina {
	int id;
	int anFabricatie;
	char* sofer;
	float kmParcursi;
	char initialaProducator;
};

struct Masina initializare(int id, int anFabricatie,const char* sofer, float kmParcursi, char initialaProducator) 
{
	struct Masina m;
	m.id = id;
	m.anFabricatie = anFabricatie;
	m.sofer = (char*)malloc(strlen(sofer) + 1); //malloc primeste ca param dimensiunea in bytes tb numarul de el*sizeof(ce e)
	strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
	m.kmParcursi = kmParcursi;
	m.initialaProducator = initialaProducator;
	return m;
}

void afisare(struct Masina m) {
	printf("ID: %d \n", m.id);
	printf("An fabricatie: %d \n", m.anFabricatie);
	printf("Sofer: %s \n", m.sofer);
	printf("km parcursi: %f \n", m.kmParcursi);
	printf("initiala: %c \n", m.initialaProducator);
}

void modifica_sofer(struct Masina* m, const char* nouSofer) {
	if (strlen(nouSofer) > 2)
	{
		if (m->sofer != NULL)
		{
			free(m->sofer);
		}
		m->sofer = (char*)malloc(strlen(nouSofer) + 1);
		strcpy_s(m->sofer, strlen(nouSofer) + 1, nouSofer);
	}
}

void dezalocare(struct Masina* m) //daca nu e * inseamna ca e o vb memorata pe stack si programatorul nu are acces acolo 
{
	if (m->sofer != NULL)
	{
		free(m->sofer);
		m->sofer = NULL;
	}
}

int main() 
{
	struct Masina masina;
	masina = initializare(1, 2004, "Robert", 2000, 'P'); 
	afisare(masina);
	modifica_sofer(&masina, "Alex");
	printf("\n");
	afisare(masina);
	printf("\n");
	dezalocare(&masina);
	afisare(masina);

	return 0;
}