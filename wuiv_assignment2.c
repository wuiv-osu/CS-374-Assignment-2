#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct movie {
  char* title;
  int year;
  char* languages;
  float rating;
  struct movie* next;
};

struct movie* createMovie(char* line) {
  struct movie* newMovie = malloc(sizeof(struct movie));
  char* token;
  char* rest = line;

  // Title
  token = strtok_r(rest, ",", &rest);
  newMovie->title = malloc(strlen(token) + 1);
  strcpy(newMovie->title, token);
}

void processMovieFile(char* filePath){
  char* currLine = NULL;
  size_t len = 0;
  size_t nread;
  int count = 0;
  struct movie* head = NULL;
  struct movie* tail = NULL;

  FILE *movieFile = fopen(filePath, "r");
  

  // Skipping header
  getline(&currLine, &len, movieFile);

  while(nread = getline(&currLine, &len, movieFile) != -1){
    struct movie* m = createMovie(currLine);
    count++;
    if (head == NULL) {
        head = m;
        tail = m;
    } else {
        tail->next = m;
        tail = m;
    }
  }
  free(currLine);
  fclose(movieFile);
  printf("Processed file %s and parsed data for %d movies\n\n", filePath);
}

int main ( int argc, char **argv ){
  if (argc < 2)
  {
      printf("You must provide the name of the file to process\n");
      printf("Example usage: ./movies movies.csv\n");
      return EXIT_FAILURE;
  }
  processMovieFile(argv[1]);
  return EXIT_SUCCESS;
}