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

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bool.h"
#ifndef PPD_COIN
#define PPD_COIN

/**
 * The delimeter for the tokenizing functrion for the coin file
 **/
#define COIN_DELIM ","

/**
 * The Maximum length of the Coin line file, not including the
 * nul terminator character.
 **/
#define COIN_MAX_LINE_LEN 8

/**
 * The maximum length of the string representation of each 
 * denomination. E.g: 5 cents
 **/
#define MAX_DENOM_LEN 12

/**
 * The maximum length of the currenecy displayed when refunding or
 * displaying change. E.g: "5c"
 **/
#define PRICE_VALUE_LEN 4

struct ppd_system;

/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

enum denomination_cent_value {
    CENT_FIVE_CENTS = 5, CENT_TEN_CENTS = 10, CENT_TWENTY_CENTS = 20, 
    CENT_FIFTY_CENTS = 50, CENT_ONE_DOLLAR = 100, CENT_TWO_DOLLARS = 200,
    CENT_FIVE_DOLLARS = 500, CENT_TEN_DOLLARS = 1000
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

/**
 * Function to read data in from the coin file data. Function parameters
 * are the systems cash register struct, and the current line in the coin
 * file.
 **/
BOOLEAN load_coin_data(struct coin *cash_register, char * coin_line);

/**
 * Small function to convert from the cent value stored on file to the
 * correct postion in the Cash Register array.
 **/
enum denomination id_to_denom(int id);

/**
 * Small function to convert from the value of the Cash Register array key
 * to the correct value of the cenomination of the coin 
 **/
enum denomination_cent_value denom_to_ID(enum denomination denom);

/**
 * Small function to convert from the value of the Cash Register array key
 * to the correct Name of the given denomination
 **/
void denom_to_Name(enum denomination denom, char *denomName);
#endif
