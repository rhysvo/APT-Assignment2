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
#include "ppd_stock.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/

 struct ppd_node * load_item_file(char * line) { 
    char *token, *price_token, *endPtr = NULL, *strPtr1 = NULL, *strPtr2 = NULL;   
    struct ppd_node *current_node;

    /**
     * Allocate neccasary memory. If allocation fails, the node is reutrned in
     * its current state, as a void pointer. This is checked in the load_data
     * function.
     **/
    current_node = malloc(sizeof(struct ppd_node));
    current_node->data = malloc(sizeof(struct ppd_stock));
    if(current_node == NULL || current_node->data == NULL) {
        printf("Error: Failure to allocate memory\n");
        free(current_node->data);
        free(current_node);
        return current_node;
    }
    
    token = strtok_r(line, STOCK_DELIM, &strPtr1);
    if(token == NULL || strlen(token) > IDLEN) {
        printf("Error: could not read in stock file.\n");
        return FALSE;
    }
    strcpy(current_node->data->id, token);
    
    token = strtok_r(NULL, STOCK_DELIM, &strPtr1);
    strcpy(current_node->data->name, token);
    if(token == NULL || strlen(token) > NAMELEN) {
        printf("Error: could not read in stock file.\n");
        return FALSE;
    }
    token = strtok_r(NULL, STOCK_DELIM, &strPtr1);
    strcpy(current_node->data->desc, token);
    if(token == NULL || strlen(token) > DESCLEN) {
        printf("Error: could not read in stock file.\n");
        return FALSE;
    }
    token = strtok_r(NULL, STOCK_DELIM, &strPtr1);
    if(token == NULL || strlen(token) > PRICELEN) {
        printf("Error: could not read in stock file.\n");
        return FALSE;
    }
    /**
     * It was neccessary to use strtok_r to tokenize the stock line, as we 
     * needed to tokenize a token. 
     **/
    price_token = strtok_r(token, PRICE_DELIM, &strPtr2);
    if(price_token == NULL || strlen(price_token) > DOLLARLEN) {
        printf("Error: could not read in stock file.\n");
        return FALSE;
    }
    current_node->data->price.dollars = (int) strtol(price_token, &endPtr, 10);
    if(endPtr == NULL) {
        printf("Error: could not proccess Items price. dollars\n");
        free(current_node->data);
        free(current_node);
        current_node = NULL;
        return current_node;
    }
    
    price_token = strtok_r(NULL, PRICE_DELIM, &strPtr2);
    if(price_token == NULL || strlen(price_token) > CENTLEN) {
        printf("Error: could not read in stock file.\n");
        return FALSE;
    }
    current_node->data->price.cents = (int) strtol(price_token, &endPtr, 10);
    if(endPtr == NULL) {
        printf("Error: could not proccess Items price.\n");
        free(current_node->data);
        free(current_node);
        current_node = NULL;
        return current_node;
    }

    token = strtok_r(NULL, STOCK_DELIM, &strPtr1);
    current_node->data->on_hand = (int) strtol(token, &endPtr, 10);
    if(endPtr == NULL) {
        printf("Error: could not proccess stock on hand of item.\n");
        free(current_node->data);
        free(current_node);
        current_node = NULL;
        return current_node;
    }

    return current_node;
}

BOOLEAN item_search(struct ppd_list *list, char *ID_to_search, 
        int *dollars_to_pay, int *cents_to_pay) {
    struct ppd_stock search;
    struct ppd_node *current, *prev = NULL;
    
    /**
     *  Setting memory of search node to 0 to avoid leftover data in memory
    **/
    memset(&search, 0, sizeof(struct ppd_stock));
    strcpy(search.id, ID_to_search);
    if(list->count == 0) {
        printf("Error: list size is 0, you cannot delete nothing!\n");
        return FALSE;
    }
    
    /** 
     *  Looping through linked list
    **/
    current =list->head;
    while(current != NULL && (item_cmp_id(current->data, &search)) != 0) 
    {
        prev = current;
        current = current->next;
    }
    /** 
     *  Check if item has been found.
    **/
    if(current == NULL) {
        printf("Error: Item not found in list\n");
        return FALSE;
    }

    printf("Success: Item found.\n\n");
   
    /**
     *  Setting the int pointers passed in to the cost of the item.
     *  Neccesary so the options 
    **/
    *dollars_to_pay = current->data->price.dollars;
    *cents_to_pay = current->data->price.cents;
    
    printf("%s|%s|%s\n", current->data->id, current->data->name, 
            current->data->desc);
    return TRUE;
}

