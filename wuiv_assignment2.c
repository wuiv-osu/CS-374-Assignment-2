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

// Parse through csv line, copy and return a movie struct
struct movie* createMovie(char* line){
  struct movie* newMovie = malloc(sizeof(struct movie));
  char* token;
  char* rest = line;

  // Title
  token = strtok_r(rest, ",", &rest);
  newMovie->title = malloc(strlen(token) + 1);
  strcpy(newMovie->title, token);

  // Year
  token = strtok_r(NULL, ",", &rest);
  newMovie->year = atoi(token);

  // Lnaguages
  token = strtok_r(NULL, ",", &rest);
  newMovie->languages = malloc(strlen(token) + 1);
  strcpy(newMovie->languages, token);

  // Rating
  token = strtok_r(NULL, "\n", &rest);
  newMovie->rating = strtof(token, NULL);

  newMovie->next = NULL;
  return newMovie;
}

// Display movies released in specific year
void showMoviesByYear(struct movie* list, int year){
  int matchfound = 0;

  // If matching year is found while going through linked list, print and set match variable to 1 to prevent error message
  while (list != NULL){
    if (list->year == year){
      printf("%s\n", list->title);
      matchfound = 1;
    }
    list = list->next;
  }

  // Error message if no matches
  if (!matchfound){
    printf("No data about movies released in the year %d\n", year);
  }
}

// Open csv, parse each movie and stores data in linked list
void processMovieFile(char* filePath){
  char* currentLine = NULL;
  size_t len = 0;
  size_t nread;
  int count = 0;
  struct movie* head = NULL;
  struct movie* tail = NULL;

  FILE *movieFile = fopen(filePath, "r");
  

  // Skipping header
  getline(&currentLine, &len, movieFile);

  while((nread = getline(&currentLine, &len, movieFile)) != -1){
    struct movie* newMovie = createMovie(currentLine);
    count++;

    if (head == NULL) {
        head = newMovie;
        tail = newMovie;
    } else {
        tail->next = newMovie;
        tail = newMovie;
    }
  }

  free(currentLine);
  fclose(movieFile);
  printf("Processed file %s and parsed data for %d movies\n\n", filePath, count);

  //--------------------------------------------------------------------------
  // Display menu for user

  int userInput;

  do{
    printf("\n1. Shows movies released in the specified year\n");

    printf("\nEnter a choice from 1 to 4: ");
    scanf("%d", &userInput);

    if (userInput == 1){
      int year;
      printf("Enter the year for which you want to see movies: ");
      scanf("%d", &year);
      showMoviesByYear(head, year);
    }
  } while (userInput != 4);






}

int main ( int argc, char **argv ){
  if (argc < 2){
      printf("You must provide the name of the file to process\n");
      printf("Example usage: ./movies movies.csv\n");
      return EXIT_FAILURE;
  }
  processMovieFile(argv[1]);
  return EXIT_SUCCESS;
}


//compile:   gcc -o wuiv_assignment2 wuiv_assignment2.c
//test line: ./wuiv_assignment2 movies_sample_1.csv 