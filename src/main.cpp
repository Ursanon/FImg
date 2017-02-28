#include <cstdio>
#include <cstdint>

const size_t HEIGHT = 512;
const size_t WIDTH = 512;
//bytes per pixel
const size_t BPP_COLOR = 3;

const size_t BPP_OUT = 1;

const size_t COLOR_MEM = HEIGHT * WIDTH * BPP_COLOR;
const size_t GRAY_MEM = HEIGHT * WIDTH * BPP_OUT; 

uint8_t lena [COLOR_MEM];

//output:
//grayscale 8BPP
uint8_t output [GRAY_MEM];



void loadImage();
uint8_t GetLuminosityFromRGB(uint8_t r, uint8_t g, uint8_t b);
void convertToGray();
void save();

int main()
{
    loadImage();

    convertToGray();
    
    save();
}

void loadImage()
{
    FILE* f = fopen("lena.raw","rb");
    if(f != NULL)
    {
        fread(lena,COLOR_MEM,1,f);
        fclose(f);
    }
    else
    {
        puts("ERROR with file open!");
    }
}

uint8_t GetLuminosityFromRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return (0.2126*r + 0.7152*g + 0.0722*b);
}

void convertToGray()
{
    for(int i = 0, j = 0; i < GRAY_MEM;++i,j+=3)
        output[i] = GetLuminosityFromRGB(lena[j],lena[j+1],lena[j+2]);
}

void save()
{
    FILE * f = fopen("out.raw","wb");
    fwrite(output,GRAY_MEM,1,f);
    fclose(f);
}