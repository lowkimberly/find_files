/*
  Searches for files of a certain size, or with a certain substring, using
  Windows api.
*/

#include <stdio.h>
#include <strings.h>
#include <windows.h>

int errors_exist(int argc, char *argv[])
{
  LPWIN32_FIND_DATA data_buf=malloc(sizeof(LPWIN32_FIND_DATA));
  HANDLE dir_exist = FindFirstFile(argv[1], data_buf);
  if (dir_exist == INVALID_HANDLE_VALUE)
    {
      printf("Can't find the directory.\n");
      return 1;
    }
  if (argc!=4)
    {
      printf("Incorrect number of arguments.\n");
      return 1;
    }  
  if (strcmp(argv[2],(char *)"-l")!=0 && strcmp(argv[2],(char *)"-s")!=0)
    {
      printf("I don't recognize that flag.\n");
      return 1;
    }
  if (!strcmp(argv[2],(char *)"-l"))
    {
      int i;
      for(i = 0;i < strlen(argv[3]);i++)
        {
          if (IsCharAlpha(argv[3][i])) { printf("That's not an integer size!\n"); return 1; }
        }
    }
  return 0;
}

int search_size(char* current_dir, int min_bytes)
{
  //Get everything in this directory
  char searchname[255];
  strcpy(searchname,current_dir);
  strcat(searchname,(char *) "/*");
  
  //POINTER is malloced the size of NONPOINTER data
  LPWIN32_FIND_DATA data_buf=malloc(sizeof(WIN32_FIND_DATA));
  HANDLE dir_exist = FindFirstFile(searchname, data_buf);
  do
    {
      char longname[255];
      strcpy(longname,current_dir);
      strcat(longname,"/");
      strcat(longname,data_buf->cFileName);      
      //Directory recursion
      if ( strcmp(data_buf->cFileName,(char*)".")==0 || strcmp(data_buf->cFileName,(char*)"..")==0)
	{
	  //empty
	}
      else if((data_buf->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
	{
	  search_size(longname,min_bytes);
	}
      else if (data_buf->nFileSizeLow >= min_bytes)
	{
	  printf("%s\n",longname);
	}      
    }
  //is there still one more file? Keep going.
  while (FindNextFile(dir_exist, data_buf));

  return 0;
}


int search_name(char* current_dir, char* search_string)
{
  //Get everything
  char searchname[255];
  strcpy(searchname,current_dir);
  strcat(searchname,(char *) "/*");
  
  //POINTER is malloced the size of NONPOINTER data
  LPWIN32_FIND_DATA data_buf=malloc(sizeof(WIN32_FIND_DATA));
  HANDLE dir_exist = FindFirstFile(searchname, data_buf);
  do
    {
      char longname[255];
      strcpy(longname,current_dir);
      strcat(longname,"/");
      strcat(longname,data_buf->cFileName);      
      //Directory recursion
      if ( strcmp(data_buf->cFileName,(char*)".")==0 || strcmp(data_buf->cFileName,(char*)"..")==0)
	{
	}
      else if((data_buf->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)!=0)
	{
	  search_name(longname,search_string);
	}
      else if (strstr(data_buf->cFileName,search_string) != NULL)
	{
	  printf("%s\n",longname);
	}
    }
  //is there still one more file? Keep going.
  while (FindNextFile(dir_exist, data_buf));
  return 0;
}


int main(int argc, char *argv[])
{
  if(errors_exist(argc,argv)==1) { return 1; }
  else if (strcmp(argv[2],(char*)"-l") == 0)  { search_size(argv[1], atoi(argv[3])); }
  else if (strcmp(argv[2],(char*)"-s") == 0)  { search_name(argv[1], argv[3]); }
  return 0;
}


