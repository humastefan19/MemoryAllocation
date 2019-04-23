#include"alloc.h"
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<string.h>

int main(){
    double* b=myalloc(100*sizeof(double));
    int i;
    for(i=0;i<100;i++)
    {
        b[i]=1.2+i;
    }
    char *c=myalloc(20*sizeof(char));
    for(i=0;i<20;i++)
    {
        c[i]='a'+i;
    }
    printf("%s", c);
    printf("\n");
    for(i=0;i<100;i++)
    {
        printf("%f ", b[i]);
    }
    c=myRealloc(c, 30*sizeof(char));
    printf("\n");
    printf("Aici am realocat vectorul de caractere: \n");
    for(i=20;i<30;i++)
    {
        c[i]='a'+i;
    }
    printf("%s", c);
    myfree(b);
    myfree(c);
    
    
}
