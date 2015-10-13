#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#define MAX_FACTORS 65
#define DICT_MAX_SZ 1000
#define MOD 8

//les nombres qu'on doit calculer pour chaque thread
uint64_t n1,n2,n3,n4;
//fichier de lecture
//FILE* input;
//mutex de lecture
pthread_mutex_t mutexLecture;
//mutex d'affichage 
pthread_mutex_t mutexAffichage;

int tab[MOD]={2,6,5,2,4,2,4,6};
int tabPrimes[10]={2,3,5,7,11,13,17,19,23,29};


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
 * Procedure qui compute sqrt d'un nombre en log(n) 
 */
uint64_t compute_sqrt(uint64_t n) {
    uint64_t first=0;
    uint64_t step=1<<31;
    for(;step;step>>=1)
    {
        uint64_t index=first+step;
        if(index*index<=n)
        {
            first=index;   
        }
    }
    return first;
}


/*
 * Procedure qui calcule les facteurs premiers d'un nombre sur 64 bits 
 */
int get_prime_factors(uint64_t n, uint64_t* dest)
{
	uint64_t i,j,racine;
	int found=0;
	int dictInd = -1;
	int k=0;
	if(n==1)
	{
		dest[k++]=1;
		return k;
	}
	uint64_t N = n; // Mémorise le chiffre pour ajout de l'entrée dans le dictionnaire
	if( (dictInd = RecupererFacteurs(n)) != -1)	 
	{ 
		unsigned int i;	
		int sz = dictionnaire[dictInd].taille;	
		for(i = 0 ; i < sz ; i++)
		{	
			dest[k++] = dictionnaire[dictInd].facteurs[i];
		}
		return k;
	}		
	for(j=0;j<9 && !found;++j)
	{
		if(n%tabPrimes[j]==0)
		{
			n=n/tabPrimes[j];
			dest[k++]=tabPrimes[j];
			while(n%tabPrimes[j]==0)
			{
			if( (dictInd = RecupererFacteurs(n)) != -1)	 
				{ found=1; break;}				
				n=n/tabPrimes[j];
				dest[k++]=tabPrimes[j];
			}
			if( (dictInd = RecupererFacteurs(n)) != -1)	 
				{ found=1; break;}
		}
	}
	i=29;
	j=0;
	racine = compute_sqrt(n);
	for(;i<=racine && !found;i+=tab[j++],j&=8)
	{
		if(n%i==0)
		{
			n=n/i;
			dest[k++]=i;			
			while(n%i==0)
			{
			if( (dictInd = RecupererFacteurs(n)) != -1)	 
				{ found=1; break;}	
				n=n/i;
				dest[k++]=i;
			}
			if( (dictInd = RecupererFacteurs(n)) != -1)	 
				{ found=1; break;}
			racine = compute_sqrt(n);				
		}
	}
	if(n!=1 && !found)
		dest[k++]=n;
	if(!found)
	{
		AjouterFacteurs(N, dest, k);
	}
	else
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
 * Procedure du premier thread 
 */
void print_prime_factors_t2(void)
{
	print_prime_factors(n2);
}

/*
 * Procedure du premier thread 
 */
void print_prime_factors_t3(void)
{
	print_prime_factors(n3);
}

/*
 * Procedure du premier thread 
 */
void print_prime_factors_t4(void)
{
	print_prime_factors(n4);
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
 * Procedure qui declanche le deuxieme thread
 */ 
void Compute_Simul_t3(void)
{
	while(1)
	{
		pthread_mutex_lock(&mutexLecture);
		if(scanf("%" SCNu64,&n3)==1)
		{
			pthread_mutex_unlock(&mutexLecture);
			print_prime_factors_t3();

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
void Compute_Simul_t4(void)
{
	while(1)
	{
		pthread_mutex_lock(&mutexLecture);
		if(scanf("%" SCNu64,&n4)==1)
		{
			pthread_mutex_unlock(&mutexLecture);
			print_prime_factors_t4();

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
	pthread_t t1,t2,t3,t4;
	//freopen("test.out","w",stdout);
	//input = fopen("test.in","r");
	pthread_mutex_init(&mutexLecture,NULL);
	pthread_mutex_init(&mutexAffichage,NULL);
	pthread_create(&t1,NULL,(void*)	Compute_Simul_t1,NULL);
	pthread_create(&t2,NULL,(void*) Compute_Simul_t2,NULL);
	pthread_create(&t3,NULL,(void*)	Compute_Simul_t3,NULL);
	pthread_create(&t4,NULL,(void*) Compute_Simul_t4,NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_join(t3, NULL);
	pthread_join(t4, NULL);
	pthread_mutex_destroy(&mutexLecture);
	pthread_mutex_destroy(&mutexAffichage);
	return 0;
}
