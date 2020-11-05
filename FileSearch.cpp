#include <iostream>
#include <vector>

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
  tdata = new struct thread_data[N_THREADS];

  unsigned int pattern_len = strlen(pattern);
  unsigned int text_len = strlen(this->file_data);
  unsigned int block_sz = (text_len / N_THREADS);
  vector<unsigned int> positions;

  // Initialize mutex before passing.
  pthread_mutex_init(&(this->mutex), NULL);

  // Create all the threads and let them run.
  for (int i = 0; i < N_THREADS; i++)
  {
    tdata[i].file_data = this->file_data;
    tdata[i].pattern = pattern;
    tdata[i].start_pos = i * block_sz;
    tdata[i].end_pos = tdata[i].start_pos + block_sz;
    tdata[i].pattern_len = pattern_len;
    tdata[i].positions = &positions;
    tdata[i].mutex = &(this->mutex);

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

  // Destroy the mutex.
  pthread_mutex_destroy(&(this->mutex));

  delete tdata;

  // Print results
  for (int i = 0; i < positions.size(); i++)
  {
    cout << "Found at " << positions[i] << "." << endl;
  }
}

/**
 * Search thread
 */
void * FileSearch::search_thread(void * data)
{
  struct thread_data * tdata = (struct thread_data *)data;

  char * begin = (tdata->file_data + tdata->start_pos);

  vector<unsigned int> local_positions;

  search_rk(begin, (tdata->end_pos - tdata->start_pos), tdata->pattern, tdata->pattern_len, local_positions);

  for (int i = 0; i < local_positions.size(); i++)
  {
    pthread_mutex_lock(tdata->mutex);
    tdata->positions->push_back(tdata->start_pos + local_positions[i]);
    pthread_mutex_unlock(tdata->mutex);
  }

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
