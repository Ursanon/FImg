// #include <cstdio>
// #include <cmath>
// #include <cstring>

// #include <exception>
// #include <fstream>
// #include <cstdint>
// #include <random>

// #include <chrono>
// #include <thread>
// #include <vector>

// //TODO:
// // *cleanup & OOP

// struct RGB
// {
//     uint8_t red;
//     uint8_t green;
//     uint8_t blue;
// };

// struct ranking
// {
//     size_t spec_indx;
//     float rating; 
// };

// struct Point
// {
//     int x;
//     int y;
// };

// RGB getColor();

// void combineColor(RGB *out, RGB &mutation, int index);

// void loadTarget();

// void mutate();

// void mutate_with_rectangle();

// void mutate_with_triangle();

// void dump_img(int indx);

// float rate(RGB *s);

// void evaluate();

// void crossOver();

// void cross(RGB * out,RGB * parent1, RGB *parent2);

// void insertion_sort(ranking * pt, int size);

// int pointLen(Point & a);

// bool isTriangle(Point &a, Point &b, Point &c);

// void combineColorWithThreads(int id);

// void rateWithThreads(int i);

// static const int W = 820;
// static const int H = 312;
// static const int MEM = W*H; 

// static const int BEST_CNT = 3;
// static const int SPEC_CNT = 12;

// static double BEST_SPEC_RATE = 2.0;

// RGB target[MEM] = {0};

// RGB best[BEST_CNT][MEM] = {0};

// RGB best_spec[MEM] = {0};

// RGB spec[SPEC_CNT][MEM] = {0};

// ranking spec_ranking[SPEC_CNT];

// static std::chrono::time_point<std::chrono::system_clock> timer_, time_end;
// static std::chrono::duration<float> elapsed_time;
// int main()
// {

// timer_ = std::chrono::system_clock::now();

//     loadTarget();

//     for(int i = 0;;++i)
//     {
//         //printf("\nGeneration: %d",i);
        
//         mutate();
         
//         evaluate();

//         if(i % 25 == 0)
//         {
//             crossOver();
//         }

//         dump_img(i);
//     }
// }

// RGB getColor()
// {
//     std::random_device rand;
//     RGB current;
//     current.red = rand() & 0xff;
//     current.green = rand() & 0xff;
//     current.blue = rand() & 0xff;
//     return current;
// }

// void combineColor(RGB *out, RGB &mutation,int index)
// {
//     out[index].red = (out[index].red + mutation.red )>>1;
//     out[index].green = (out[index].green + mutation.green )>>1;
//     out[index].blue = (out[index].blue + mutation.blue )>>1;
// }

// void loadTarget()
// {
//     FILE * file;
//     file = fopen("../images/eir_820_312.raw","rb");
    
//     for(int i = 0; i < MEM;++i)
//     {
//         fread(&target[i].red,sizeof(uint8_t),1,file);
//         fread(&target[i].green,sizeof(uint8_t),1,file);
//         fread(&target[i].blue,sizeof(uint8_t),1,file);
//     }
//     fclose(file);
// }

// void mutate()
// {
//     mutate_with_rectangle();
//     //mutate_with_triangle();
// }

// void mutate_with_rectangle()
// {
//     std::vector<std::thread> threads_;

//     for(int i = (SPEC_CNT-BEST_CNT+1); i < SPEC_CNT;++i)
//     {
//         threads_.push_back(std::thread(combineColorWithThreads,i));
//     }

//     for(auto&& th : threads_)
//     {
//         th.join();        
//     }
// }

// void combineColorWithThreads(int id)
// {
//     std::random_device rand;

//     uint32_t maxX = W - 1;
//     uint32_t maxY = H - 1;
//     uint32_t minX = 0;
//     uint32_t minY = 0;

//     uint32_t start_x = rand() % maxX;
//     uint32_t x_len = rand() % (maxX - start_x);

//     uint32_t start_y = rand() %maxY;
//     uint32_t y_len = rand() %(maxY - start_y);

//     //color for mutation
//     RGB color = getColor();

//     for(int j = 0; j < y_len;j++)
//     {
//         for(int k = 0; k < x_len;k++)
//         {
//             combineColor(spec[id],color,(start_y+j)*W + k + start_x);
//         }
//     }
// }

// void mutate_with_triangle()
// {
//     std::random_device rand;

//     uint32_t maxX = W - 1;
//     uint32_t maxY = H - 1;
//     uint32_t minX = 0;
//     uint32_t minY = 0;

//     Point points[3];

//     uint32_t h_x = 0;
//     uint32_t h_y = 0;
//     uint32_t l_x = maxX;
//     uint32_t l_y = maxY;

//     //color for mutation
//     RGB color = getColor();

//     double ar;

//     do
//     {
//         for(int i = 0; i < 3;++i)
//         {
//             points[i].x = rand() % maxX;
//             points[i].y = rand() % maxY;

//             if(h_x < points[i].x)
//                 h_x = points[i].x;
//             if(h_y < points[i].y)
//                 h_y = points[i].y;

