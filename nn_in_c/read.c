#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "read.h"

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
   uint32_t image_index;
   uint32_t label_index;

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
   data.label_file = fopen("mnist_data/train-labels-idx1-ubyte","r"); // read mode
   data.image_file = fopen("mnist_data/train-images-idx3-ubyte","r"); // read mode
 
   if( data.label_file == NULL || data.image_file == NULL)
   {
      perror("Error while opening the mnist file.\n");
      exit(EXIT_FAILURE);
   }
   
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
   data.image_index = 0;
   data.label_index = 0;

   Vector *inputs = malloc(sizeof(Vector) + sizeof(double)*2);
   inputs->size = 2;
   inputs->vals[0] = 1;
   inputs->vals[1] = 2;

   data.cur_image = (Vector *)malloc(sizeof(Vector) 
                    + sizeof(double) * (data.image_rows * data.image_cols));
   data.cur_label = -1; // no data yet  


   return 1;
}

void free_mninst()
{
   free(data.cur_image);
   fclose(data.label_file);
   fclose(data.image_file);
}

Vector *getNextImage()
{
   for (int i = 0; i < data.image_rows * data.image_cols; ++i)
   {
      data.cur_image->vals[i] = ((double) fgetc(data.image_file))/256.0;
   }
   data.cur_image->size = 724;
   data.image_index++;
   return data.cur_image;
}

int getNextLabel(){
   data.cur_label = (int) fgetc(data.label_file);
   return data.cur_label;
}

/*
int main()
{
   // init
   load_minist_init();

   // call api
   Vector *inputs = getNextImage();
   int label = getNextLabel();
   
   // check output
   // printf("%d\n", label);
   // for (int i = 0; i < 784; ++i)
   // {
   //    label = inputs->vals[i];
   //    printf("%d\n", label);
   // }

   // free data
   free_mninst();
   return 0;
}
*/

