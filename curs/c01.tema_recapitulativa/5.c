#include <stdio.h>
#include <stdlib.h>

struct complex
{
    int real;
    int imaginar;
};
typedef struct complex COMPLEX;

COMPLEX fSum (COMPLEX *v , int n)
{
    COMPLEX sum={0,0};
    for (int i=0;i<n;++i)
    {
        sum.real+=v[i].real;
        sum.imaginar+=v[i].imaginar;
    }
    return sum;
}

void fModif(COMPLEX * v,int n,int poz,COMPLEX nou)
{
    v[poz] = nou;
}

COMPLEX fSterge(COMPLEX ** adr_v,int* adr_n,int poz)
{
    int n=*adr_n;
    COMPLEX* v = *adr_v;

    COMPLEX scos = v[poz];

    for (int i=poz+1;i<n;++i)
    {
        v[i]=v[i+1];
    }

    v=realloc(v,(n-1)*sizeof(COMPLEX));
    --n;

    *adr_n = n;
    *adr_v = v;

    return scos;
}

int fAdauga (COMPLEX ** adr_v,int* adr_n,int poz,COMPLEX nou)
{


    int n = *adr_n;


    COMPLEX* v=*adr_v;




    if(poz>n+1 || poz<0) return -1;


    ++n;
    v=realloc(v,(n+1)*sizeof(COMPLEX));


    for (int i=poz+1;i<n+1;++i)
    {
        v[i]=v[i-1];
    }
    v[poz]=nou;

    *adr_n=n;
    *adr_v=v;

    return 0;

}

int main()
{
    int n;
    printf("n=");
    scanf("%d",&n);


    COMPLEX * v = calloc(n,sizeof(COMPLEX));

    for (int i=0;i<n;++i)
    {
        printf("\ni=%d",i);
        printf("\nreal= ");
        scanf("%d",&v[i].real);
        printf("\nimaginar= ");
        scanf("%d",&v[i].imaginar);
    }


    COMPLEX sum=fSum(v,n);
    printf("\nsum=%d+i*%d",sum.real,sum.imaginar);

    COMPLEX nou;


    int poz;
    printf("\npozitie adaugare=");
    scanf("%d",&poz);
    printf("\nnou.real=");
    scanf("%d",&nou.real);
    printf("\nnou.imaginar=");
    scanf("%d",&nou.imaginar);

    int ret=fAdauga(v,&n,poz,nou);


    printf("\npozitie modificare=");
    scanf("%d",&poz);
    printf("\nnou.real=");
    scanf("%d",&nou.real);
    printf("\nnou.imaginar=");
    scanf("%d",&nou.imaginar);

    fModif(v,n,poz,nou);

    printf("\npozitie stergere=");
    scanf("%d",&poz);
    COMPLEX scos = fSterge(&v,&n,poz);
    printf("\nscos=%d+i*%d",scos.real,scos.imaginar);




}
