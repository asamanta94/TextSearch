#ifndef FILESEARCH_H
#define FILESEARCH_H

#include <string>

#include <stdio.h>
#include <pthread.h>

#define N_THREADS 10

using namespace std;

struct thread_data
{
  char * file_data;
  char * pattern;

  unsigned int start_pos;
  unsigned int end_pos;
  unsigned int pattern_len;

  pthread_t thread;
};

class FileSearch
{
  /* File information */
  char * file_name;
  char * file_data;
  unsigned int file_size;

  /* Thread information */
  struct thread_data * tdata;

public:

  FileSearch(char * file_name);
  ~FileSearch();

  void read_file();
  void search(char * pattern);

  static void * search_thread(void * data);
};

#endif
