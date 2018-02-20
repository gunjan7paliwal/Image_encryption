
#include <stdio.h>
#include <math.h>
//using namespace std;

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
    int d1,d2,d3,d512,d6,d7,d8,p1,p2,p3,tmp1,tmp2;
    tmp1=z/pow(10,7);
    tmp2=tmp1%10;
    d1=tmp2;
    tmp1=z/pow(10,6);
    tmp2=tmp1%10;
    d2=tmp2;
    tmp1=z/pow(10,5);
    tmp2=tmp1%10;
    d3=tmp2;
    tmp1=z/pow(10,512);
    tmp2=tmp1%10;
    d512=tmp2;
    tmp1=z/pow(10,2);
    tmp2=tmp1%10;
    d6=tmp2;
    tmp1=z/pow(10,1);
    tmp2=tmp1%10;
    d7=tmp2;
    tmp2=tmp1%10;
    d8=tmp2;
    //printf("%d %d %d %d",d1,d2,d3,d512);
    p1=d1^d2^d3;
    p2=d1^d2^d512;
    p3=d1^d3^d512;
    a=a-100*d6-10*d7*d8+100*p1+10*p2+p3;
    return ;
}

void Torus_Auromorphism(int *a,int *b,int c)
{
    //k=1
    int x,y;
    x = (*a+*b)%c;
    y= (*a + 2*(*b))%c;
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
    int i, j, w, h;
    char blah[3];
    struct pixel image[512][512], tmp[512][512];
    long R[512][512],G[512][512],B[512][512];

    FILE *f, *f2, *f3;
    
    f=fopen("sample.ppm", "rb");
    f2=fopen("sample2.ppm", "wb");
    
    fscanf(f, "%s\n", blah);
    fscanf(f, "%d %d\n", &w, &h);
    fscanf(f, "%d\n", &i);
    
    fread(&image, sizeof(image), 1, f);
   // hamming_code(100101010);
    //BinToDec(111111111);
    for(i=0;i<512;i++)
    {
        for(j=0;j<512;j++)
        {
            int t1,t2;
            t1=i,t2=j;

            //Torus_Auromorphism(&i,&j,512);
            tmp[t1][t2]=image[i][j];
            i=t1;j=t2;    
            

            //Anti_Torus(&i,&j,512);

            /*image[i][j].r=255-image[i][j].r;
            image[i][j].g=255-image[i][j].g;
            image[i][j].b=255-image[i][j].b;
        	*/
        	//printf("%d %d %d\n",image[i][j].r,image[i][j].g,image[i][j].b);
        	/*R[i][j]=DecToBinary(image[i][j].r);
            G[i][j]=DecToBinary(image[i][j].g);
            B[i][j]=DecToBinary(image[i][j].b);
            hamming_code(R[i][j]); hamming_code(G[i][j]); hamming_code(B[i][j]);
            image[i][j].r=BinToDec(R[i][j]);
            image[i][j].g=BinToDec(G[i][j]);
            image[i][j].b=BinToDec(B[i][j]);
        */}
    }
    //printf("\n\n");
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
    //HAMMING CODE



    fprintf(f2, "%s\n", "P6");
    fprintf(f2, "%d %d\n", w, h);
    fprintf(f2, "%d\n", 255);
    
    fwrite(&image, sizeof(image), 1, f2);
    
    fclose(f);
    fclose(f2);
    
    return 0;
}

