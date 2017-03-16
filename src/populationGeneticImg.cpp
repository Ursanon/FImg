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
void combineColor(RGB *out, RGB &mutation, int index);

void loadTarget();

void mutate();

void dump_img(int indx);

double rate(RGB *s);

void evaluate();

void crossOver();

void cross(RGB * out,RGB * parent1, RGB *parent2);

void insertion_sort(ranking * pt, int size);

static const int W = 128;
static const int H = 128;
static const int BPP = 1;
static const int MEM = W*H*BPP; 

static const int BEST_CNT = 5;
static const int SPEC_CNT = 7;

static double BEST_SPEC_RATE = 2.0;

RGB target[MEM] = {0};

RGB best[BEST_CNT][MEM] = {0};

RGB best_spec[MEM] = {0};

RGB spec[SPEC_CNT][MEM] = {0};

ranking spec_ranking[SPEC_CNT];



int main()
{

    loadTarget();

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
    //UNCOMMENT!
    //current.green = rand() & 0xff;
    //current.blue = rand() & 0xff;
    return current;
}

void combineColor(RGB *out, RGB &mutation,int index)
{
    out[index].red = (out[index].red + mutation.red )>>1;
    //UNCOMMENT!
    //out[index].green = (in[index].green + mutation.green )>>1;
    //out[index].blue = (in[index].blue + mutation.blue )>>1;
}

void loadTarget()
{
    FILE * file;
    file = fopen("../images/genetic_g.raw","rb");
    
    for(int i = 0; i < MEM;++i)
    {
        fread(&target[i].red,sizeof(uint8_t),1,file);
        //UNCOMMENT!
        //fread(&target[i].green,sizeof(uint8_t),1,file);
        //fread(&target[i].blue,sizeof(uint8_t),1,file);
    }
    fclose(file);
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

    //color for mutation
    RGB color = getColor();

    for(int i = (SPEC_CNT-BEST_CNT+1); i < SPEC_CNT;++i)
    {
        for(int j = 0; j < y_len;j++)
        {
            for(int k = 0; k < x_len;k++)
            {
                combineColor(spec[i],color,(start_y+j)*H + k + start_x);
            }
        }
    }

}

double rate(RGB* s)
{
    double diff = 0.0;

    for(int i = 0; i < MEM;i++)
    {
        diff += abs((double)target[i].red - (double)s[i].red)/((double)MEM*255);  
        //TODO:
        //other colors
    }

    return diff;
}

void evaluate()
{
    //rate every specimens
    for(int i =0; i < SPEC_CNT;++i)
    {
        spec_ranking[i].spec_indx = i;
        spec_ranking[i].rating = rate(spec[i]);
    }
    //sort'em
    insertion_sort(spec_ranking,SPEC_CNT);


    for(int i = 0;i < BEST_CNT;++i)
    {
        memcpy(best[i],spec[spec_ranking[i].spec_indx],sizeof(best[i]));
        
        if(BEST_SPEC_RATE > spec_ranking[i].rating)
        {
            memcpy(best_spec,best[i],sizeof(best_spec));
            BEST_SPEC_RATE = spec_ranking[i].rating;            
        }
    }
}

void crossOver()
{
    int i = 0;
    for(; i < (SPEC_CNT - BEST_CNT+1);++i)
    {
        memcpy(spec[i],best[i],sizeof(spec[i]));
    }

    for(int j = 1;i < SPEC_CNT;++i,j++)
    {
        cross(spec[i],best[j-1],best[j]);
    }

}

void cross(RGB * out,RGB * parent1, RGB *parent2)
{
    int i = 0;

    for(; i < MEM/2;++i)
        out[i] = parent1[i]; 

    for(; i < MEM;++i)
        out[i] = parent2[i]; 
}

void dump_img(int indx)
{
    if(indx % 5000)
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
        //UNCOMMENT!
        //fwrite(&best_spec[i].green,sizeof(uint8_t),1,file);
        //fwrite(&best_spec[i].blue,sizeof(uint8_t),1,file);
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