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
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_coin.h"
#ifndef PPD_STOCK
#define PPD_STOCK
#include <stdlib.h>

/**
 * @file ppd_stock.h this file defines the data structures required to 
 * manage the stock list. You should add here the function prototypes for
 * managing the list here and implement them in ppd_stock.c
 **/

/**
 * The length of the id string not counting the nul terminator
 **/
#define IDLEN 5

/**
 * The maximum length of a product name not counting the nul terminator
 **/
#define NAMELEN 40

/**
 * The maximum length of a product description not counting the nul
 * terminator.
 **/
#define DESCLEN 255

/**
 * The maximum length of the price of a product, not counting the nul terminator
 **/
#define PRICELEN 5

 /**
 * The maximum length of the dollar section of a product, not counting the 
 * nul terminator
 **/
#define DOLLARLEN 2

 /**
 * The maximum length of the cent section of a product, not counting the 
 * nul terminator
 **/
#define CENTLEN 2

/**
 * The maximum total length of the stock line on file
 **/
 #define STOCK_MAX_LINE_LEN 300

/**
 * The default coin level to reset the coins to on request
 **/
#define DEFAULT_COIN_COUNT 20

/**
 * The default stock level that all new stock should start at and that 
 * we should reset to on restock
 **/
#define DEFAULT_STOCK_LEVEL 20

/**
 * The number of denominations of currency available in the system 
 **/
#define NUM_DENOMS 8

/**
 * a structure to represent a price. One of the problems with the floating
 * point formats in C like float and double is that they have minor issues
 * of inaccuracy due to rounding. In the case of currency this really is
 * not acceptable so we introduce our own type to keep track of currency.
 **/
struct price
{
    /**
     * the dollar value for some price
     **/
    unsigned dollars, 
             /**
              * the cents value for some price
              **/
             cents;
};

/**
 * data structure to represent a stock item within the system
 **/
struct ppd_stock
{
    /**
     * the unique id for this item
     **/
    char id[IDLEN + 1];
    /**
     * the name of this item
     **/
    char name[NAMELEN + 1];
    /**
     * the description of this item
     **/
    char desc[DESCLEN + 1];
    /**
     * the price of this item
     **/
    struct price price;
    /**
     * how many of this item do we have on hand? 
     **/
    unsigned on_hand;
    /**
     * a pointer to the next node in the list
     **/
};

/**
 * the node that holds the data about an item stored in memory
 **/
struct ppd_node
{
    /* pointer to the data held for the node */
    struct ppd_stock * data;
    /* pointer to the next node in the list */
    struct ppd_node * next;
};

/**
 * the list of products - each link is the list is a @ref ppd_node
 **/
struct ppd_list
{
    /**
     * the beginning of the list
     **/
    struct ppd_node * head;
    /**
     * how many nodes are there in the list?
     **/
    unsigned count;
};

/**
 * this is the header structure for all the datatypes that is 
 * passed around and manipulated
 **/
struct ppd_system
{
    /**
     * the container for all the money manipulated by the system
     **/
    struct coin cash_register[NUM_DENOMS];

    /**
     * the linked list - note that this is a pointer - how does that
     * change what we need to do for initialization of the list?
     **/
    struct ppd_list * item_list;

    /**
     * the name of the coin file - we need this for saving as all menu
     * items only take the one parameter of the ppd_system
     **/
    const char * coin_file_name;
    /**
     * the name of the stock file
     **/
    const char * stock_file_name;
};

/**
 * This was needed to use the strtok_r function
 **/

extern char *strtok_r(char *, const char *, char **);

/**
 * Function that takes in the current line passed to it by the fgets function.
 * Returns a struct ppd_node pointer, which is malloc'd and checked within the
 * function. 
 **/
struct ppd_node * load_item_file(char * line);

/** 
 * Small function that iterates over the linked list and returns TRUE, and the 
 * price of the item, if it is found. Otherwise, returns FALSE. 
 **/
BOOLEAN item_search(struct ppd_list *list, char *ID_to_search, 
        int *dollars_to_pay, int *cents_to_pay);

/** 
 * Small function very simmilar in working to the item_search function, which
 * re-iterates over the list and finds the item, but is called AFTER the payment
 * proccess is completed successfuly, and alters the value of stock on hand.
 **/
BOOLEAN item_purchase(struct ppd_system *system, char *ID_to_purchase);

/**
 * One of the core functions in linked lists, this function takes the parameters
 * of the System list, and an already malloc'd Node. This function assumes that
 * all data has been validated, and that the node passed to it has been 
 * succesfully malloc'd. 
 **/
BOOLEAN list_add_node(struct ppd_list *list, struct ppd_node * newNode);

/**
 * Another core function in linked lists, this function takes the parameters of
 * the system list, and only an ID to search for. This function iterates of the
 * linked list until either the end is reached, in which case FALSE is returned,
 * or until the id comparing function equals 0. This function then proceeds to 
 * free the data, then the node, and then returns TRUE. It should also be noted
 * that before iteration, the function checks if the list has size to save 
 * resources.
 **/
BOOLEAN list_delete_node(struct ppd_list * list, char * itemId);

/**
 * A small function that compares the names of two nodes passed to it, and 
 * returns the value of strcmp of thier names. 
 **/
int item_cmp(struct ppd_stock *a, struct ppd_stock *b);

/**
 * A small function that comapres the ids of two nodes passed to it, and returns
 * the value of strcmp of their ids.
 **/
int item_cmp_id(struct ppd_stock *a, struct ppd_stock *b);

/**
 * The Delimeter for the tokenizing function for the stock files.
 **/
#define STOCK_DELIM "|"

/**
 * The delimeter for the price of an item in the stock file
 **/
#define PRICE_DELIM "."

#endif


