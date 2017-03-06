#include <cstdio>
#include <cstdint>

const int HEIGHT_CHUNK = 4;
const int WIDTH_CHUNK = 4;

void byteswap_uint32(unsigned int &x)
{
    x = (x>>24) | 
    ((x<<8) & 0x00FF0000) |
    ((x>>8) & 0x0000FF00) |
    (x<<24);
}


struct ImageSize {
    uint32_t width;
    uint32_t height;    
};

const uint8_t png_signature [] = {137,80,78,71,13,10,26,10};

int main()
{
    ImageSize image;

    unsigned int uint32_dr [1];
    uint8_t *uint8_dr = new uint8_t;

    FILE * f = fopen("lena.png","rb");
    if(f != NULL)
    {
        bool is_good = true;
////////////////////
//PNG signature 
      
        for(int i = 0; i < 8;i++)
        {
            //todo: delete magic numbers
            if(fread(uint8_dr,sizeof(uint8_t),1,f) != 1)
                printf("Error with file reading!\n");
            else
            {
                if(*uint8_dr != png_signature[i])
                {
                    is_good = false;
                    break;
                }
            }
                
        }
//end of PNG signature
////////////////////
    delete uint8_dr; 
    
    if(!is_good)
    {
        puts("Wrong PNG signature!");
        fclose(f);
    }

    puts("___\nReading chunks:\n___");

////////////////////
//IHDR chunk
        ////////////////////
        //chunk length        
        if(fread(uint32_dr,sizeof(unsigned int),1,f) != 1)
            printf("Error with file reading!\n");    
        printf("--\nLEN:%u\n",*uint32_dr);
        
        ////////////////////
        //chunk type-code
        if(fread(uint32_dr,sizeof(unsigned int),1,f) != 1)
            printf("Error with file reading!\n");
        printf("--\nType:0x%X\n--\n",*uint32_dr);

        ////////////////////
        //img width
        if(fread(&image.width,sizeof(unsigned int),1,f) != 1)
            printf("Error with file reading!\n");
        byteswap_uint32(image.width);

        printf("Width:%u\n",image.width);

        ////////////////////

        ////////////////////
        //img height
        if(fread(&image.height,sizeof(unsigned int),1,f) != 1)
            printf("Error with file reading!\n");
        byteswap_uint32(image.height);

        printf("Height:%u\n",image.height);
        ////////////////////

//end of IHDR chunk
////////////////////


        fclose(f);
    }

}