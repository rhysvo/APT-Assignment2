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

#include "ppd_utility.h"
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, 
const char * stock_name)
{   

    FILE * coin_file, * stock_file;
    char coin_line[COIN_MAX_LINE_LEN + 1], stock_line[STOCK_MAX_LINE_LEN + 1];
    struct ppd_node *newNode = NULL;
    struct coin *cashPtr = system->cash_register;
    
    /* check to make sure file names are not empty or NULL */
    if(strcmp(coins_name, " ") == 0 || coins_name == NULL) {
        printf("Error: coin file name not found\n");
        return FALSE;
    }
    if(strcmp(stock_name, " ") == 0 || coins_name == NULL) {
        printf("Error: stock file name not found\n");
        return FALSE;
    }
    
    /* load the coin file. */
    system->coin_file_name = coins_name;  
    
    coin_file = fopen(system->coin_file_name, "r");
    if(coin_file == NULL) {
        printf("Error: could not load coin file\n");
        return FALSE;
    }
    while(fgets(coin_line, COIN_MAX_LINE_LEN + 1, coin_file) != NULL) {
        if(load_coin_data(cashPtr, coin_line) == FALSE) {
            printf("Returning to menu\n");
            fclose(coin_file);
            return FALSE;
        }
    }
    printf("\n\n");
    printf("Coin data successfuly loaded in\n");
    fclose(coin_file);

    /* stock file handeling */
    system->stock_file_name = stock_name;
    stock_file = fopen(system->stock_file_name, "r");
    if(stock_file == NULL) {
        printf("Error: could not load stock file\n");
        return FALSE;
    }
    
    while(fgets(stock_line, STOCK_MAX_LINE_LEN + 1, stock_file) != NULL) {
        newNode = load_item_file(stock_line);
        if(newNode == NULL) {
            printf("Returning to menu.\n\n");
            fclose(stock_file);
            return FALSE;
        }
        if(list_add_node(system->item_list, newNode) == FALSE) {
            printf("Returning to menu.\n\n");
            return FALSE;
        }
    }
    printf("Stock file successfuly loaded in \n\n");
    fclose(stock_file);
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{   
    memset(system->cash_register, 0, sizeof(system->cash_register));
    system->item_list = malloc(sizeof(struct ppd_list));
    if(list_init(system->item_list) == FALSE) {
        printf("ERROR: List init did not work\n");
    }
    system->coin_file_name = NULL;
    system->stock_file_name = NULL;
    return TRUE;
}

BOOLEAN list_init(struct ppd_list *list) {
    list->count = 0;
    list->head = NULL;
    return list != NULL;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
    struct ppd_node *current, *next;
    current = system->item_list->head;
    while(current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(system->item_list);
}

BOOLEAN get_int_in_range(int lower, int higher, int *output) {
    char input[MAX_INT_INPUT], *endPtr = NULL;
    BOOLEAN inputSuccess = FALSE;
    while(!inputSuccess) {
        if(fgets(input, MAX_INT_INPUT, stdin) == NULL) {
            return FALSE;
        }
        if(input[0] == '\n') {
            return FALSE;
        }

        if(input[strlen(input)-1] != '\n') {
            printf("Error: input too long\n");
            printf("Please enter a interger between %d and %d\n", lower, 
                    higher);
            read_rest_of_line();
            continue;
        }
        input[strlen(input) -1] = '\0';
        *output = (int)strtol(input, &endPtr, 10);
        
        if(strcmp(endPtr, "") == 0) {
            if(*output < lower || *output > higher) {
            printf("Error: The value you entered was not in the required range\n");
            printf("Please enter a interger between %d and %d\n", lower, 
                    higher);
             continue;
            }
            else {
                inputSuccess = TRUE;
            }
        } 
    }
    return TRUE;  
}

BOOLEAN refund_money(int *moneyInsertedArr) {
    int i, j;
    char denomName[PRICE_VALUE_LEN];

    for(i = NUM_DENOMS-1; i >= 0; --i) {
        if(moneyInsertedArr[i] != 0) {
            switch(i) {
                case FIVE_CENTS:
                    strcpy(denomName, "5c");
                    break;
                case TEN_CENTS:
                    strcpy(denomName, "10c");
                    break;
                case TWENTY_CENTS:
                    strcpy(denomName, "20c");
                    break;
                case FIFTY_CENTS:
                    strcpy(denomName, "50c");
                    break;
                case ONE_DOLLAR:
                    strcpy(denomName, "$1");
                    break;
                case TWO_DOLLARS:
                    strcpy(denomName, "$2");
                    break;
                case FIVE_DOLLARS:
                    strcpy(denomName, "$5");
                    break;
                case TEN_DOLLARS:
                    strcpy(denomName, "$10");
                    break;
                default:
                    printf("Error: out of array bounds\n");
                    printf("Returning to menu.\n\n");
                    return FALSE;
            }
            for(j = moneyInsertedArr[i]; j > 0; --j) {
                printf("%s ", denomName);
            }
        }
    }
    printf("\n");
    return TRUE;
}

BOOLEAN save_stock_file(struct ppd_system *system) {
    struct ppd_node *current;
    FILE *stock_file;
    
    stock_file = fopen(system->stock_file_name, "w");
    if(stock_file == NULL) {
        printf("Error: There was a problem opening the stock file.\n");
        return FALSE;
    }
    current = system->item_list->head;
    while(current != NULL) {
        fprintf(stock_file, "%s%s%s%s%s%s%d%s%d%s%d\n", 
            current->data->id, 
            STOCK_DELIM,
            current->data->name,
            STOCK_DELIM,
            current->data->desc,
            STOCK_DELIM,
            current->data->price.dollars,
            PRICE_DELIM,
            current->data->price.cents,
            STOCK_DELIM,
            current->data->on_hand);
        current= current->next;
    }
    fclose(stock_file);
    return TRUE;
}

BOOLEAN save_coin_file(struct ppd_system *system) {
    FILE *coin_file;
    int i;
    enum denomination_cent_value centValue;

    coin_file = fopen(system->coin_file_name, "w");
    if(coin_file == NULL) {
        printf("Error: Failed to open coin file\n");
        return FALSE;
    }

    for(i = NUM_DENOMS-1; i >= 0; --i) {
        centValue = denom_to_ID(i);
        fprintf(coin_file, "%d%s%d\n", 
            centValue,
            COIN_DELIM,
            system->cash_register[i].count);
    }
    fclose(coin_file);
    return TRUE;
}
