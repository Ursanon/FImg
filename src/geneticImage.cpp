#include <cstdio>
#include <cmath>
#include <cstring>

#include <fstream>
#include <cstdint>
#include <random>

struct RGB
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

RGB getColor();
void combineColor(RGB *out,RGB * in, RGB &mutation, int index);

void mutate();

void dump_img(int indx);

void rate();

static const int W = 128;
static const int H = 128;
static const int MEM = W*H; 

RGB target[MEM] = {0};

RGB best[MEM] = {0};
RGB spec[MEM] = {0};


int main()
{
    FILE * file;
    file = fopen("../images/genetic.raw","rb");
    
    for(int i = 0; i < MEM;++i)
    {
        fread(&target[i].red,sizeof(uint8_t),1,file);
        fread(&target[i].green,sizeof(uint8_t),1,file);
        fread(&target[i].blue,sizeof(uint8_t),1,file);
    }
    
    fclose(file);
    for(int i = 0;;++i)
    {
        mutate();

        rate();

        dump_img(i);

    }


}

RGB getColor()
{
    std::random_device rand;
    RGB current;
    current.red = rand() & 0xff;
    current.green = rand() & 0xff;
    current.blue = rand() & 0xff;
    return current;
}

void combineColor(RGB *out,RGB *in, RGB &mutation,int index)
{
    out[index].red = (in[index].red + mutation.red )>>1;
    out[index].green = (in[index].green + mutation.green )>>1;
    out[index].blue = (in[index].blue + mutation.blue )>>1;
}

void mutate()
{
    std::random_device rand;

    uint8_t maxX = 127;
    uint8_t maxY = 127;
    uint8_t minX = 0;
    uint8_t minY = 0;

    uint8_t start_x = rand() % maxX;
    uint8_t x_len = rand() % (maxX - start_x);

    uint8_t start_y = rand() %maxY;
    uint8_t y_len = rand() %(maxY - start_y);

    RGB color = getColor();

    //printf("\nX: start: %d len: %d \nY: start:  %d len: %d",start_x,x_len,start_y,y_len);

    for(int i = 0 ;i < y_len;++i)
    {
        for(int j = 0; j < x_len;++j)
        {
            combineColor(spec,best,color,((start_y+i)*W)+(start_x+j));
        }
    }

}

void rate()
{
    // 256 'couse uint8_t limit's 255
    static double best_grade = 9000000;
    double grade = 0.0;

    for(int i = 0; i < MEM;i++)
    {
        grade += abs(target[i].red-spec[i].red);
        grade += abs(target[i].green-spec[i].green);
        grade += abs(target[i].blue-spec[i].blue);
    }

    if(best_grade > grade)
    {
        for(int i = 0 ; i < MEM;i++)
        {
            best[i].red = spec[i].red;
            best[i].green = spec[i].green;
            best[i].blue = spec[i].blue;
        }

        best_grade = grade;
    }

}

void dump_img(int indx)
{
    if(indx % 50000)
        return;

    char filename [256];
    sprintf(filename,"simpleSingleParent/out_%d.raw",indx);
    printf("\nOUT: %d",indx);
    FILE * file;
    file = fopen(filename,"wb");
    for(int i = 0; i < MEM;++i)
    {
        fwrite(&best[i].red,sizeof(uint8_t),1,file);
        fwrite(&best[i].green,sizeof(uint8_t),1,file);
        fwrite(&best[i].blue,sizeof(uint8_t),1,file);
    }
    fclose(file);
}