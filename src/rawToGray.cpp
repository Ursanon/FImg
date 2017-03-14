#include <cstdio>
#include <cstdint>

const size_t HEIGHT = 512;
const size_t WIDTH = 512;
//bytes per pixel
const size_t BPP_COLOR = 3;

const size_t BPP_OUT = 1;

const size_t GRAY_MEM = HEIGHT * WIDTH * BPP_OUT;

uint8_t lena [BPP_COLOR];

//output:
//grayscale 1BPP
uint8_t output [GRAY_MEM];

uint8_t GetLuminosityFromRGB(uint8_t r, uint8_t g, uint8_t b);
void convertToGray();
void save();

int main(int argc, char * argv[])
{
    convertToGray();
    
    save();
}

void convertToGray()
{
    FILE* f = fopen("../images/lena.raw","rb");
    if(f != NULL)
    {
        for(int i = 0, j = 0; i < GRAY_MEM;++i)
        {
            fread(lena,BPP_COLOR,1,f);
            output[i] = GetLuminosityFromRGB(lena[j],lena[j+1],lena[j+2]);
        }
        save();

        fclose(f);
    }
    else
        puts("ERROR with file open!");
}

uint8_t GetLuminosityFromRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return (0.2126*r + 0.7152*g + 0.0722*b);
}

void save()
{
    FILE * f = fopen("out.raw","wb");
    if(f != NULL)
    {  
        fwrite(output,GRAY_MEM,1,f);
        fclose(f);
    }
    else
        puts("ERROR with file open!");
}