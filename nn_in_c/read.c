#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>


// label magic number 
#define LABEL_MAGIC 0x00000801

// label magic number 
#define IMAGES_MAGIC 0x00000803

// State of the data reader
static struct
{
   // mnist format data
   uint32_t data_size;
   FILE *image_file;
   FILE *label_file;
   uint32_t image_rows;
   uint32_t image_cols;

   // bookkeeping
   uint32_t cur_index;

   Vector *cur_image;
   int cur_label;   
} data;

static uint32_t endian_swap(uint32_t x)
{
   return ((x>>24)              | 
          ((x<<8) & 0x00FF0000) |
          ((x>>8) & 0x0000FF00) |
          (x<<24));
}


// void free_mninst();
// Vector *getNextImage();
// int getNextLabel();

int load_minist_init()
{
   // read labels
   char ch;
   data.label_file = fopen("mnist_data/labels","r"); // read mode
   data.image_file = fopen("mnist_data/images","r"); // read mode
 
   if( data.label_file == NULL || data.image_file == NULL)
   {
      perror("Error while opening the mnist file.\n");
      exit(EXIT_FAILURE);
   }
   
   // while( ( ch = fgetc(fp) ) != EOF )
   //    printf("%x \n",ch); 

   // check magic number
   uint32_t label_magic_read;
   uint32_t image_magic_read;
   fread(&label_magic_read, 4, 1, data.label_file);
   fread(&image_magic_read, 4, 1, data.image_file);
   assert(endian_swap(label_magic_read)==LABEL_MAGIC);
   assert(endian_swap(image_magic_read)==IMAGES_MAGIC);
   printf("%d read label \n", endian_swap(label_magic_read));

   // check data size matches
   uint32_t label_num;
   uint32_t image_num;
   fread(&label_num, 4, 1, data.label_file);
   fread(&image_num, 4, 1, data.image_file);
   assert(label_num == image_num);
   data.data_size = endian_swap(label_num);
   printf("%d read size \n", data.data_size);
   
   // save row and cols
   uint32_t tmp_row, tmp_col;
   fread(&tmp_row, 4, 1, data.image_file);
   fread(&tmp_col, 4, 1, data.image_file);
   data.image_rows = endian_swap(tmp_row);
   data.image_cols = endian_swap(tmp_col);
   printf("%d read row \n", data.image_rows);
   printf("%d read col \n", data.image_cols);

   // init other data
   data.cur_index = 0;

   Vector *inputs = malloc(sizeof(Vector) + sizeof(double)*2);
   inputs->size = 2;
   inputs->vals[0] = 1;
   inputs->vals[1] = 2;


   data.cur_image = (Vector *)malloc(sizeof(Vector) 
                    + sizeof(double) * (data.image_rows * data.image_cols));
   data.cur_label = -1; // no data yet  


   // printf("\n");
   // printf("%x \n", fgetc(data.label_file));
   // printf("%x \n", fgetc(data.label_file));
   // printf("%x \n", fgetc(data.label_file));
   // printf("%x \n", fgetc(data.label_file));
   // printf("%x \n", fgetc(data.label_file));
   // printf("%x \n", fgetc(data.label_file));
   // printf("\n");
   // printf("%x \n", fgetc(data.image_file));
   // printf("%x \n", fgetc(data.image_file));
   // printf("%x \n", fgetc(data.image_file));
   // printf("%x \n", fgetc(data.image_file));
   // printf("%x \n", fgetc(data.image_file));
   // printf("%x \n", fgetc(data.image_file));


   // fclose(data.label_file);
   // fclose(data.image_file);
   return 1;
}


Vector *getNextImage();
int getNextLabel();



int main()
{
   load_minist_init();
   return 0;
}


// Vector *read_mnist_label()
// {
//    char ch;
//    FILE *fp;
//    char *file_name = "mnist_data/testing";
//    fp = fopen(file_name,"r"); // read mode
 
//    if( fp == NULL )
//    {
//       perror("Error while opening the file.\n");
//       exit(EXIT_FAILURE);
//    }
 
//    printf("The contents of %s file are :\n", file_name);
   
//    while( ( ch = fgetc(fp) ) != EOF )
//       printf("%x ",ch);
 
//    fclose(fp);
//    return 0;
// }



    

// Vector *readfile()
// {
//    FILE *fileptr;
//    char *buffer;
//    long filelen;

//    fileptr = fopen("mnist_data/testing", "rb");  // Open the file in binary mode
//    fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
//    filelen = ftell(fileptr);             // Get the current byte offset in the file
//    rewind(fileptr);                      // Jump back to the beginning of the file

//    buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
//    fread(buffer, filelen, 1, fileptr); // Read in the entire file
//    fclose(fileptr); // Close the file

//    uint32_t magic = endian_swap(*(uint32_t *)(buffer));
//    uint32_t num = endian_swap(*((uint32_t *)(buffer) + 1 ));

//    // printf("%d\n", endian_swap(*(uint32_t *)(buffer)));
//    // printf("%d\n", endian_swap(*((uint32_t *)(buffer) + 1 )));


//    if( magic != 2049 )
//    {
//       perror("Wrong magic number for labels.\n");
//       exit(EXIT_FAILURE);
//    }

//    for (int i = 0; i < num; ++i)
//    {
//       /* code */
//       printf("%d", *(buffer + i));
//    }
//    return 0;
// }   
