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

#include "ppd_menu.h"
/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
	struct menu_item temp_array[NUMOPTIONS] =
	{
		{"Display Items", display_items}, 
		{"Purchase Item", purchase_item}, 
		{"Save and Exit", save_system}, 
		{"Add Item", add_item}, 
		{"Remove Item", remove_item}, 
		{"Display Coins", display_coins}, 
		{"Reset Stock", reset_stock}, 
		{"Reset Coins", reset_coins}, 
		{"Abort Program", abort_program}	
	};

	memcpy(menu, temp_array, sizeof(temp_array));
}

/**
 * @param menu the menu array, which is stored in the main.c file.
 **/
void print_menu(struct menu_item *menu) {
	int i;
	printf("Main Menu:\n");
	for(i = 0; i < NUMOPTIONS; ++i) {
		if(i == 3) {
			printf("Administrator-Only Menu: \n");
		}
		printf("\t%d. %s\n", i+1, menu[i].name);

	}
	printf("\n");
	printf("Select your option (1-9): \n");
}
