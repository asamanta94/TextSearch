#include <iostream>

#include "FileSearch.h"
#include "Search.h"

#define FILE_PATH_ARG 1
#define PATTERN_ARG   2

string usage = "./ts <File Path> <String to search>";

FileSearch::FileSearch(char * file_name)
{
  this->file_name = file_name;
  file_data = NULL;
  file_size = 0;
}

/**
 * Read the contents of the file.
 */
void FileSearch::read_file()
{
  unsigned int char_pos = 0;

  FILE * file = fopen(file_name, "r");
  if (!file)
  {
    cout << "Either file doesn't exist or we don't have permissions." << endl;
    exit(-1);
  }

  // Get size of file
  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);

  // Reset the file pointer
  fseek(file, 0L, SEEK_SET);

  // Create character array of file size
  file_data = new char[file_size];

  // Read file data into the array
  while ((*(file_data + char_pos) = fgetc(file)) != EOF)
  {
     char_pos++;
  }

  // Add EOF to the data.
  *(file_data + char_pos) = '\0';

  fclose(file);
}

/**
 * Create threads for reading the same file.
 */
void FileSearch::search(char * pattern)
{
  tdata = new struct thread_data[2];

  // Create all the threads and let them run.
  for (int i = 0; i < 2; i++)
  {
    tdata[i].file_data = this->file_data;
    tdata[i].pattern = pattern;
    tdata[i].pos = i * 2;

    if (pthread_create(&(tdata[i].thread), NULL, FileSearch::search_thread, &(tdata[i])))
    {
      cout << "Failed to create thread." << endl;
    }
  }

  // Wait for all the threads to join.
  for (int i = 0; i < 2; i++)
  {
    pthread_join(tdata[i].thread, NULL);
  }

  delete tdata;
}

/**
 * Search thread
 */
void * FileSearch::search_thread(void * data)
{
  struct thread_data * tdata = (struct thread_data *)data;

  char * begin = (tdata->file_data + tdata->pos);
  char * pattern = tdata->pattern;

  for (int i = 0; i < 10; i++)
  {
    cout << *(begin + i);
  }

  search_rk(begin, pattern);

  cout << endl;

  return NULL;
}

FileSearch::~FileSearch()
{
}

int main(int argc, char * argv[])
{
  if (argc != 3)
  {
    cout << usage << endl;
    return -1;
  }

  FileSearch * fs = new FileSearch((char *)argv[FILE_PATH_ARG]);

  fs->read_file();
  fs->search((char *)argv[PATTERN_ARG]);

  delete fs;

  return 0;
}