BOOLEAN item_purchase(struct ppd_system *system, char *ID_to_purchase) {
    struct ppd_stock search;
    struct ppd_node *current, *prev = NULL;
    
    memset(&search, 0, sizeof(struct ppd_stock));
    strcpy(search.id, ID_to_purchase);
     
    current = system->item_list->head;
    while(current != NULL && item_cmp_id(current->data, &search) != 0) {
        prev = current;
        current = current->next;
    }

    if(current == NULL) {
        printf("Error: Item not found in list\n");
        return FALSE;
    }
    if(current->data->on_hand < 1) {
        printf("Error: None of this item is left in stock.\n");
        return FALSE;
    }
    printf("Dispensing: %s|%s|%s\n", current->data->id, current->data->name, 
            current->data->desc);
    --current->data->on_hand;
    return TRUE;
}

BOOLEAN list_add_node(struct ppd_list *list, struct ppd_node * newNode) {
    struct ppd_stock search;
    struct ppd_node *current, *prev = NULL;

    memset(&search, 0, sizeof(struct ppd_stock));
    strcpy(search.name, newNode->data->name);  
   
    current = list->head;

    /* List empty: Inserting at the start of the list. */
    if(list->head == NULL) {
        list->head = newNode;
        newNode->next = NULL;
        ++list->count;
        return TRUE;
    }

    /** 
     *  Loop through linked list until either the comparing function is 
     *  satisfied, or the end of the list is reached
    **/
    while(current != NULL && item_cmp(current->data, &search) < 0) {
        prev = current;
        current = current->next;
    }

    /* Inserting item to the end of the list */
    if(current == NULL) {
        newNode->next = NULL;
        prev->next = newNode;
        ++list->count;
        return TRUE;
    }

    /* Check for duplicates */
    if(item_cmp(current->data, &search) == 0) {
        printf("Error: Duplicate item found!\n");
        return FALSE;
    }
    
    /* Inserting Item at the start of the list */
    if(prev == NULL) {
        newNode->next = list->head;
        list->head = newNode;
    } 
    /* Inserting item in the middle of the list */   
    else {
        newNode->next = current;
        prev->next = newNode;
    }
    ++list->count;
    return TRUE;
}

BOOLEAN list_delete_node(struct ppd_list * list, char * itemId) {
    struct ppd_stock search;
    struct ppd_node *current, *prev = NULL;
    
    /**
     *  To avoid any stay data found memory, a search node is created. An extra 
     *  precaution is setting all memory to 0, and the prev node pointer to 
     *  NULL.
    **/
    memset(&search, 0, sizeof(struct ppd_stock));
    strcpy(search.id, itemId);

    /**
     *  If statement to check the size of the List. Will only be entered if
     *  there is nothing left in the list, or there was a problem loading data
     *  into the list that was not caught. 
    **/
    if(list->count == 0) {
        printf("Error: list size is 0, you cannot delete nothing!\n");
        return FALSE;
    }
    
    /* Loop through list head */
    current =list->head;
    while(current != NULL && item_cmp_id(current->data, &search) != 0) {
        prev = current;
        current = current->next;
    }

    /** 
     *  Case for item not found in the list. As the while loop will only break
     *  if the end of the list is reached or the item search function returns
     *  0, the only time current will == NULL is when the function never 
     *  return 0.
    **/
    if(current == NULL) {
        printf("Error: Item not found in list\n");
        return FALSE;
    }

    /** 
     *  Case for deleting from the start of the list.
    **/
    if(prev == NULL) {
        list->head = list->head->next;
    }

    /** 
     *  Case for deleting from the middle or end of the list.
    **/
    else {
        prev->next = current->next;
    }
     printf("%s|%s|%s has succusfully been deleted.\n", current->data->id, 
        current->data->name, current->data->desc);
    free(current->data);
    free(current);
    --list->count;
   
    return TRUE;
}

int item_cmp(struct ppd_stock *a, struct ppd_stock *b) {
    int cmp;
    
    cmp = strcmp(a->name, b->name);
    return cmp;
}

int item_cmp_id(struct ppd_stock *a, struct ppd_stock *b) {
    int cmp;
    
    cmp = strcmp(a->id, b->id);
    return cmp;
}
