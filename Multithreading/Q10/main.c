#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#define MAX_FACTORS 65
#define DICT_MAX_SZ 1000

//les nombres qu'on doit calculer pour chaque thread
static uint64_t n1,n2;
//fichier de lecture
//static FILE* input;
//mutex de lecture
static pthread_mutex_t mutexLecture;
//mutex d'affichage 
static pthread_mutex_t mutexAffichage;

typedef struct Factorisation{
	uint64_t nombre;
	int taille;
	uint64_t facteurs[MAX_FACTORS];
} Factors;

// mutex de protection dictionnaire
static pthread_mutex_t mutexDict;
// dictionnaire des factorisations réalisées précédemment
static Factors dictionnaire[DICT_MAX_SZ];
// Indice du prochain slot disponible
static unsigned int slotLibre = 0;

/*
 * Retourne l'index du nombre dans le dictionnaire ou -1 si le nombre n'existe pas dans le dictionnaire 
 * Complexité : O(n)
 */
int RecupererFacteurs(uint64_t nombre)
{	int i;	
	pthread_mutex_lock(&mutexDict);
	for(i=0 ; i < slotLibre ; i++)
	{	if(dictionnaire[i].nombre == nombre)
		{	
			pthread_mutex_unlock(&mutexDict);
			return i;
		}
	}
	pthread_mutex_unlock(&mutexDict);
	return -1;
}

/*
 * Ajoute un nombre et ses facteurs au dictionnaire
 * Complexité : O(n)
 */
void AjouterFacteurs(uint64_t nombre, const uint64_t * facteurs, int taille)
{	unsigned int thisSlot;	
	pthread_mutex_lock(&mutexDict);
	thisSlot = slotLibre++;
	dictionnaire[thisSlot].nombre = nombre;
	unsigned int i;	
	for(i=0 ; i < taille ; i++) 
	{	dictionnaire[thisSlot].facteurs[i] = facteurs[i];
	}
	dictionnaire[thisSlot].taille = taille;
	pthread_mutex_unlock(&mutexDict);
}

/*
 * Procedure qui calcule les facteurs premiers d'un nombre sur 64 bits 
 */
int get_prime_factors(uint64_t n, uint64_t* dest)
{
	uint64_t i;
	int k = 0;
	int dictInd = -1;
	int found = 0; // bool
	uint64_t N = n; // Mémorise le chiffre pour ajout de l'entrée dans le dictionnaire
	if( (dictInd = RecupererFacteurs(n)) != -1) 
	{ 
		found=1; 
	}
	if(n == 1)
	{ dest[k++]=1; 
		return k;
	} 
	else if(!found)
	{
		if(n%2==0)
		{
			n=n/2;
			dest[k++]=2;
			while(n%2==0)
			{	
				if( (dictInd = RecupererFacteurs(n)) != -1) 
				{ found=1; break; }
				n=n/2;
				dest[k++]=2;
			}
			if( (dictInd = RecupererFacteurs(n)) != -1) 
				{ found=1;}
		}
	// Si on n'a pas trouvé d'entrée dans le dictionnaire on effectue le traitement qui suit	
		if(!found)
		{
			for(i=3;i<=n && !found ;i+=2)
			{
				if(n%i==0)
				{
					n=n/i;
					dest[k++]=i;
						
					while(n%i==0)
					{	
						if( (dictInd = RecupererFacteurs(n)) != -1)	 
						{ found=1; break; }
						n=n/i;
						dest[k++]=i;
					}
					if( (dictInd = RecupererFacteurs(n)) != -1)	 
					{ found=1; }
				}
			}
		}
		if(!found)
		{
			AjouterFacteurs(N, dest, k);
		}
	}
	// Si on a trouvé on va chercher le reste des facteurs dans le dictionnaire
	if(found == 1)
	{	
		unsigned int i;	
		int sz = dictionnaire[dictInd].taille;	
		for(i = 0 ; i < sz ; i++)
		{	
			dest[k++] = dictionnaire[dictInd].facteurs[i];
		}
		// On inscrit le nombre dans le dictionnaire et donc sa factorisation associée
		AjouterFacteurs(N, dest, k);
	}
	return k;
}

/*
 * Procedure qui affiche les facteurs premiers d'un nombre sur 64 bits 
 */
void print_prime_factors(uint64_t n)
{
	uint64_t factors[MAX_FACTORS];
	int j,k;
	k = get_prime_factors(n,factors);
	pthread_mutex_lock(&mutexAffichage);
	printf("%" SCNu64":",n);
	
	for(j=0; j<k; ++j)
	{
		printf(" %" SCNu64,factors[j]);
	}
	printf("\n");
	pthread_mutex_unlock(&mutexAffichage);
}

/*
 * Procedure du premier thread 
 */
void print_prime_factors_t1(void)
{
	print_prime_factors(n1);
}

/*
 * Procedure du deuxieme thread 
 */
void print_prime_factors_t2(void)
{
	print_prime_factors(n2);
}

/*
 * Procedure qui declanche le premier thread
 */ 
void Compute_Simul_t1(void)
{
	
	while(1)
	{
		pthread_mutex_lock(&mutexLecture);
		if(scanf("%" SCNu64,&n1)==1)
		{
			pthread_mutex_unlock(&mutexLecture);
			print_prime_factors_t1();
		}
		else 
		{
			pthread_mutex_unlock(&mutexLecture);
			break;
		}
	}
}

/*
 * Procedure qui declanche le deuxieme thread
 */ 
void Compute_Simul_t2(void)
{
	while(1)
	{
		pthread_mutex_lock(&mutexLecture);
		if(scanf("%" SCNu64,&n2)==1)
		{
			pthread_mutex_unlock(&mutexLecture);
			print_prime_factors_t2();

		}
		else
		{
			pthread_mutex_unlock(&mutexLecture);
			break;
		}
	}
}

/*
 * Procedure Main du fichier source
 */ 
int main()
{
	//int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);
	//int pthread_join(pthread_t thread, void **retval);
	//int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
	//freopen("test.in","r",stdin);
	pthread_t t1,t2;
	//freopen("test.out","w",stdout);
	//input = fopen("test.in","r");
	pthread_mutex_init(&mutexLecture,NULL);
	pthread_mutex_init(&mutexAffichage,NULL);
	pthread_mutex_init(&mutexDict, NULL);
	pthread_create(&t1,NULL,(void*)	Compute_Simul_t1,NULL);
	pthread_create(&t2,NULL,(void*) Compute_Simul_t2,NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&mutexLecture);
	pthread_mutex_destroy(&mutexAffichage);
	return 0;
}
