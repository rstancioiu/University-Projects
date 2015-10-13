#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <inttypes.h>
#define MAX_FACTORS 65
#define DICT_MAX_SZ 2500
#define N 510510
#define MAX_UINT64 18446744073709551615ull


//les nombres qu'on doit calculer pour chaque thread
uint64_t n1,n2,n3,n4;
//fichier de lecture
FILE* input;
//mutex de lecture
pthread_mutex_t mutexLecture;
//mutex d'affichage 
pthread_mutex_t mutexAffichage;

int primes[N/5];
char v[N/8/2];
int index1=0;

 typedef struct Factorisation{
 	uint64_t nombre;
 	int taille;
 	uint64_t facteurs[MAX_FACTORS];
 } Factors;
 
 // mutex de protection dictionnaire
 pthread_mutex_t mutexDict;
 // dictionnaire des factorisations réalisées précédemment
 Factors dictionnaire[DICT_MAX_SZ];
 // Indice du prochain slot disponible
 unsigned int slotLibre = 0;

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
 
 void GeneratePrimes()
{
	primes[index1++]=2;
	int i,j,k;
	for(i=1;((i*i)<<1)+(i<<1)<=N;++i)
	{
		if((v[i>>3]&(1<<(i&7)))==0)
		{
			primes[index1++]=(i<<1)+1;
			for(j=((i*i)<<1)+(i<<1),k=(i<<1)+1;(j<<1)+1<=N;j+=k)
			{
				v[j>>3]|=(1<<(j&7));
			}
		}
	}
	for(;(i<<1)+1<=N;++i)
	{
		if((v[i>>3]&(1<<(i&7)))==0)
		{
			primes[index1++]=(i<<1)+1;
		}
	}
}

 uint64_t gcd(uint64_t a, uint64_t b)
 {
 	while(b!=0)
 	{
 		uint64_t aux=a%b;
 		a=b;
 		b=aux;
 	}
 	return a;
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

uint64_t Multiplication(uint64_t a,uint64_t b,uint64_t m)
{

   // Perform 128 multiplication and division
   uint64_t q; // q = ⌊a b / m⌋
   uint64_t r; // r = a b mod m
   asm(
      "mulq %3;"
      "divq %4;"
      : "=a"(q), "=d"(r)
      : "a"(a), "rm"(b), "rm"(m));
   return r;
}

uint64_t brent_pollard_factor(uint64_t n)
{
   uint64_t m = 1000;
   uint64_t a, x, y, ys=2, r, q, g;
   a = 1;
   y = 2;
   r = 1;
   q = 1;
 
   do {
      x = y;
      uint64_t i;
      for( i=0; i < r; i++) {
         // y = y² + a mod n
         y = Multiplication(y, y, n);
         y += a;
         if(y < a)
            y += (MAX_UINT64 - n) + 1;
         y %= n;
      }
 
      uint64_t k =0;
      do {
      	uint64_t i;
         for( i=0; i < m && i < r-k; i++) {
            ys = y;
 
            // y = y² + a mod n
            y = Multiplication(y, y, n);
            y += a;
            if(y < a)
               y += (MAX_UINT64 - n) + 1;
            y %= n;
 
            // q = q |x-y| mod n
            q = Multiplication(q, (x>y)?x-y:y-x, n);
         }
         g = gcd(q, n);
         k += m;
      } while(k < r && g == 1);
 
      r <<= 1;
   } while(g == 1);
 
   if(g == n) {
      do {
         // ys = ys² + a mod n
         ys = Multiplication(ys, ys, n);
         ys += a;
         if(ys < a)
            ys += (MAX_UINT64 - n) + 1;
         ys %= n;
 
         g = gcd((x>ys)?x-ys:ys-x, n);
      } while(g == 1);
   }
   return g;
}

uint64_t Power(uint64_t b, uint64_t e, uint64_t m)
{
   uint64_t r = 1;
   for(; e; e >>= 1) {
      if(e & 1)
         r = Multiplication(r, b, m);
      b = Multiplication(b, b, m);
   }
   return r;
}

int MillerRabin(uint64_t n, uint64_t k)
{
   // Factor n-1 as d*2^s
   uint64_t s = 0;
   uint64_t d = n - 1;
   for(; !(d & 1); s++)
      d >>= 1;
   // Verify x = k^(d 2^i) mod n != 1
   uint64_t x = Power(k % n, d, n);
   if(x == 1 || x == n-1)
      return 1;
   while(s-- > 1) {
      // x = x^2 mod n
      x = Multiplication(x, x, n);
      if(x == 1)
         return 0;
      if(x == n-1)
         return 1;
   }
   return 0;
}

int Solve(uint64_t n, uint64_t* dest, int k)
{
	if(MillerRabin(n,19))
	{
		dest[k++]=n;
		return k;
	}
	else
	{
		uint64_t a = brent_pollard_factor(n);
		uint64_t b = n/a;
		if(a!=1 && a!=n)
		{
			k = Solve(a,dest,k);
			k = Solve(b,dest,k);
		}
		else
		{
			dest[k++]=n;
		} 
	}
	return k;
}

/*
 * Procedure qui calcule les facteurs premiers d'un nombre sur 64 bits 
 */
int get_prime_factors(uint64_t n, uint64_t* dest)
{
	int k=0;
	int dictInd=-1;
	int found=0;
	uint64_t nombre=n;
	if((dictInd = RecupererFacteurs(n))!=-1)
		found=1;
	if(n==1 && !found)
	{
		dest[k++]=1;
	}
	else if(!found)
	{
		int i;
		for( i = 0; i <index1;++i)
		{
			while(n%primes[i]==0)
			{
				dest[k++]=primes[i];
				n/=primes[i];
			}
		}
		if(n!=1)
			k = Solve(n,dest,k);
	}
	if(!found)
		AjouterFacteurs(nombre,dest,k);
	else
	{
		unsigned int i;
		int sz=dictionnaire[dictInd].taille;
		for(i=0;i<sz;++i)
		{
			dest[k++]=dictionnaire[dictInd].facteurs[i];
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
		printf(" %"SCNu64,factors[j]);
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
 * Procedure qui declanche le deuxieme thread
 */ 
void Compute_Simul_t3(void)
{
	while(1)
	{
		pthread_mutex_lock(&mutexLecture);
		if(fscanf(input,"%" SCNu64,&n3)==1)
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
		if(fscanf(input,"%" SCNu64,&n4)==1)
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
	GeneratePrimes();
	input = fopen("race.txt","r");
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
