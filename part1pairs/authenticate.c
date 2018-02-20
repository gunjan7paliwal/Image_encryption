
#include <stdio.h>
#include <math.h>
//using namespace std;

int min(int a,int b){
    if(a>b)
        return b;
    else 
        return a;
}

int max(int a,int b){
    if(a>b)
        return a;
    else 
        return b;
}

struct pixel
{  
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char rf;
    unsigned char gf;
    unsigned char bf;
  
};

void hamming_code(int a)
{
    int z=a;
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
    tmp2=tmp1%10;
    d8=tmp2;
    //printf("%d %d %d %d",d1,d2,d3,d4);
    p1=d1^d2^d3;
    p2=d1^d2^d4;
    p3=d1^d3^d4;
    a=a-100*d6-10*d7*d8+100*p1+10*p2+p3;
    return ;
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
    int i, j, w, h;
    char blah[3];
    struct pixel image[512][512];
    long R[512][512],G[512][512],B[512][512];

    FILE *f, *f2;
    
    f=fopen("sample.ppm", "rb");
    f2=fopen("authenticatd.ppm", "wb");
    
    fscanf(f, "%s\n", blah);
    fscanf(f, "%d %d\n", &w, &h);
    fscanf(f, "%d\n", &i);
    
    fread(&image, sizeof(image), 1, f);
    int ar,br,cr,ag,bg,cg,ab,bb,cb;
    for(i=0;i<h;i++){
        for(j=0;j<w;j++){
            if(j%10==0)
            {
                cr=image[i-1][j-1].r;
                ar=image[i][j-1].r;
                br=image[i-1][j].r;
                //image[i][j].r=(ar+br)/2;
                if(cr>=max(ar,br))
                    image[i][j].r=min(ar,br);
                else if(cr<=min(ar,br))
                    image[i][j].r=max(ar,br);
                else
                    image[i][j].r=ar+br-cr;
                
                cg=image[i-1][j-1].g;
                ag=image[i][j-1].g;
                bg=image[i-1][j].g;
                //image[i][j].g=(ag+bg)/2;    
                
                if(cg>=max(ag,bg))
                    image[i][j].g=min(ag,bg);
                else if(cg<=min(ag,bg))
                    image[i][j].g=max(ag,bg);
                else
                    image[i][j].g=ag+bg-cg;
                    

                cb=image[i-1][j-1].b;
                ab=image[i][j-1].b;
                bb=image[i-1][j].b;

                if(cb>=max(ab,bb))
                    image[i][j].b=min(ab,bb);
                else if(cb<=min(ab,bb))
                    image[i][j].b=max(ab,bb);
                else
                    image[i][j].b=ab+bb-cb;                  
                
            }            
        }
    }

    fprintf(f2, "%s\n", blah);
    fprintf(f2, "%d %d\n", w, h);
    fprintf(f2, "%d\n", 255);
    
    fwrite(&image, sizeof(image), 1, f2);
    
    fclose(f);
    fclose(f2);
    
    return 0;
}

