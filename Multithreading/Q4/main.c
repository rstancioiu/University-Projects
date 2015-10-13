#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>

//les nombres qu'on doit calculer pour chaque thread
uint64_t n1,n2; 

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
 * Procedure qui declanche les deux thread et execute le program
 */ 
void ComputeSimul()
{
	pthread_t t1,t2;
	
	while(1)
	{
		if(scanf("%" SCNu64,&n1)==1)
		{
			pthread_create(&t1,NULL,(void*)print_prime_factors_t1,NULL);
		}
		else break;
		pthread_join(t1, NULL);
		if(scanf("%" SCNu64,&n2)==1)
		{
			pthread_create(&t2,NULL,(void*)print_prime_factors_t2,NULL);
		}
		else break;
		pthread_join(t2, NULL);
	}
}


int main()
{
	//freopen("test.in","r",stdin);
	//freopen("test.out","w",stdout);
	ComputeSimul();
	return 0;
}
