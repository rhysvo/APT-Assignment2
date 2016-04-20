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

#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
    struct ppd_node *current;
    current = system->item_list->head;
    printf("Items Menu\n");
    printf("----------\n");
    printf("ID   |Name\t\t\t| Available | Price\n");
    printf("---------------------------------------------------\n");
    while(current != NULL) {
        printf("%s|%-26s|%-11d|$%d.%.2d\n", current->data->id, 
            current->data->name, current->data->on_hand, 
            current->data->price.dollars, current->data->price.cents);
        current = current->next;
    }
    printf("\n");
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
    char idInput[IDLEN + EXTRA_CHARS], costInput[PRICELEN + EXTRA_CHARS],
         denomName[PRICE_VALUE_LEN], **endPtr = NULL;
    int i, j, dollarsToPay, centsToPay, totalToPay = 0, moneyInserted = 0, 
        change = 0, moneyForCashReg[NUM_DENOMS], moneyForChange[NUM_DENOMS];
    enum denomination registerArrayPos;
    enum denomination_cent_value centValue;
    BOOLEAN inputSuccess = FALSE, hasChange = FALSE, hasInsertedMoney = FALSE;

    /**
     * memset the arrays uses to track money inserted and change to give, to
     * avoid leftover data in memory giving unexpected values.
     **/
    memset(moneyForCashReg, 0, sizeof(moneyForCashReg));
    memset(moneyForChange, 0, sizeof(moneyForChange));

    /**
     * The while loop which prompts user for the ID of the item they wish to
     * purchase
     **/
    printf("Please enter the ID of the item you would like to purchase: ");
    while(!inputSuccess) {
        if(fgets(idInput, IDLEN + EXTRA_CHARS, stdin) == NULL) {
            printf("Returning to the menu.\n");
            return FALSE;
        }
        if (idInput[0] == '\n') {
            printf("Returning to the menu.\n");
            return FALSE;
        }
        else if(idInput[strlen(idInput)-1] != '\n') {
            printf("Error: input to long.\n");
            printf("Please enter the letter 'I' followed by 4 digits: ");
            read_rest_of_line();
            continue;
        }
        if(idInput[0] != 'I') {
            printf("Error: incorrect ID format\n");
            printf("Please enter the letter 'I' followed by 4 digits: ");
            continue;
        }
        idInput[strlen(idInput)-1] = '\0';
        if(item_search(system->item_list, idInput, &dollarsToPay, 
            &centsToPay) == FALSE) {
            printf("Please enter an existing ID: ");
            continue;
        }
        inputSuccess = TRUE;
    }
    inputSuccess = FALSE;

    printf("Please hand over: $%d.%.2d \n", dollarsToPay, centsToPay);
    printf("Maximum denomination accepted is: 10 dollars (1000 cents)");
    printf(" at a time.\n");
    printf("Press crtl + d or enter on a new line at any time to cancel.\n");
    printf("Please enter the value of each note/coin in cents: ");
    totalToPay = ((dollarsToPay * 100) + centsToPay);

    /**
     * The loop which promts the user to input the money needed to purchase the
     * item. 
     **/
    while(totalToPay != 0) {
        if(fgets(costInput, PRICELEN + EXTRA_CHARS, stdin) == NULL) {
            printf("Returning to Menu.\n");
            if(hasInsertedMoney) {
                    refund_money(moneyForCashReg);
            }
            return FALSE;
        }
        if(costInput[0] == '\n') {
            printf("Returning to menu.\n");
            if(hasInsertedMoney) {
                printf("Refunding Money Inserted: ");
                refund_money(moneyForCashReg);
            }
            return FALSE;
        }
        if(costInput[strlen(costInput)-1] != '\n') {
            printf("Error: input too long\n");
            printf("Please enter an ID less than %d characters long", PRICELEN);
            read_rest_of_line();
            continue;
        }
        
        /**
         * Checks if money inserted is a valid coin/note. If it is, adds the 
         * coin/note to the array, and continues depending on whether money
         * is still owed or not
         **/
        moneyInserted = (int)strtol(costInput, endPtr, 10);
        switch(moneyInserted) {
            case CENT_FIVE_CENTS:
            case CENT_TEN_CENTS:
            case CENT_TWENTY_CENTS:
            case CENT_FIFTY_CENTS:
            case CENT_ONE_DOLLAR:
            case CENT_TWO_DOLLARS:
            case CENT_FIVE_DOLLARS:
            case CENT_TEN_DOLLARS:

            if(moneyInserted % CENT_FIVE_CENTS == 0) {
                if(moneyInserted < totalToPay) {
                    registerArrayPos = id_to_denom(moneyInserted);
                    ++moneyForCashReg[registerArrayPos];
                    totalToPay -= moneyInserted;
                    hasInsertedMoney = TRUE;
                    printf("You still owe: %d cents.\n", totalToPay);
                    printf("Please insert more money: ");
                    continue;

                }
                else if(moneyInserted == totalToPay) {
                    registerArrayPos = id_to_denom(moneyInserted);
                    ++moneyForCashReg[registerArrayPos];
                    moneyInserted = 0;
                    totalToPay = 0;
                    change = 0;
                    hasInsertedMoney = TRUE;
                    break;
                }
                else if(moneyInserted > totalToPay) {
                    registerArrayPos = id_to_denom(moneyInserted);
                    ++moneyForCashReg[registerArrayPos];
                    moneyInserted -= totalToPay;
                    change = moneyInserted;
                    hasChange = TRUE;
                    hasInsertedMoney = TRUE;
                    totalToPay = 0;
                    break;
                }
             } 
            else {
                printf("Sorry, that is not a recognised Australian Currency\n");
                printf("Please enter a valid note/coin in cents: ");
            }
        }
        if(totalToPay == 0) {
            break;
        }
        printf("Sorry, that is not a recognised Australian Currency\n");
        printf("Please enter a valid note/coin in cents: \n");
    }

    /**
     * Cycles through the change value, starting from the highest denomination, 
     * and adds change to the array if it can, otherwise goes to the next lowest
     * denomination. If there is still change left after this process, the 
     * register does not have enough money to give change, the transaction is
     * cancelled, and the money inserted is refunded.
     **/
    if(change != 0) {
        for(i = NUM_DENOMS-1; i >= 0; --i) {
            if(change == 0) {
                break;
            }
            centValue = denom_to_ID(i);
            if(change >= centValue) {
                if((change / centValue) > system->cash_register[i].count) {
                    /* go to next lower denomination */
                    moneyForChange[i] += system->cash_register[i].count;
                    continue;
                }
                moneyForChange[i] = change / centValue;
                change = change % centValue;
            }
        }
        if(change != 0) {
            printf("Error: Not enough change to proccess sale.\n");
            printf("Refunding money inserted: ");
            if(hasInsertedMoney) {
                printf("Refunding Money Inserted: ");
                refund_money(moneyForCashReg);
            }
            printf("Returning to Menu.\n\n");
            return FALSE;
        }
    }

    /**
     * If the system cannot dispense the item, money is refunded and the user
     * is returned to the menu
     **/
    printf("\n");
    if(item_purchase(system, idInput) == FALSE) {
        if(hasInsertedMoney) {
            printf("Refunding Money Inserted: ");
            refund_money(moneyForCashReg);
        }
        printf("Returning to menu.\n\n");
        return FALSE;
    }

    /** 
     * Adds the money inserted to the systems cash register and deducts the 
     * change given from the register.
     **/
    for(i = 0; i < NUM_DENOMS; ++i) {
        system->cash_register[i].count += moneyForCashReg[i];
        system->cash_register[i].count -= moneyForChange[i];
    }

    /**
     * If change was needed to be given, this prints out the the change in the
     * specified format.
     **/
    if(hasChange) {
        printf("Thank you. Here is your item, and your change of %d.%02d: ", 
                moneyInserted/100, moneyInserted % 100);
        for(i= NUM_DENOMS-1; i >= 0; --i) {
            if(moneyForChange[i] != 0) {
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
                for(j = moneyForChange[i]; j > 0; --j) {
                    printf("%s ", denomName);
                }
            }
        }
    }
    printf("\n\n");
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    if(save_coin_file(system) == FALSE) {
        printf("Returning to menu.\n\n");
        return FALSE;
    }
    if(save_stock_file(system) == FALSE) {
        printf("Returning to menu.\n\n");
        return FALSE;
    }
    printf("Current data successfully saved to file.\n");
    printf("Now exitting the system\n\n");
    system_free(system);
    exit(EXIT_SUCCESS);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
    char idInput[IDLEN + EXTRA_CHARS], nameInput[NAMELEN + EXTRA_CHARS], 
    descInput[DESCLEN + EXTRA_CHARS], priceInput[PRICELEN + EXTRA_CHARS];
    char *token, **endPtr = NULL;
    int dollarsToSave, centsToSave, idToIncrement;
    BOOLEAN inputSuccess = FALSE;

    struct ppd_node *current;
    struct ppd_stock search;


    current = system->item_list->head;
    memset(&search, 0, sizeof(struct ppd_stock));
    strcpy(search.id, current->data->id);
    
    /**
     * Small loop to find the highest id and copies it.
     **/
    while(current != NULL) {
        if(item_cmp_id(current->data, &search) > 0) {
            strcpy(search.id, current->data->id);
        }
        current = current->next;
    }

    /**
     * Retrieves the number from the highest id and increments it by 1.
     **/
    token = strtok(search.id, ID_DELIM);
    
    idToIncrement = (int)strtol(token, endPtr, 10);
    ++idToIncrement;
    /**
     * Checks the ID, and for up to a maximum of 9999 ID's in the linked list,
     * adds the correct amount of 0's to the front, depending on its size.
     **/
    if(idToIncrement < 10) {

        sprintf(idInput, "I000%d", idToIncrement);
        printf("The new ID for your product is: %s\n", idInput); 
    }
    else if(idToIncrement < 100) {
        sprintf(idInput, "I00%d", idToIncrement);
        printf("The new ID for your product is: %s\n", idInput); 
    }
    else if(idToIncrement < 1000) {
        sprintf(idInput, "I0%d", idToIncrement);
        printf("The new ID for your product is: %s\n", idInput); 
    }
    else if(idToIncrement <= 9999) {
        sprintf(idInput, "I%d", idToIncrement);
        printf("The new ID for your product is: %s\n", idInput); 
    }
    else {
        printf("Error: can only have a total of 9999 items in the list\n");
        printf("Returning to menu\n");
        return FALSE;
    }
    token = NULL;

    /**
     * Loop until a name has been entered correctly.
     **/
    printf("Please enter the name of the item: ");
    printf("Press crtl + d or enter on a new line at any time to cancel.\n");
    while(!inputSuccess) {
        if(fgets(nameInput, NAMELEN + EXTRA_CHARS, stdin) == NULL) {
            printf("Returning to menu\n");
            return FALSE;
        }
        if(nameInput[0] == '\n') {
            printf("Returning to menu.\n");
            return FALSE;
        }
        if(nameInput[strlen(nameInput)-1] != '\n') {
            printf("Error: input too long\n");
            printf("Please enter a name less than %d characters long", NAMELEN);
            read_rest_of_line();
            continue;
        }
        nameInput[strlen(nameInput)-1] = '\0';
        inputSuccess = TRUE;
    }
    inputSuccess = FALSE;

    /**
     * Loop until a description has been entered correctly.
     **/
    printf("Please enter a description of the item you are adding: ");
    while(!inputSuccess) {
        if(fgets(descInput, DESCLEN + EXTRA_CHARS, stdin) == NULL) {
            printf("Returning to menu\n");
            return FALSE;
        }
        if(descInput[0] == '\n') {
            printf("Returning to menu.\n");
            return FALSE;
        }
        if(descInput[strlen(descInput)-1] != '\n') {
            printf("Error: input too long\n");
            printf("Please enter a description less than %d", DESCLEN);
            printf(" characters long");
            read_rest_of_line();
            continue;
        }
        descInput[strlen(descInput)-1] = '\0';
        inputSuccess = TRUE;
    }
    inputSuccess = FALSE;
    token = NULL;
    endPtr = NULL;

    /**
     * Loop until a price that can be made up by Australian currency, and is
     * less than $100.
     **/
    printf("The price of an item must be less than $100, seperated by a '.', ");
    printf("and two digits must be entered for the cents value.\n");
    printf("Please enter the price of the item you are adding: ");
    while(!inputSuccess) {
        if(fgets(priceInput, PRICELEN + EXTRA_CHARS, stdin) == NULL) {
            printf("Returning to menu\n");
            return FALSE;
        }
        if(priceInput[0] == '\n') {
            printf("Returning to menu.\n");
            return FALSE;
        }
        if(priceInput[strlen(priceInput)-1] != '\n') {
            printf("Error: input too long\n");
            printf("Please enter a name less than %d long", PRICELEN);
            read_rest_of_line();
            continue;
        }
        token = strtok(priceInput, CURRENCY_DELIM);
        dollarsToSave = (int)strtol(token, endPtr, 10);
        if(strlen(token) > 2) {
            printf("Error: price must be less than $100\n");
            printf("Please enter a price less than $100\n");
            continue;
        }

        token = strtok(NULL, CURRENCY_DELIM);
        centsToSave = (int)strtol(token, endPtr, 10);
        if(strlen(token) != 3 || (centsToSave % CENT_FIVE_CENTS) != 0) {
            printf("Error: You must enter exatly two digits for cents.\n");
            printf("Eg: 20.00, or 9.60\n");
            printf("Please enter a price less than $100 that conforms to ");
            printf("these standards\n");
            continue;
        }
        inputSuccess = TRUE;
    }

    /**
     * Allocate the memory for the new node, now that all information has been
     * gathered.
     **/
    current = malloc(sizeof(struct ppd_node));
    current->data = malloc(sizeof(struct ppd_stock));
    if(current == NULL || current->data == NULL) {
        printf("The memory alocation failed\n");
        printf("Returning to menu\n");
        free(current->data);
        free(current);
        return FALSE;
    }

    /**
     * Copy information into the node,
     **/
    current->next = NULL;
    strcpy(current->data->id, idInput);
    strcpy(current->data->name, nameInput);
    strcpy(current->data->desc, descInput);
    current->data->price.dollars = dollarsToSave;
    current->data->price.cents = centsToSave;
    current->data->on_hand = DEFAULT_STOCK_LEVEL;
    
    /**
     * Pass the node to the list_add_system function, check for problems, 
     * free memory if adding fails.
     **/
    if(list_add_node(system->item_list, current) == FALSE) {
        printf("The node failed to load\n");
        printf("This was not meant to happen\n");
        free(current->data);
        free(current);
        return FALSE;
    }
    printf("New item: %s|%s|%s|%d.%d|%d\n", idInput, nameInput, descInput, 
        dollarsToSave, centsToSave, DEFAULT_STOCK_LEVEL);
    printf("has been successfully added!\n");
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
    char input[IDLEN + EXTRA_CHARS];
    BOOLEAN inputSuccess = FALSE;

    /**
     * Loop until ID in correct format is found, and the ID is found in the
     * linked list.
     **/
    printf("Press crtl + d or enter on a new line at any time to cancel.\n");
    printf("Please enter the ID of the item you wish to delete: ");
    while(!inputSuccess) {
        if(fgets(input, IDLEN + EXTRA_CHARS, stdin) == NULL) {
            printf("Returning to the menu.\n");
            return FALSE;
        }
        if (input[0] == '\n') {
            printf("Returning to the menu.\n");
            return FALSE;
        }
        else if(input[strlen(input)-1] != '\n') {
            printf("Error: input to long.\n");
            printf("Please enter the letter 'I' followed by 4 digits: ");
            read_rest_of_line();
            continue;
        }
        if(input[0] != 'I') {
            printf("Error: incorrect ID format\n");
            printf("Please enter the letter 'I' followed by 4 digits: ");
            continue;
        }
        input[strlen(input)-1] = '\0';
        if(list_delete_node(system->item_list, input) == FALSE) {
            printf("Please enter an existing ID: ");
            continue;
        }
        else {
            printf("Success! returning to the menu\n");
            return TRUE;
        }
    }

    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
    struct ppd_node *current;

    current = system->item_list->head;
    while(current != NULL) {
        current->data->on_hand = DEFAULT_STOCK_LEVEL;
        current = current->next;
    }
    printf("All stock reset to: %d\n\n", DEFAULT_STOCK_LEVEL);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    int i;

    for(i = 0; i < NUM_DENOMS; ++i) {
        system->cash_register[i].count = DEFAULT_COIN_COUNT;
    }
    printf("Succes! Coin counts reset to %d\n\n", DEFAULT_COIN_COUNT);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
    int i;
    char denomName[MAX_DENOM_LEN];
    printf("Denomination\t|\tCount\n");
    printf("-----------------------------------\n");
    for(i = 0; i < NUM_DENOMS; ++i) {
        denom_to_Name(system->cash_register[i].denom, denomName);
        printf("%-16s|\t%d\n", denomName, system->cash_register[i].count);
    }
    printf("\n\n");
    return TRUE;
}

BOOLEAN abort_program(struct ppd_system *system) {
    system_free(system);
    exit(EXIT_SUCCESS);
    return FALSE;
}