//             if(l_x > points[i].x)
//                 l_x = points[i].x;
//             if(l_y > points[i].y)
//                 l_y = points[i].y;
//         }

//         ar = 1/2.f * ((points[1].x - points[0].x) * ( points[2].y - points[0].y) - (points[1].y - points[0].y) * (points[2].x - points[0].x));
    
//     }while(!isTriangle(points[0],points[1],points[2]) || (ar == 0));
 
//     int sign = (ar < 0) ? -1 : 1;

//     for(int i =(SPEC_CNT-BEST_CNT+1);i < SPEC_CNT;++i)
//     {
//         for(int j = l_y;j < h_y;++j)
//         {
//             for(int k = l_x;k<h_x;++k)
//             {
//                 long long int s = (points[0].y * points[2].x - points[0].x * points[2].y + (points[2].y - points[0].y) *k + (points[0].x - points[2].x) *j) * sign;
//                 long long int t = (points[0].x * points[1].y - points[0].y * points[1].x + (points[0].y - points[1].y) *k + (points[1].x - points[0].x) *j) * sign;
                    
//                 if(s >= 0 && t >= 0 && ((s + t) < (2 * ar * sign)))
//                 {
//                     combineColor(spec[i],color,(j)*W + k);
//                 }
//             }
//         }
//     }
// }

// int pointLen(Point & a)
// {
//     return sqrt(a.x * a.x + a.y * a.y);
// }

// bool isTriangle(Point &a, Point &b, Point &c)
// {
//     int aLen = pointLen(a);
//     int bLen = pointLen(b);
//     int cLen = pointLen(c);

//     if(aLen < bLen + cLen)
//         return true;
//     else if(bLen < aLen + cLen)
//         return true;
//     else if(cLen < aLen+bLen)
//         return true;
//     else 
//         return false;
// }


// float rate(RGB* s)
// {
//     float diff = 0.0;

//     for(int i = 0; i < MEM;i++)
//     {
//         diff += abs(target[i].red - s[i].red)/(MEM*255.f);
//         diff += abs(target[i].green - s[i].green)/(MEM*255.f);  
//         diff += abs(target[i].blue - s[i].blue)/(MEM*255.f);    
//     }

//     return diff/3.f;
// }

// void evaluate()
// {
//     std::vector<std::thread> threads_;
    
//     for(int i = 0; i < SPEC_CNT;++i)
//     {
//         threads_.push_back(std::thread(rateWithThreads,i));
//     }
    
//     for(auto&& th : threads_)
//     {
//         th.join();        
//     }

//     //sort'em
//     //good enought for small (< ~9k items) array     
//     insertion_sort(spec_ranking,SPEC_CNT);

//     for(int i = 0;i < BEST_CNT;++i)
//     {
//         memcpy(best[i],spec[spec_ranking[i].spec_indx],sizeof(best[i]));
        
//         if(BEST_SPEC_RATE > spec_ranking[i].rating)
//         {
//             memcpy(best_spec,best[i],sizeof(best_spec));
//             BEST_SPEC_RATE = spec_ranking[i].rating;            
//         }
//     }
// }

// void rateWithThreads(int i)
// {
//     spec_ranking[i].spec_indx = i;
//     spec_ranking[i].rating = rate(spec[i]);
// }

// void crossOver()
// {
//     int i = 0;
//     for(; i < (SPEC_CNT - BEST_CNT+1);++i)
//     {
//         memcpy(spec[i],best[i],sizeof(spec[i]));
//     }

//     for(int j = 1;i < SPEC_CNT;++i,j++)
//     {
//         cross(spec[i],best[j-1],best[j]);
//     }

// }

// void cross(RGB * out,RGB * parent1, RGB *parent2)
// {
//     int i = 0;

//     for(; i < MEM/2;++i)
//         out[i] = parent1[i]; 

//     for(; i < MEM;++i)
//         out[i] = parent2[i]; 
// }

// void dump_img(int indx)
// {
//     if(!(indx % 100))
//         printf(".");
//     if(indx % 100000)
//         return;

//     //char x;
//     //scanf("%c",&x);
//     char filename [256];
//     sprintf(filename,"populationBased/out_%d.raw",indx);
//     printf("\nOUT: %d",indx);
//     FILE * file;
//     file = fopen(filename,"wb");
//     for(int i = 0; i < MEM;i++)
//     {
//         fwrite(&best_spec[i].red,sizeof(uint8_t),1,file);
//         fwrite(&best_spec[i].green,sizeof(uint8_t),1,file);
//         fwrite(&best_spec[i].blue,sizeof(uint8_t),1,file);
//     }
//     fclose(file);
// }

// void insertion_sort(ranking * pt, int size)
// {
//     for(int i = 1; i < size;++i)
//     {
//         ranking temp = pt[i];

//         int j = i-1;
//         while(temp.rating < pt[j].rating && j >= 0)
//         {
//             pt[j+1] = pt[j];
//             j--;
//         }
//         pt[j+1] = temp;
//     }
// }