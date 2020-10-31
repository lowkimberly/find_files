/*

   This program has two parts, searching for files larger or equal to a certain
   size, and files with a particular string in the name. There is also a third
   function, erros_exist, which runs at the beginning and attempts to make sure
   you entered all the arguments properly.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

int errors_exist(int argc, char *argv[])
{
  if (!opendir(argv[1]))
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
      for(i = 0;i < strlen(argv[2]);i++)
        {
          if (isalpha(argv[2][3])) { printf("That's not an integer size!\n"); return 1; }
        }
    }
  return 0;
}


int search_size(char* current_dir, int min_bytes)
{
  DIR* ptr_directory = opendir(current_dir);
  if (ptr_directory == NULL) { printf("Could not open directory.\n"); return 1; }
  struct dirent* ptr_dirent;
  struct stat* ptr_stats = malloc(sizeof(struct stat));
  if (ptr_stats == NULL) { printf("Could not allocate memory.\n"); return 1; }

  while ((ptr_dirent = readdir(ptr_directory))!=NULL)
    {
      //take the name of the file and stick the stats in ptr_stats
      char longname[255];
      strcpy(longname,current_dir);
      strcat(longname,"/");
      strcat(longname,ptr_dirent->d_name);
      stat(longname, ptr_stats);

      //ignore the dot files
      if (strcmp(ptr_dirent->d_name,(char*)".")==0 || strcmp(ptr_dirent->d_name,(char*)"..")==0)
	{
	}

      //is it a file of the right size? print.
      else if (S_ISREG(ptr_stats->st_mode) && (ptr_stats->st_size >= min_bytes))
	{
	  printf("%s\n",longname);
	}

      //is it a directory? Then we want to do recursion
      else if (S_ISDIR(ptr_stats->st_mode))
	{
	  search_size(longname, min_bytes);
	}
    }
  free(ptr_stats);
  closedir(ptr_directory);
  return 0;
}


int search_name(char* current_dir, char* search_string)
{
  DIR* ptr_directory = opendir(current_dir);
  if (ptr_directory == NULL) { printf("Could not open directory.\n"); return 1; }
  struct dirent* ptr_dirent;
  struct stat* ptr_stats = malloc(sizeof(struct stat));
  if (ptr_stats == NULL) { printf("Could not allocate memory.\n"); return 1; }

  while ((ptr_dirent = readdir(ptr_directory))!=NULL)
    {
      //take the name of the file and stick the stats in ptr_stats
      char longname[255];
      strcpy(longname,current_dir);
      strcat(longname,"/");
      strcat(longname,ptr_dirent->d_name);
      stat(longname, ptr_stats);

      //ignore the dot files
      if (strcmp(ptr_dirent->d_name,(char*)".")==0 || strcmp(ptr_dirent->d_name,(char*)"..")==0)
	{
	}

      //if a file and substring in name, print it
      else if (S_ISREG(ptr_stats->st_mode) && (strstr(ptr_dirent->d_name,search_string) != NULL))
	{
	  printf("%s\n",longname);
	}

      //if directory, recursively check what's inside
      else if (S_ISDIR(ptr_stats->st_mode))
	{
	  search_name(longname, search_string);
	}
    }

  free(ptr_stats);
  closedir(ptr_directory);
  return 0;
}


int main(int argc, char *argv[])
{
  if(errors_exist(argc,argv)==1) { return 1; }
  else if (strcmp(argv[2],(char*)"-l") == 0)  { search_size(argv[1], atoi(argv[3])); }
  else if (strcmp(argv[2],(char*)"-s") == 0)  { search_name(argv[1], argv[3]); }

  return 0;
}


