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

struct ranking
{
    size_t spec_indx;
    double rating; 
};

RGB getColor();
void combineColor(RGB *out,RGB * in, RGB &mutation, int index);

void mutate();

void dump_img(int indx);

double rate(RGB *s);

void evaluate();

void crossOver();

void insertion_sort(ranking * pt, int size);

static const int W = 128;
static const int H = 128;
static const int MEM = W*H; 

static const int BEST_CNT = 2;
static const int SPEC_CNT = 10;

RGB target[MEM] = {0};

RGB best[BEST_CNT][MEM] = {0};

RGB best_spec [MEM] = {0};

RGB spec[SPEC_CNT][MEM] = {0};

ranking spec_ranking[SPEC_CNT];

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

        evaluate();
        
        crossOver();
        
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

    for(int m = 0; m < SPEC_CNT;++m)
    {
        for(int i = 0 ;i < y_len;++i)
        {
            for(int j = 0; j < x_len;++j)
            {
                combineColor(spec[m],best_spec,color,((start_y+i)*W)+(start_x+j));
            }
        }
    }
    

}

double rate(RGB* s)
{
    double miss = 0.0;

    for(int i = 0; i < MEM;i++)
    {
        miss += abs(target[i].red-s[i].red);
        miss += abs(target[i].green-s[i].green);
        miss += abs(target[i].blue-s[i].blue);
    }
    return abs(1 -(miss)/(3*MEM));
}

void evaluate()
{
    for(int i = 0; i < SPEC_CNT;++i)
    {
        spec_ranking[i].spec_indx = i;
        spec_ranking[i].rating = rate(spec[i]);
    }

    insertion_sort(spec_ranking,SPEC_CNT);

    for(int i = 0; i < BEST_CNT;++i)
    {
        printf("\nRating: %lf CURR BEST:%lf",spec_ranking[i].rating,rate(best[i]) );
        if(spec_ranking[i].rating < rate(best[i]))
            memcpy(best[i],spec[spec_ranking[i].spec_indx],sizeof(best[i]));
    }

}

void crossOver()
{
    for(int i = 0; i < MEM;i++)
    {
        int x = i % BEST_CNT;
        best_spec[i] = best[x][i];
    }
}

void dump_img(int indx)
{
    if(indx % 10000)
        return;

    //char x;
    //scanf("%c",&x);

    char filename [256];
    sprintf(filename,"populationBased/out_%d.raw",indx);
    printf("\nOUT: %d",indx);
    FILE * file;
    file = fopen(filename,"wb");
    for(int i = 0; i < MEM;++i)
    {
        fwrite(&best_spec[i].red,sizeof(uint8_t),1,file);
        fwrite(&best_spec[i].green,sizeof(uint8_t),1,file);
        fwrite(&best_spec[i].blue,sizeof(uint8_t),1,file);
    }
    fclose(file);
}

void insertion_sort(ranking * pt, int size)
{
    for(int i = 1; i < size;++i)
    {
        ranking temp = pt[i];

        int j = i-1;
        while(temp.rating < pt[j].rating && j >= 0)
        {
            pt[j+1] = pt[j];
            j--;
        }
        pt[j+1] = temp;
    }
}