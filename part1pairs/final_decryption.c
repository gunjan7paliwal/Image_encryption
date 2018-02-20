
#include <stdio.h>
#include <math.h>
#include <time.h>
//using namespace std;

struct pixel
{  
    unsigned char r;
    unsigned char g;
    unsigned char b;
  
};

void hamming_code(long *a)
{
    long z=*a;
    int d1,d2,d3,d4,d6,d7,d8,p1,p2,p3,tmp1,tmp2;
    tmp1=z/pow(10,7);
    tmp2=tmp1%10;
    d1=tmp2;
    tmp1=z/pow(10,6);
    tmp2=tmp1%10;
    d2=tmp2;
    tmp1=z/pow(10,5);
    tmp2=tmp1%10;
    d3=tmp2;
    tmp1=z/pow(10,4);
    tmp2=tmp1%10;
    d4=tmp2;
    tmp1=z/pow(10,2);
    tmp2=tmp1%10;
    d6=tmp2;
    tmp1=z/pow(10,1);
    tmp2=tmp1%10;
    d7=tmp2;
    tmp2=(*a)%10;
    d8=tmp2;

    p1=d1^d2^d3;
    p2=d1^d2^d4;
    p3=d1^d3^d4;
    *a=(*a)-100*d6-10*d7-d8+100*p1+10*p2+p3;
return ;
}


void bit_rotation(long *a)
{
    long z=*a,tmp,tmp1,tmp2,p1,p2,p3,d1,d2,d3,d4;
    tmp1=z/pow(10,2);
    tmp2=tmp1%10;
    p1=tmp2;
    tmp1=z/pow(10,1);
    tmp2=tmp1%10;
    p2=tmp2;
    tmp2=*a%10;
    p3=tmp2;
    *a=(*a)-100*p1-10*p2-p3;
    tmp=p1;
    p1=p2;
    p2=p3;
    p3=tmp;
    *a=(*a)+100*p1+10*p2+p3;
    return;
}

void rev_bit_rotation(long *a)
{
    long z=*a,tmp1,tmp2,p1,p2,p3,tmp;
    tmp1=z/pow(10,2);
    tmp2=tmp1%10;
    p1=tmp2;
    tmp1=z/pow(10,1);
    tmp2=tmp1%10;
    p2=tmp2;
    tmp2=*a%10;
    p3=tmp2;
    *a=(*a)-100*p1-10*p2-p3;
    tmp=p3;
    p3=p2;
    p2=p1;
    p1=tmp;
    //printf("%d\n",*a);
    *a=(*a)+100*p1+10*p2+p3;
    //printf("%d",*a);
    return;
}

void Torus_Auromorphism(int *a,int *b,int c)
{
    //k=1
    int x,y;
    x = (*a+*b)%c;
    y= (*a + 2*(*b))%c;
   // if(*a<2&&*b<2)
    //printf("a=%d b=%d x=%d y=%d\n",*a,*b,x,y);
    *a=x;*b=y;
    return;
}

void Anti_Torus(int *a,int *b,int c)
{
    int x,y;
    x=(2*(*a)+(-1)*(*b)+100000*c)%c;
    y=((-1)*(*a)+*b+10000*c)%c;
    //printf("a=%d b=%d x=%d y=%d\n",*a,*b,x,y);
    *a=x;*b=y;
    //if(*a=2&&*b2)
    
    //*a=x;*b=y;
}
    
int DecToBinary(int num)
{
	int decimal_num,count=0, remainder, base = 1, binary = 0, no_of_1s = 0,flag=0;
	
    while (num > 0)
    {
    	count++;
        remainder = num % 2;
        flag=0;
        if (remainder == 1)
        {
            no_of_1s++;
        }
        binary = binary + remainder * base;
        num = num / 2;
        base = base * 10;

    }
    binary+=100000000;

//    printf("binary = %d\n",binary);
	return binary;
}

int BinToDec(int num){
    num-=100000000;
    int dec=0,k=1,i=0;
    //printf("%d",num);

    while(1){
        
        dec=dec + k*(num%10);
        if(i==0)
            i=1;
        else
            i=2*i;
        if(num==0)
        {
            break;
        }

        k=2*i;
        
        num/=10;
    }
    //printf("\n%d",dec);
    return dec;
}

int main()
{
    
    clock_t begin = clock();
    //clock_t begin1=clock();

    int i, j, w, h;
    char blah[3];
    struct pixel image[512][512], tmp[512][512];
    long R[512][512],G[512][512],B[512][512];

    FILE *f, *f2, *f3;
    
    f=fopen("encrypted.ppm", "rb");
    f2=fopen("decrypted.ppm", "wb");
    
    fscanf(f, "%s\n", blah);
    fscanf(f, "%d %d\n", &w, &h);
    fscanf(f, "%d\n", &i);
    
    fread(&image, sizeof(image), 1, f);

    rev_bit_rotation(&R[i][j]);rev_bit_rotation(&G[i][j]);rev_bit_rotation(&B[i][j]);

    
    for(i=0;i<512;i++)
    {
        for(j=0;j<512;j++)
        {
            

            int t1,t2;
            t1=i,t2=j;
            tmp[t1][t2]=image[i][j];
            i=t1;j=t2;   
        }
    }        
    for(i=0;i<512;i++)
    {
        for(j=0;j<512;j++)
        {
            int t1,t2;
            t1=i;t2=j;   
            Anti_Torus(&i,&j,512);
            image[i][j]=tmp[t1][t2];
            i=t1;j=t2;
        }
    }

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f micro seconds\n",time_spent*1000000);


    fprintf(f2, "%s\n", blah);
    fprintf(f2, "%d %d\n", w, h);
    fprintf(f2, "%d\n", 255);
    
    fwrite(&image, sizeof(image), 1, f2);
    
    fclose(f);
    fclose(f2);
    
    
    return 0;
}

