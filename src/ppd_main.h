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

#include <stdio.h>
#include <stdlib.h>
#include "ppd_utility.h"
#include "ppd_menu.h"
#ifndef PPD_MAIN
#define PPD_MAIN
/**
 * @file ppd_main.h this file holds the rest of the application together.
 * From these header files, main() can call all functions refered to 
 * in the header files included. You might also want to insert here
 * any functions for handling and validation of command line arguments
 **/

/**
 * datatype to represent a boolean value within the system has been moved to 
 * a bool.h file, so that BOOLEAN could be included in every file.
 **/

/**
 * The maximum amount of command line arguments allowed, including the
 * executable file 
 **/
#define NUMARGS 3

/** 
 * The extra characters appended by fgets function
 **/
#define EXTRACHARS 2
#endif
