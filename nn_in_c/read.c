#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>

Vector *what()
{
   char ch;
   FILE *fp;
   char *file_name = "mnist_data/train-images-idx3-ubyte";
   fp = fopen(file_name,"r"); // read mode
 
   if( fp == NULL )
   {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
   }
 
   printf("The contents of %s file are :\n", file_name);
 
   while( ( ch = fgetc(fp) ) != EOF )
      printf("%x",ch);
 
   fclose(fp);
   return 0;
}

Vector *readfile()
{
   FILE *fileptr;
   char *buffer;
   long filelen;

   fileptr = fopen("mnist_data/testing", "rb");  // Open the file in binary mode
   fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
   filelen = ftell(fileptr);             // Get the current byte offset in the file
   rewind(fileptr);                      // Jump back to the beginning of the file

   buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
   fread(buffer, filelen, 1, fileptr); // Read in the entire file
   fclose(fileptr); // Close the file
   for (int i = 0; i < filelen; ++i)
   {
      /* code */
      printf("%x", *(buffer + i));
   }
   return 0;
}             
int main()
{
   readfile();
   return 0;
}