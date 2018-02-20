//#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <cuda.h>

struct pixel
{  
    unsigned char r;
    unsigned char g;
    unsigned char b;
  
};
__device__
int power(int a,int b){
    long sum=1;
   for (int i=1;i<=b;i++){
       sum=sum*a;     
   }
   return sum; 
}

__device__
void hamming_code(long *a)
{
    long z=*a;
    int d1,d2,d3,d4,d6,d7,d8,p1,p2,p3,tmp1,tmp2;
    tmp2=power(10,7);
    tmp1=z/tmp2;
    tmp2=tmp1%10;
    d1=tmp2;
    tmp2=power(10,6);
    tmp1=z/tmp2;
    tmp2=tmp1%10;
    d2=tmp2;
    tmp2=power(10,5);
    tmp1=z/tmp2;
    tmp2=tmp1%10;
    d3=tmp2;
    tmp2=power(10,4);
    tmp1=z/tmp2;
    tmp2=tmp1%10;
    d4=tmp2;
    tmp1=z/100;
    tmp2=tmp1%10;
    d6=tmp2;
    tmp1=z/10;
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
__device__
void bit_rotation(long *a)
{
    long z=*a,tmp,tmp1,tmp2,p1,p2,p3;
    tmp1=z/100;
    tmp2=tmp1%10;
    p1=tmp2;
    tmp1=z/10;
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

__device__
void rev_bit_rotation(long *a)
{
    long z=*a,tmp1,tmp2,p1,p2,p3,tmp;
    tmp1=z/power(10,2);
    tmp2=tmp1%10;
    p1=tmp2;
    tmp1=z/power(10,1);
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


__device__
void Torus_Auromorphism(int *a,int *b,int c)
{
    //k=1
    int x,y;
    x = (*a+*b)%c;
    y= (*a + 2*(*b))%c;
 
    *a=x;*b=y;
    return;
}

__device__
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


__device__
int DecToBinary(int num)
{
    int count=0, remainder, base = 1, binary = 0, no_of_1s = 0;
    
    while (num > 0)
    {
        count++;
        remainder = num % 2;
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

__device__
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



__global__
void PictureKernel(struct pixel* IPimage, struct pixel* OPimage,long* R,long* G,long* B,int w,int h)
{
    // Calculate the row # of the d_Pin and d_Pout element
    int Col = blockIdx.x*blockDim.x + threadIdx.x;
    // Calculate the column # of the d_Pin and d_Pout element
    int Row = blockIdx.y*blockDim.y + threadIdx.y;
    // each thread computes one element of d_Pout if in range
    
    int tmp1,tmp2;
    tmp2=Col;tmp1=Row;
    Torus_Auromorphism(&tmp1,&tmp2,w);

    R[Col+Row*w]=DecToBinary(IPimage[Col+Row*w].r);
    G[Col+Row*w]=DecToBinary(IPimage[Col+Row*w].g);
    B[Col+Row*w]=DecToBinary(IPimage[Col+Row*w].b);

    hamming_code(&R[Col+Row*w]);hamming_code(&G[Col+Row*w]);hamming_code(&B[Col+Row*w]);
    bit_rotation(&R[Col+Row*w]);bit_rotation(&G[Col+Row*w]);bit_rotation(&B[Col+Row*w]);


    IPimage[Col+Row*w].r=BinToDec(R[Col+Row*w]);
    IPimage[Col+Row*w].g=BinToDec(G[Col+Row*w]);
    IPimage[Col+Row*w].b=BinToDec(B[Col+Row*w]);
    
    

    //Torus
    OPimage[tmp1*w+tmp2]=IPimage[Row*w+Col];

    //printf("1");
    
    //printf("%d %d %d %d\n",Col,Row,tmp1,tmp2);
    //OPimage[tmp1 + n*tmp2]=IPimage[Col+Row*n];
    return;
}



int main(void)
{
    int i, w, h;
    char blah[3];

    FILE *f, *f2;
    
    //clock_t begin = clock();
    //clock_t begin1=clock();

    f=fopen("man.ppm", "rb");
    f2=fopen("testencrypted.ppm", "wb"); 
    fscanf(f, "%s\n", blah);
    fscanf(f, "%d %d\n", &w, &h);
    fscanf(f, "%d\n", &i);
    struct pixel image[h][w]; 
    fread(&image, sizeof(image), 1, f);
    
  
    
    //double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //printf("%f",time_spent);

    //long R[h][w],G[h][w],B[h][w];
    struct pixel *d_A,*d_F;
    long *d_R,*d_G,*d_B;
    
    long n=w*h;
    //const long size=n;
    const long bytes = 3*sizeof(unsigned char)*n;


    //Assigning memory in device

    cudaMalloc((void **)&d_A,sizeof(pixel)*n);
    cudaMalloc((void **)&d_F,sizeof(pixel)*n);
    cudaMalloc((void **)&d_R,sizeof(long)*n);
    cudaMalloc((void **)&d_G,sizeof(long)*n);
    cudaMalloc((void **)&d_B,sizeof(long)*n);


    cudaMemcpy(d_A,image,bytes,cudaMemcpyHostToDevice);

/*  cudaMemcpy(&d_R,&image,bytes,cudaMemcpyHostToDevice);
    cudaMemcpy(&d_G,&image,bytes,cudaMemcpyHostToDevice);
    cudaMemcpy(&d_B,&image,bytes,cudaMemcpyHostToDevice);
  */ 
  //dim3 X((n-1)/4+1,(n-1)/4+1);
   //dim3 Y( 4,4);

   dim3 threadsPerBlock(32, 32);
   dim3 numBlocks(w/threadsPerBlock.x,h/threadsPerBlock.y); 

    clock_t begin = clock();
    PictureKernel<<<numBlocks,threadsPerBlock>>>(d_A,d_F,d_R,d_G,d_B,w,h);    
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("%f micro seconds\n",time_spent*1000000);
    
    cudaMemcpy(image,d_F,bytes,cudaMemcpyDeviceToHost);



    cudaFree(d_A);
    cudaFree(d_F);
    cudaFree(d_R);
    cudaFree(d_G);
    cudaFree(d_B);


    


    fprintf(f2, "%s\n", blah);
    fprintf(f2, "%d %d\n", w, h);
    fprintf(f2, "%d\n", 255);
    
    fwrite(&image, sizeof(image), 1, f2);
    
    fclose(f);
    fclose(f2);
    
    return 0;
}

