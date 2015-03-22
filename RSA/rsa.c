#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXLENGTH 100
#define SUCCESS 0
#define FAILED 1

typedef struct {
  unsigned int n;
  unsigned int e;
} public_key;

typedef struct {
  unsigned int n;
  unsigned int d;
} private_key;

unsigned GenerateKey(public_key * public, private_key * private,unsigned phi);
unsigned GetE(unsigned phi);
unsigned Encrypt(public_key * public,char msg);
unsigned Decrypt(private_key * private, unsigned cypher);
unsigned GetRandomPrime(unsigned * p, unsigned *q);
unsigned Inv(unsigned e,unsigned phi);
int exgcd(int a, int b,int c, int * x,int * y);

int main(int argc,char * argv[])
{
  unsigned int p,q,phi;
  GetRandomPrime(&p,&q);
  unsigned n = p * q;

  //define container for message and cyphertext
  char msg[MAXLENGTH] = "HELLO";
  unsigned cypher[MAXLENGTH];

  //define public key and private key
  public_key public;
  private_key private;

  public.n = n;
  private.n = n;

  phi = (p -1)*(q-1);
  GenerateKey(&public,&private,phi);
  for(int i = 0; i < strlen(msg); i++)
  {
    cypher[i] = Encrypt(&public,msg[i]);
    printf(" %u -----> %u\n",msg[i],cypher[i]);
  }

  printf("----------------------------------------\n");
  printf("Decryption\n");

  unsigned result;
  for(int i = 0; i< strlen(msg); i++)
  {
    unsigned result = Decrypt(&private,cypher[i]);
    printf(" %u -----> %u\n",cypher[i],result);
  }

  return 0;
}

unsigned GetRandomPrime(unsigned int *p, unsigned int *q)
{
  //this is not a very important part of this program
  *p = 3;
  *q = 11;
  return SUCCESS;
}

unsigned GetE(unsigned phi)
{
  //just pick one
  return 7;
}

unsigned Inv(unsigned e,unsigned phi)
{
  int x,y;
  int d = exgcd(e,phi,1,&x,&y);
  int k = 1/d;
  x *= k;
  return x;
}

int exgcd(int a,int b,int c,int *x,int *y)
{
  if(b==0)
  {
    *x = 1;
    *y = 0;
    return a;
  }
  int r = exgcd(b,a%b,c,x,y);
  int t = *x;
  *x=*y;
  *y = t -a/b*(*y);
  return r;
}


unsigned GenerateKey(public_key * public,private_key * private,unsigned phi)
{
  unsigned e = GetE(phi);
  unsigned d = Inv(e,phi);

  public->e = e;
  private->d = d;

  printf("public key:%u %u\nprivate key: %u %u\n",public->n,public->e,private->n,private->d);
  return phi;
}

unsigned Encrypt(public_key * public,char msg)
{
  int result = (int) (fmod( pow(msg, public->e), public->n ) + 0.5);
  if ( result < 0 )   result += public->n;
  return (unsigned int) result;
}

unsigned Decrypt(private_key * private,unsigned cypher)
{
  int result = (int) (fmod( pow(cypher, private->d), private->n ) + 0.5);
  if ( result < 0 )   result += private->n;
  return (unsigned int) result;
}
