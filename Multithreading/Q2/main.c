#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>


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
 * Le main de la source 
 * on prend plusieurs nombres
 */
int main()
{
	//freopen("test.in","r",stdin);
	//freopen("test.out","w",stdout);
	uint64_t n;
	while(scanf("%" SCNu64,&n)==1)
	{
		print_prime_factors(n);
	}
	return 0;
}
