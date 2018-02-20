#include<stdio.h>

struct pixel
{  
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

int main()
{
    int i, j;
    struct pixel image[400][400];
    FILE *f;
    
    f=fopen("sample.ppm", "w");
    
    fprintf(f, "P6\n");
    fprintf(f, "400 400\n");
    fprintf(f, "255\n");
    
    for(i=0;i<400;i++)
    {
        for(j=0;j<400;j++)
        {
            pixel[i][j].r=50;
            pixel[i][j].g=100;
            pixel[i][j].b=150;
        }
    }
    
    ZZ
    
    fclose(f);
    
    return 0;
}

