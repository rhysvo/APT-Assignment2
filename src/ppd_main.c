/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Rhys Van Orsouw
 * Student Number   : s3542173
 * Course Code      : COSC 1076
 * Program Code     : BP 096 Software Eng  
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/

#include "ppd_main.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    char *stock = argv[1];
    char *coins = argv[2];
    int response;
    struct menu_item menu[NUMOPTIONS];
    BOOLEAN menuControl = FALSE;
    
    /* represents the data structures to manage the system */
    struct ppd_system system;
    /* validate command line arguments */
   if(argc != NUMARGS){
       printf("Error. Correct number of Arguments not passed in\n");
       printf("Correct arguments are: \n");
       printf("\t ./ppd <stockfile> <coinfile>\n");
       printf("Where <stockfile> and <coinfile> are two vaild files in the ");
       printf("expected format\n");
       return EXIT_FAILURE;
    }

    /* init the system */
    if (!system_init(&system)){
      printf("Error: System initialsation failed\n");
      system_free(&system);
      return EXIT_FAILURE;
    }
    printf("\n\n");

    /* load data */
    if(!load_data(&system, coins, stock)){
      printf("Exiting the system, freeing all memory\n");
      system_free(&system);
      return EXIT_FAILURE;
    }

    /* test if everything has been initialised correctly */
    if((sizeof(system.cash_register))/(sizeof(system.cash_register[0])) != 
          NUM_DENOMS) {
      printf("The internet lied to me\n");
    }

    /* initialise the menu system */
    init_menu(menu);
    /* loop, asking for options from the menu */
    while(!menuControl) {
      print_menu(menu);
      while(get_int_in_range(1, NUMOPTIONS, &response) != TRUE) {
      printf("Error: Please enter a number between 1 and %d\n", NUMOPTIONS);
    }

    /* decrease response by one for array index */
    --response;
    printf("\n");

    /* run each option selected */
    menu[response].function(&system);
    /* until the user quits */
    }
    /* make sure you always free all memory and close all files 
     * before you exit the program
     */
     system_free(&system);

    /**
     * EXIT_FAILURE is returned here as the program should never reach this 
     * point, as all exitting is done in the menu options.
     **/
    return EXIT_FAILURE;
}
