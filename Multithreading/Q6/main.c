#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>

//les nombres qu'on doit calculer pour chaque thread
uint64_t n1,n2;
//fichier de lecture
FILE* input;
// mutex qui nous permet de differencier les threads
pthread_mutex_t mutexLecture;

/*
 * Procedure qui affiche les facteurs premiers d'un nombre sur 64 bits 
 * Il regard,d'abord, si le nombre est pair
 * et apres il prend tout ses facteurs impaires  
 */
void print_prime_factors(uint64_t n)
{
	uint64_t i;
	printf("%" SCNu64":",n);
	if(n==1)
		printf(" 1");
	if(n%2==0)
	{
		n=n/2;
		printf(" 2");
		while(n%2==0)
		{
			n=n/2;
		    printf(" 2");
		}
	}
	for(i=3;i<=n;i+=2)
	{
		if(n%i==0)
		{
			n=n/i;
			printf(" %" SCNu64,i);
			while(n%i==0)
			{
				n=n/i;
			    printf(" %" SCNu64,i);
			}
		}
	}
	printf("\n");
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
	input = fopen("seul.in","r");
	pthread_mutex_init(&mutexLecture,NULL);
	pthread_create(&t1,NULL,(void*)	Compute_Simul_t1,NULL);
	pthread_create(&t2,NULL,(void*) Compute_Simul_t2,NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	pthread_mutex_destroy(&mutexLecture);
	return 0;
}
