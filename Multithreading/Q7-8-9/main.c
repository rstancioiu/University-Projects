#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#define MAX_FACTORS 65

//les nombres qu'on doit calculer pour chaque thread
uint64_t n1,n2;
//fichier de lecture
FILE* input;
//mutex de lecture
pthread_mutex_t mutexLecture;
//mutex d'affichage 
pthread_mutex_t mutexAffichage;


/*
 * Procedure qui calcule les facteurs premiers d'un nombre sur 64 bits 
 */
int get_prime_factors(uint64_t n, uint64_t* dest)
{
	uint64_t i;
	int k=0;
	if(n%2==0)
	{
		n=n/2;
		dest[k++]=2;
		while(n%2==0)
		{
			n=n/2;
			dest[k++]=2;
		}
	}
	for(i=3;i<=n;i+=2)
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
		}
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
 * Procedure qui declanche le premier thread
 */ 
void Compute_Simul_t1(void)
{
	
	while(1)
	{
		pthread_mutex_lock(&mutexLecture);
		if(fscanf(input,"%" SCNu64,&n1)==1)
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
		if(fscanf(input,"%" SCNu64,&n2)==1)
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
	input = fopen("test.in","r");
	pthread_mutex_init(&mutexLecture,NULL);
	pthread_mutex_init(&mutexAffichage,NULL);
	pthread_create(&t1,NULL,(void*)	Compute_Simul_t1,NULL);
	pthread_create(&t2,NULL,(void*) Compute_Simul_t2,NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&mutexLecture);
	pthread_mutex_destroy(&mutexAffichage);
	return 0;
}
