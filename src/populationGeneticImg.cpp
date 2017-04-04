#include <cstdio>
#include <cmath>
#include <cstring>

#include <exception>
#include <fstream>
#include <cstdint>
#include <random>

//TODO:
// *cleanup & OOP

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


struct Point
{
    int x;
    int y;
};

RGB getColor();
void combineColor(RGB *out, RGB &mutation, int index);

void loadTarget();

void mutate();

void mutate_with_rectangle();

void mutate_with_triangle();

void dump_img(int indx);

double rate(RGB *s);

void evaluate();

void crossOver();

void cross(RGB * out,RGB * parent1, RGB *parent2);

void insertion_sort(ranking * pt, int size);

int pointLen(Point & a);

bool isTriangle(Point &a, Point &b, Point &c);

static const int W = 200;
static const int H = 133;
static const int MEM = W*H; 

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
    current.green = rand() & 0xff;
    current.blue = rand() & 0xff;
    return current;
}

void combineColor(RGB *out, RGB &mutation,int index)
{
    out[index].red = (out[index].red + mutation.red )>>1;
    out[index].green = (out[index].green + mutation.green )>>1;
    out[index].blue = (out[index].blue + mutation.blue )>>1;
}

void loadTarget()
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
}

void mutate()
{
    //mutate_with_rectangle();
    mutate_with_triangle();
}

void mutate_with_rectangle()
{
    std::random_device rand;

    uint32_t maxX = W - 1;
    uint32_t maxY = H - 1;
    uint32_t minX = 0;
    uint32_t minY = 0;

    uint32_t start_x = rand() % maxX;
    uint32_t x_len = rand() % (maxX - start_x);

    uint32_t start_y = rand() %maxY;
    uint32_t y_len = rand() %(maxY - start_y);

    //color for mutation
    RGB color = getColor();

    for(int i = (SPEC_CNT-BEST_CNT+1); i < SPEC_CNT;++i)
    {
        for(int j = 0; j < y_len;j++)
        {
            for(int k = 0; k < x_len;k++)
            {
                combineColor(spec[i],color,(start_y+j)*W + k + start_x);
            }
        }
    }
}


void mutate_with_triangle()
{
    std::random_device rand;

    uint32_t maxX = W - 1;
    uint32_t maxY = H - 1;
    uint32_t minX = 0;
    uint32_t minY = 0;

    Point points[3];

    uint32_t h_x = 0;
    uint32_t h_y = 0;
    uint32_t l_x = maxX;
    uint32_t l_y = maxY;

    //color for mutation
    RGB color = getColor();

    do
    {
        for(int i = 0; i < 3;++i)
        {
            points[i].x = rand() % maxX;
            points[i].y = rand() % maxY;

            if(h_x < points[i].x)
                h_x = points[i].x;
            if(h_y < points[i].y)
                h_y = points[i].y;

            if(l_x > points[i].x)
                l_x = points[i].x;
            if(l_y > points[i].y)
                l_y = points[i].y;
        }
    }while(!isTriangle(points[0],points[1],points[2]));

    long long int ar = 1/2 * (-points[1].y * points[2].x + points[0].y * (-points[1].y * points[2].x) + points[0].x * (points[1].y - points[2].y) + points[1].x * points[2].y);
    int sign = (ar < 0) ? -1 : 1;


    for(int i =(SPEC_CNT-BEST_CNT+1);i < SPEC_CNT;++i)
    {
        for(int j = l_y;j < h_y;++j)
        {
            for(int k = l_x;k<h_x;++k)
            {
                    long long int s = (points[0].y * points[2].x - points[0].x * points[2].y + (points[2].y - points[0].y) *k + (points[0].x - points[2].x) *j) * sign;
                    long long int t = (points[0].x * points[1].y - points[0].y * points[1].x + (points[0].y - points[1].y) *k + (points[1].x - points[0].x) *j) * sign;
                    if(s >= 0 && t >= 0)
                        combineColor(spec[i],color,(j)*W + k);
            }
        }
    }
}

int pointLen(Point & a)
{
    return sqrt(a.x * a.x + a.y * a.y);
}

bool isTriangle(Point &a, Point &b, Point &c)
{
    int aLen = pointLen(a);
    int bLen = pointLen(b);
    int cLen = pointLen(c);

    if(aLen > bLen +cLen)
        return true;
    else if(bLen > aLen + cLen)
        return true;
    else if(cLen > aLen+bLen)
        return true;
    else 
        return false;
}


double rate(RGB* s)
{
    double diff = 0.0;

    for(int i = 0; i < MEM;i++)
    {
        diff += abs((double)target[i].red - (double)s[i].red)/((double)MEM*255);
        diff += abs((double)target[i].green - (double)s[i].green)/((double)MEM*255);  
        diff += abs((double)target[i].blue - (double)s[i].blue)/((double)MEM*255);    
    }

    return diff/3.f;
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
    if(indx % 1000)
        return;

    //char x;
    //scanf("%c",&x);
    char filename [256];
    sprintf(filename,"populationBased/out_%d.raw",indx);
    printf("\nOUT: %d",indx);
    FILE * file;
    file = fopen(filename,"wb");
    for(int i = 0; i < MEM;i++)
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