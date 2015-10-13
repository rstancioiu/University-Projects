#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#define MAX_FACTORS 65
#define MOD 8

//les nombres qu'on doit calculer pour chaque thread
uint64_t n1,n2,n3,n4;
//fichier de lecture
FILE* input;
//mutex de lecture
pthread_mutex_t mutexLecture;
//mutex d'affichage 
pthread_mutex_t mutexAffichage;

int tab[MOD]={2,6,5,2,4,2,4,6};
int tabPrimes[10]={2,3,5,7,11,13,17,19,23,29};


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
	int k=0;
	for(j=0;j<9;++j)
	{
		if(n%tabPrimes[j]==0)
		{
			n=n/tabPrimes[j];
			dest[k++]=tabPrimes[j];
			while(n%tabPrimes[j]==0)
			{
				n=n/tabPrimes[j];
				dest[k++]=tabPrimes[j];
			}
		}
	}
	i=29;
	j=0;
	printf("%llu\n",n);
	racine = compute_sqrt(n);
	for(;i<=racine;i+=tab[j++],j&=8)
	{
		if(n%i==0)
		{
			n=n/i;
			dest[k++]=i;
			while(n%i==0)
			{
				n=n/i;
				dest[k++]=i;
			}
			racine = compute_sqrt(n);
		}
	}
	if(n!=1)
		dest[k++]=n;
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
	printf("%llu:",n);
	
	for(j=0; j<k; ++j)
	{
		printf(" %llu",factors[j]);
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
		if(fscanf(input,"%llu",&n1)==1)
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
		if(fscanf(input,"%llu",&n2)==1)
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
		if(fscanf(input,"%llu",&n3)==1)
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
		if(fscanf(input,"%llu",&n4)==1)
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
	input = fopen("nombre.txt","r");
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
