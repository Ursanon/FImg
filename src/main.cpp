#include <cstdio>
#include <cstdint>


const int HEIGHT_CHUNK = 4;
const int WIDTH_CHUNK = 4;

int main()
{

    unsigned int uint32_dr [1];
    uint8_t uint8_dr [1];

    FILE * f = fopen("lena.png","rb");
    if(f != NULL)
    {
////////////////////
//PNG signature
      
      
        for(int i = 0; i < 8;i++)
        {
            //todo: delete magic numbers
            if(fread(uint8_dr,sizeof(uint8_t),1,f) != 1)
                printf("error!\n");
        printf("0x%X\n",*uint8_dr);
                
        }
//end of PNG signature
////////////////////

        puts("___\nChunks\n___");

////////////////////
//IHDR chunk
        ////////////////////
        //chunk length        
        if(fread(uint32_dr,sizeof(unsigned int),1,f) < 0)
            printf("error!\n");    
        printf("--\nLEN:0x%X\n",*uint32_dr);
        
        ////////////////////
        //chunk type-code
        if(fread(uint32_dr,sizeof(unsigned int),1,f) < 0)
            printf("error!\n");
        printf("--\nType:0x%X\n",*uint32_dr);

        ////////////////////
        //img width
        if(fread(uint32_dr,sizeof(unsigned int),1,f) < 0)
            printf("error!\n");
        printf("--\nWidth:0x%X\n",*uint32_dr);

        ////////////////////

        uint8_t uint8_dra[4];
        fread(uint8_dra,sizeof(uint8_t),4,f);
        for(int i = 3 ;i>-1;--i)
            printf("%d ",*(uint8_dra+i));

/*
        //img height
        if(fread(uint8_dr,sizeof(uint8_t),1,f) < 0)
            printf("error!\n");
        printf("--\nWidth:0x%X\n",*uint8_dr);
        
        //img height
        if(fread(uint8_dr,sizeof(uint8_t),1,f) < 0)
            printf("error!\n");
        printf("--\nWidth:0x%X\n",*uint8_dr);

        //img height
        if(fread(uint8_dr,sizeof(uint8_t),1,f) < 0)
            printf("error!\n");
        printf("--\nWidth:0x%X\n",*uint8_dr);

        //img height
        if(fread(uint8_dr,sizeof(uint8_t),1,f) < 0)
            printf("error!\n");
        printf("--\nWidth:0x%X\n",*uint8_dr);
*/
        
//end of IHDR chunk
////////////////////


        fclose(f);
    }

}