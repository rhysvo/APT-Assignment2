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
#include "ppd_coin.h"

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/

/**
 * @param cash_register a pointer to the systems cash register array
 * @param coin_line the current line retrieved from the fgets function.
 **/
BOOLEAN load_coin_data(struct coin *cash_register, char * coin_line) {
	char * token = NULL, *endPtr = NULL;
	enum denomination struct_denom_input;
	int coin_input, file_denom_input;

	token = strtok(coin_line,COIN_DELIM); 
    file_denom_input= (int) strtol(token, &endPtr, 10);
    struct_denom_input = id_to_denom(file_denom_input);
    if(struct_denom_input == -1) {
        printf("Error: problem in reading in coin data values\n");
        printf("Denominatio read from file was not a recognised value.\n");
        return FALSE;
    }
    token = strtok(NULL, COIN_DELIM);
    
    coin_input = (int) strtol(token, &endPtr, 10);
    if(coin_input > 99) {
    	printf("Error: problem in reading in coin data values\n");
        printf("Amount of a coin (%d cents) was greater than 99 in count.\n",
        	file_denom_input);
        return FALSE;
    }

   if(coin_input < 0) {
   	printf("Error: Cannot accept negative values for coins.\n");
   	return FALSE;
   }

  	
    cash_register[struct_denom_input].denom = struct_denom_input;
    cash_register[struct_denom_input].count = coin_input;
    return TRUE;
}

/**
 * @param id the value, in cents, of the denomination stored on file.
 * example: 1000 returns 7, the enum for TEN_CENTS
 **/
enum denomination id_to_denom(int id) {
	switch(id) {
		case CENT_FIVE_CENTS:
			return FIVE_CENTS;
			break;

		case CENT_TEN_CENTS:
			return TEN_CENTS;
			break;

		case CENT_TWENTY_CENTS:
			return TWENTY_CENTS;
			break;

		case CENT_FIFTY_CENTS:
			return FIFTY_CENTS;
			break;

		case CENT_ONE_DOLLAR:
			return ONE_DOLLAR;
			break;

		case CENT_TWO_DOLLARS:
			return TWO_DOLLARS;
			break;

		case CENT_FIVE_DOLLARS:
			return FIVE_DOLLARS;
			break;

		case CENT_TEN_DOLLARS:
			return TEN_DOLLARS;
			break;

		default:
			return -1;
			break;	
	}
}

/**
 * @param denom the key to the Cash Register array.
 * example: Cash_register[7] returns 1000
 **/
enum denomination_cent_value denom_to_ID(enum denomination denom) {
	switch(denom) {
		case FIVE_CENTS:
			return CENT_FIVE_CENTS;
			break;
		case TEN_CENTS:
			return CENT_TEN_CENTS;
			break;
		case TWENTY_CENTS:
			return CENT_TWENTY_CENTS;
			break;
		case FIFTY_CENTS:
			return CENT_FIFTY_CENTS;
			break;
		case ONE_DOLLAR:
			return CENT_ONE_DOLLAR;
			break;
		case TWO_DOLLARS:
			return CENT_TWO_DOLLARS;
			break;
		case FIVE_DOLLARS:
			return CENT_FIVE_DOLLARS;
			break;
		case TEN_DOLLARS:
			return CENT_TEN_DOLLARS;
			break;
		default:
			return -1;
			break;
	}
}

/**
 * @param denom the key to the Cash Register array
 * @param denomName a pointer to the string(char array) where the
 * relevant English name of a denomination is copied
 **/
void denom_to_Name(enum denomination denom, char *denomName) {
	switch(denom) {
	case FIVE_CENTS:
		strcpy(denomName, "5 Cents");
		break;

	case TEN_CENTS:
		strcpy(denomName, "10 Cents");
		break;

	case TWENTY_CENTS:
		strcpy(denomName, "20 Cents");
		break;

	case FIFTY_CENTS:
		strcpy(denomName, "50 Cents");
		break;

	case ONE_DOLLAR:
		strcpy(denomName, "1 Dollars");
		break;

	case TWO_DOLLARS:
		strcpy(denomName, "2 Dollars");
		break;

	case FIVE_DOLLARS:
		strcpy(denomName, "5 Dollars");
		break;

	case TEN_DOLLARS:
		strcpy(denomName, "10 Dollars");
		break;

	default:
		strcpy(denomName, "error");
		break;	
	}
}
