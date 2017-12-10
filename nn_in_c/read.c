// @author Bernie Yang

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
} data_test;

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
} data_train;


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
int load_mnist_init_train()
{
   // read labels
   char ch;
   data_train.label_file = fopen("mnist_data/train-labels-idx1-ubyte","r"); // read mode
   data_train.image_file = fopen("mnist_data/train-images-idx3-ubyte","r"); // read mode
 
   if( data_train.label_file == NULL || data_train.image_file == NULL)
   {
      perror("Error while opening the mnist file.\n");
      exit(EXIT_FAILURE);
   }
   
   // check magic number
   uint32_t label_magic_read;
   uint32_t image_magic_read;
   fread(&label_magic_read, 4, 1, data_train.label_file);
   fread(&image_magic_read, 4, 1, data_train.image_file);
   assert(endian_swap(label_magic_read)==LABEL_MAGIC);
   assert(endian_swap(image_magic_read)==IMAGES_MAGIC);
   printf("%d read label \n", endian_swap(label_magic_read));

   // check data size matches
   uint32_t label_num;
   uint32_t image_num;
   fread(&label_num, 4, 1, data_train.label_file);
   fread(&image_num, 4, 1, data_train.image_file);
   assert(label_num == image_num);
   data_train.data_size = endian_swap(label_num);
   printf("%d read size \n", data_train.data_size);
   
   // save row and cols
   uint32_t tmp_row, tmp_col;
   fread(&tmp_row, 4, 1, data_train.image_file);
   fread(&tmp_col, 4, 1, data_train.image_file);
   data_train.image_rows = endian_swap(tmp_row);
   data_train.image_cols = endian_swap(tmp_col);
   printf("%d read row \n", data_train.image_rows);
   printf("%d read col \n", data_train.image_cols);

   // init other data
   data_train.image_index = 0;
   data_train.label_index = 0;

   Vector *inputs = malloc(sizeof(Vector) + sizeof(double)*2);
   inputs->size = 2;
   inputs->vals[0] = 1;
   inputs->vals[1] = 2;

   data_train.cur_image = (Vector *)malloc(sizeof(Vector) 
                    + sizeof(double) * (data_train.image_rows * data_train.image_cols));
   data_train.cur_label = -1; // no data yet  


   return 1;
}

void free_mnist_train()
{
   free(data_train.cur_image);
   fclose(data_train.label_file);
   fclose(data_train.image_file);
}

Vector *getNextImageTrain()
{
   for (int i = 0; i < data_train.image_rows * data_train.image_cols; ++i)
   {
      data_train.cur_image->vals[i] = ((double) fgetc(data_train.image_file))/256.0;
   }
   data_train.cur_image->size = 724;
   data_train.image_index++;
   return data_train.cur_image;
}

int getNextLabelTrain(){
   data_train.cur_label = (int) fgetc(data_train.label_file);
   return data_train.cur_label;
}



int load_mnist_init_test()
{
   // read labels
   char ch;
   data_test.label_file = fopen("mnist_data/t10k-labels-idx1-ubyte","r"); // read mode
   data_test.image_file = fopen("mnist_data/t10k-images-idx3-ubyte 2","r"); // read mode
 
   if( data_test.label_file == NULL || data_test.image_file == NULL)
   {
      perror("Error while opening the mnist file.\n");
      exit(EXIT_FAILURE);
   }
   
   // check magic number
   uint32_t label_magic_read;
   uint32_t image_magic_read;
   fread(&label_magic_read, 4, 1, data_test.label_file);
   fread(&image_magic_read, 4, 1, data_test.image_file);
   assert(endian_swap(label_magic_read)==LABEL_MAGIC);
   assert(endian_swap(image_magic_read)==IMAGES_MAGIC);
   printf("%d read label \n", endian_swap(label_magic_read));

   // check data size matches
   uint32_t label_num;
   uint32_t image_num;
   fread(&label_num, 4, 1, data_test.label_file);
   fread(&image_num, 4, 1, data_test.image_file);
   assert(label_num == image_num);
   data_test.data_size = endian_swap(label_num);
   printf("%d read size \n", data_test.data_size);
   
   // save row and cols
   uint32_t tmp_row, tmp_col;
   fread(&tmp_row, 4, 1, data_test.image_file);
   fread(&tmp_col, 4, 1, data_test.image_file);
   data_test.image_rows = endian_swap(tmp_row);
   data_test.image_cols = endian_swap(tmp_col);
   printf("%d read row \n", data_test.image_rows);
   printf("%d read col \n", data_test.image_cols);

   // init other data
   data_test.image_index = 0;
   data_test.label_index = 0;

   Vector *inputs = malloc(sizeof(Vector) + sizeof(double)*2);
   inputs->size = 2;
   inputs->vals[0] = 1;
   inputs->vals[1] = 2;

   data_test.cur_image = (Vector *)malloc(sizeof(Vector) 
                    + sizeof(double) * (data_test.image_rows * data_test.image_cols));
   data_test.cur_label = -1; // no data yet  


   return 1;
}

void free_mnist_test()
{
   free(data_test.cur_image);
   fclose(data_test.label_file);
   fclose(data_test.image_file);
}

Vector *getNextImageTest()
{
   for (int i = 0; i < data_test.image_rows * data_test.image_cols; ++i)
   {
      data_test.cur_image->vals[i] = ((double) fgetc(data_test.image_file))/256.0;
   }
   data_test.cur_image->size = 724;
   data_test.image_index++;
   return data_test.cur_image;
}

int getNextLabelTest(){
   data_test.cur_label = (int) fgetc(data_test.label_file);
   return data_test.cur_label;
}


// int main()
// {
//    // init
//    load_mnist_init_test();

//    // call api
//    Vector *inputs = getNextImageTest();
//    int label = getNextLabelTest();
   
//    // check output
//    printf("%d\n", label);
//    for (int i = 0; i < 784; ++i)
//    {
//       double inp = inputs->vals[i];
//       printf("%f\n", inp);
//    }

//    // free data
//    free_mnist_test();

//    // init
//    load_mnist_init_train();

//    // call api
//    inputs = getNextImageTrain();
//    label = getNextLabelTrain();
   
//    // check output
//    printf("%d\n", label);
//    for (int i = 0; i < 784; ++i)
//    {
//       double inp = inputs->vals[i];
//       printf("%f\n", inp);
//    }

//    // free data
//    free_mnist_train();
//    return 0;
// }


