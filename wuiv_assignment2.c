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

// Prints the highest rated movies of each year in order of oldest to newest movie
void showHighestRatedMovies(struct movie* list) {
  // Stores pointers to each year 
  struct movie* topRated[2022] = {NULL};

  // Iterate each movie and stores it
  while (list != NULL){
    int inputYear = list->year;
    // If it is first movie of that year, or movie is rated higher than other movie of same year, store it
    if (topRated[inputYear] == NULL || list->rating > topRated[inputYear]->rating) {
      topRated[inputYear] = list;
    }
    list = list->next;
  }

  // Prints movies in order. If NULL, theres no movie to print
  for (int i = 1900; i <= 2021; i++) {
    if (topRated[i] != NULL) {
        printf("%d %.1f %s\n", topRated[i]->year, topRated[i]->rating, topRated[i]->title);
    }
  }
}

void showMoviesByLanguage(struct movie* list, char* language){
  int matchfound = 0;

  // Iterate through movie list
  while (list != NULL) {
    char temp[256];
    strcpy(temp, list->languages);

    // Seperate languages with brackets and semicolons as delimiters 
    char* token = strtok(temp, "[];");
    
    // Check movie's languages for input language for match
    while (token != NULL){
      if (strcmp(token, language) == 0){
        printf("%d %s\n", list->year, list->title);
        matchfound = 1;
        break;
      }
      // Get the next language a movie has
      token = strtok(NULL, "[];");
    }
    list = list->next;
  }

  // Error message if no matches
  if (!matchfound) {
    printf("No data about movies released in %s\n", language);
  }
}

// Free allocated memory of each movie in linked list
void freeMovies(struct movie* list){
  while (list != NULL){
      struct movie* temp = list;
      list = list->next;
      free(temp->title);
      free(temp->languages);
      free(temp);
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

  // Copy each line and store into linked list
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
  printf("Processed file %s and parsed data for %d movies\n", filePath, count);

  //--------------------------------------------------------------------------
  // Display menu for user

  int userInput;

  do{
    printf("\n1. Shows movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");

    printf("\nEnter a choice from 1 to 4: ");
    scanf("%d", &userInput);

    if (userInput == 1){
      int year;
      printf("Enter the year for which you want to see movies: ");
      scanf("%d", &year);
      showMoviesByYear(head, year);

    } else if (userInput == 2) {
      showHighestRatedMovies(head);

    } else if (userInput == 3) {
      char language[21];
      printf("Enter the language for which you want to see movies: ");
      scanf("%s", language);
      showMoviesByLanguage(head, language);
    } else if (userInput == 4){

    } else{
      printf("You entered an incorrect choice. Try again.\n");
    }
  } while (userInput != 4);
  freeMovies(head);
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
//Mem leak test: valgrind --leak-check=yes --show-reachable=yes ./wuiv_assignment2 movies_sample_1.csv