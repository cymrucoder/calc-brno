#define STATE_NONE 0
#define STATE_START 1
#define STATE_INTEGER 2
#define STATE_DECIMAL 3
#define STATE_FLOATING_POINT 4
#define STATE_END 5

#define MAX_TYPES 4 //Not including type none

#define TYPE_NONE 0
#define TYPE_INTEGER 1
#define TYPE_FLOATING_POINT 2
#define TYPE_OPERATOR 3
#define TYPE_BRACKET 4

#define EQUALS 5
#define END 8//To account for higher numbers in PREC_X, see about twenty lines down
#define ENONTERM 9
#define ITERM 10

#define MAX_OPERATORS 7

#define OP_NONE -1
#define OP_PLUS 0
#define OP_MINUS 1
#define OP_MULTIPLY 2
#define OP_DIVIDE 3
#define OP_MODULUS 4
#define OP_POWER 5

#define MAX_BRACKETS 3

#define BRACKET_NONE -1
#define BRACKET_LEFT 6
#define BRACKET_RIGHT 7

#define NUMBER_PREC_SIGNS 10
#define PREC_SIGN_NONE 0
#define PREC_SIGN_LEFT 1
#define PREC_SIGN_RIGHT 2
#define PREC_SIGN_EQUALS 3
#define PREC_SIGN_ERROR 4

#define PREC_PLUS 0
#define PREC_MINUS 1
#define PREC_MULTIPLY 2
#define PREC_DIVIDE 3
#define PREC_MODULUS 4
#define PREC_POWER 5
#define PREC_LEFT_BRACKET 6
#define PREC_RIGHT_BRACKET 7
#define PREC_I 8
#define PREC_$ 9

#define TABLE_SIZE 10

#define RULE_NUMBER 8

#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

//int reachedEOF = 0;

/*int intTable[10];
int opTable[10];
int bracketTable[10];

int numberInts = 0;
int numberOps = 0;
int numberBrackets = 0;*/

/*int tokenTable[MAX_TYPES][TABLE_SIZE];
float floatTable[TABLE_SIZE];*/
int precTable[NUMBER_PREC_SIGNS][NUMBER_PREC_SIGNS] = {
//..Plus.......Miuns......Multiply.....Divide.....Modulus......Power.....L bracket...R bracket........i...........$....//
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT}, //Plus
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT}, //Minus
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT}, //Multiply
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT}, //Divide
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT}, //Modulus
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT,  PREC_SIGN_LEFT,  PREC_SIGN_RIGHT}, //Power
{PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_EQUALS, PREC_SIGN_LEFT,  PREC_SIGN_ERROR}, //Left bracket
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_ERROR, PREC_SIGN_RIGHT,  PREC_SIGN_ERROR, PREC_SIGN_RIGHT}, //Right bracket
{PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_RIGHT, PREC_SIGN_ERROR, PREC_SIGN_RIGHT,  PREC_SIGN_ERROR, PREC_SIGN_RIGHT}, //i
{PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_LEFT,  PREC_SIGN_ERROR,  PREC_SIGN_LEFT,  PREC_SIGN_ERROR} //$
};

int ruleTable[RULE_NUMBER][4] = {//Four is one each for A=AxA, where A is an E or i and x is an operator, or A are brackets and x is E, or E=i(ii where these two 'i's are ignored)

{ENONTERM, ENONTERM, OP_PLUS , ENONTERM},
{ENONTERM, ENONTERM, OP_MINUS , ENONTERM},
{ENONTERM, ENONTERM, OP_MULTIPLY , ENONTERM},
{ENONTERM, ENONTERM, OP_DIVIDE , ENONTERM},
{ENONTERM, ENONTERM, OP_MODULUS , ENONTERM},
{ENONTERM, ENONTERM, OP_POWER , ENONTERM},
{ENONTERM, BRACKET_LEFT, ENONTERM, BRACKET_RIGHT},
{ENONTERM, ITERM, ITERM, ITERM}
};

float tokenTable[MAX_TYPES][TABLE_SIZE];
//char *tokenTable[MAX_TYPES][TABLE_SIZE];
int tokenCount[MAX_TYPES] = {0, 0, 0, 0};
char operatorTable[MAX_OPERATORS] = {'!', '+', '-', '*', '/', '%', '^'};
char bracketTable[MAX_BRACKETS] = {'!', '(', ')'};

struct token {

    int type;
    int attribute;

};

struct tokenNode {

    struct token tokenHolder;
    struct tokenNode *nodeNext;
    int precArrow;
};

struct token getNextToken();

int equalsPressed = 0;

char *stringInput;
//char stringInput[128];
int stringInputPosition = 0;

int main(int argc, char *argv[]) {



    if (argc != 2) {

        printf("Usage: calculator <string to calculate>\n");
    }
    else {

        stringInput = argv[1];
    }

    struct token tokenTemp;
    struct tokenNode *tokenFirst = (struct tokenNode*) malloc(sizeof(struct tokenNode));
    struct tokenNode *tokenActive = tokenFirst;
    //struct token tokenArray[100];

    //tokenHolder.type == TYPE_NONE;

    while (tokenTemp.type != EQUALS) {

        tokenTemp = getNextToken();
        if (tokenTemp.type != EQUALS) {

            tokenActive->tokenHolder = tokenTemp;
            tokenActive->nodeNext = (struct tokenNode*) malloc(sizeof(struct tokenNode));
            tokenActive = tokenActive->nodeNext;
        }
        else {

            tokenActive->tokenHolder = tokenTemp;
            break;
        }
    }
    /*printf("Equals pressed - outputting string ");
    tokenActive = tokenFirst;
    while (tokenActive != NULL) {

        switch (tokenActive->tokenHolder.type) {

            case TYPE_INTEGER:
                printf("%i", (int)tokenTable[tokenActive->tokenHolder.type][tokenActive->tokenHolder.attribute]);
                tokenActive = tokenActive->nodeNext;
                break;
            case TYPE_FLOATING_POINT:
                printf("%f", tokenTable[tokenActive->tokenHolder.type][tokenActive->tokenHolder.attribute]);
                tokenActive = tokenActive->nodeNext;
                break;
            case TYPE_OPERATOR:
                printf("%c", operatorTable[(int)tokenTable[tokenActive->tokenHolder.type][tokenActive->tokenHolder.attribute]]);
                tokenActive = tokenActive->nodeNext;
                break;
            case TYPE_BRACKET:
                printf("%c", bracketTable[(int)tokenTable[tokenActive->tokenHolder.type][tokenActive->tokenHolder.attribute]]);
                tokenActive = tokenActive->nodeNext;
                break;
            case EQUALS:
                printf("\n");
                tokenActive = NULL;
                break;
            default:
                printf("Error has occured while printing string - Closing program");
                return 0;
                break;
        }
    }*/

    //printf("Beginning O-P P\n");

    struct tokenNode *pushdownFirst = (struct tokenNode *) malloc(sizeof(struct tokenNode));

    struct token tempToken;
    tempToken.type = END;

    pushdownFirst->nodeNext = NULL;
    pushdownFirst->tokenHolder = tempToken;

    struct tokenNode *pushdownCurrent;
    struct tokenNode *inputCurrent;
    struct tokenNode *auxToken;
    //struct tokenNode *tempToken;

    int exitParse = 0;

    /*while (exitParse == 0) {

        pushdownCurrent = pushdownFirst;
        while ((pushdownCurrent->tokenHolder).type != ENONTERM) {

            pushdownCurrent = pushdownCurrent->nodeNext;
        }
        inputCurrent = tokenFirst;

        //------------------------------------------------------------------------------------------------------------------
        //Converts token type into a number that can be used by the precedence table

        int precNumLeft, precNumRight;

        if (pushdownCurrent->tokenHolder.type == TYPE_INTEGER || pushdownCurrent->tokenHolder.type == TYPE_FLOATING_POINT) {

            precNumLeft = PREC_I;
        }

        else {

            precNumLeft = tokenTable[pushdownCurrent->tokenHolder.type][pushdownCurrent->tokenHolder.attribute];
        }

        if (inputCurrent->tokenHolder.type == TYPE_INTEGER || inputCurrent->tokenHolder.type == TYPE_FLOATING_POINT) {

            precNumRight = PREC_I;
        }

        else {

            precNumRight = tokenTable[inputCurrent->tokenHolder.type][inputCurrent->tokenHolder.attribute];
        }

        //------------------------------------------------------------------------------------------------------------------
        //Uses numbers generated above to work out if input is acceptable

        switch (precTable[precNumLeft][precNumRight]) {

            case PREC_SIGN_EQUALS:

                auxToken = inputCurrent->nodeNext;

                inputCurrent->nodeNext = pushdownCurrent;
                pushdownFirst = inputCurrent;

                tokenFirst = auxToken;
                break;

            case PREC_SIGN_LEFT:

                pushdownFirst->precArrow = PREC_SIGN_LEFT;

                auxToken = inputCurrent->nodeNext;

                inputCurrent->nodeNext = pushdownCurrent;
                pushdownFirst = inputCurrent;

                tokenFirst = auxToken;
                break;

            case PREC_SIGN_RIGHT:

                if ((precNumLeft = PREC_I) && (pushdownCurrent->precArrow == PREC_SIGN_LEFT)) {//Special case where pushdown top is "<i", turns into "E"

                    pushdownCurrent->tokenHolder.type = ENONTERM;
                    pushdownCurrent->precArrow = PREC_SIGN_NONE;
                    //return r (rule number)
                    }
                else if (precNumLeft == PREC_RIGHT_BRACKET) {//E = (E)

                    if (pushdownCurrent->nodeNext->tokenHolder.type == ENONTERM) {

                        auxToken = pushdownCurrent->nodeNext->nodeNext;

                        if (tokenTable[auxToken->tokenHolder.type][auxToken->tokenHolder.attribute] == BRACKET_LEFT) {

                            auxToken->tokenHolder.type = ENONTERM;
                            auxToken->precArrow = PREC_SIGN_NONE;
                            //Need to make auxToken first token and free previous two and any structs within them)
                            auxToken->nodeNext = pushdownFirst->nodeNext->nodeNext->nodeNext;
                            pushdownFirst = auxToken;
                            //return r (rule number)
                        }
                        else {

                            printf("Error - left bracket expected\n");//Don't think this can ever run but just in case
                        }
                    }
                    else {

                        printf("Error - number expected before right bracket\n");//Don't think this can ever run
                    }
                }
                else if (pushdownCurrent->tokenHolder.type == TYPE_OPERATOR) {

                    if (pushdownCurrent->nodeNext->tokenHolder.type == ENONTERM) {

                        if (pushdownFirst->tokenHolder.type == ENONTERM) {

                            if (pushdownCurrent->nodeNext->precArrow == PREC_SIGN_LEFT) {

                                struct tokenNode *newPushdownNode = (struct tokenNode *) malloc(sizeof(struct tokenNode));
                                newPushdownNode->nodeNext = pushdownFirst->nodeNext->nodeNext->nodeNext;
                                newPushdownNode->tokenHolder.type = ENONTERM;

                            }

                        }

                    }
                }
                break;

            default:

                if (pushdownCurrent->tokenHolder.type == END) {

                    if (inputCurrent == NULL) {

                        exitParse = 1;//Ends the loop, input string is valid
                    }
                }
            break;

        }
    }*/
    return 0;
}

struct token getNextToken() {

    struct token tempToken;
    tempToken.type = TYPE_NONE;

    int state = STATE_START;
    char stringin[100] = "";//Max size 100, for now
    char input;
    //input = 'a';//Default value, if not changed error will occur

    while (state != STATE_END) {

        input = stringInput[stringInputPosition];
        if (state == STATE_START) {

            if ((input-'0') >= 0 && (input-'0' <= 9)) {//Checks if input char is a number between 0 and 9

                state = STATE_INTEGER;
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                stringInputPosition++;
            }
            else if (input == '+') {

                //stringin = stringin+input;
                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_OPERATOR;
                tempToken.attribute = tokenCount[TYPE_OPERATOR];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_OPERATOR][tokenCount[TYPE_OPERATOR]] =  OP_PLUS;
                tokenCount[TYPE_OPERATOR]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '-') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_OPERATOR;
                tempToken.attribute = tokenCount[TYPE_OPERATOR];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_OPERATOR][tokenCount[TYPE_OPERATOR]] =  OP_MINUS;
                tokenCount[TYPE_OPERATOR]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '*') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_OPERATOR;
                tempToken.attribute = tokenCount[TYPE_OPERATOR];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_OPERATOR][tokenCount[TYPE_OPERATOR]] =  OP_MULTIPLY;
                tokenCount[TYPE_OPERATOR]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '/') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_OPERATOR;
                tempToken.attribute = tokenCount[TYPE_OPERATOR];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_OPERATOR][tokenCount[TYPE_OPERATOR]] =  OP_DIVIDE;
                tokenCount[TYPE_OPERATOR]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '%') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_OPERATOR;
                tempToken.attribute = tokenCount[TYPE_OPERATOR];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_OPERATOR][tokenCount[TYPE_OPERATOR]] =  OP_MODULUS;
                tokenCount[TYPE_OPERATOR]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '^') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_OPERATOR;
                tempToken.attribute = tokenCount[TYPE_OPERATOR];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_OPERATOR][tokenCount[TYPE_OPERATOR]] =  OP_POWER;
                tokenCount[TYPE_OPERATOR]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '(') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_BRACKET;
                tempToken.attribute = tokenCount[TYPE_BRACKET];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_BRACKET][tokenCount[TYPE_BRACKET]] =  BRACKET_LEFT;
                tokenCount[TYPE_BRACKET]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == ')') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                tempToken.type = TYPE_BRACKET;
                tempToken.attribute = tokenCount[TYPE_BRACKET];//Finds the first free space in table and assigns it to attribute
                tokenTable[TYPE_BRACKET][tokenCount[TYPE_BRACKET]] =  BRACKET_RIGHT;
                tokenCount[TYPE_BRACKET]++;
                stringInputPosition++;
                return tempToken;
            }
            else if (input == '\0') {

                tempToken.type = EQUALS;
                stringInputPosition++;
                return tempToken;
            }
            else {

                return tempToken;//ERROR SOMEHOW, TOKEN NOT IN LANGUAGE
            }
        }
        else if (state == STATE_INTEGER) {

            if ((input-'0') >= 0 && (input-'0') <= 9) {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                stringInputPosition++;
            }
            else if (input == '.') {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                state = STATE_DECIMAL;
                stringInputPosition++;
            }
            else if (input == '=') {

                tempToken.type = EQUALS;
                stringInputPosition++;
                return tempToken;
            }
            else {//Not a number or decimal point, so return token as integer

                tempToken.type = TYPE_INTEGER;
                tempToken.attribute = tokenCount[TYPE_INTEGER];
                tokenTable[TYPE_INTEGER][tokenCount[TYPE_INTEGER]] = atof(stringin);//Places integer value of string in first tree table position
                //tokenTable[TYPE_INTGER][tokenCount[TYPE_INTEGER]] = stringin;
                tokenCount[TYPE_INTEGER]++;
                //stringInputPosition++;
                printf("Token is %f\n", atof(stringin));
                return tempToken;
            }
        }
        else if (state == STATE_DECIMAL) {

            if ((input-'0') >= 0 && (input-'0') <= 9) {

                state = STATE_FLOATING_POINT;
                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                stringInputPosition++;
            }
            else {

                printf("ERROR - Only numbers can go after decimal point");
                return tempToken;//ERROR ONLY NUMBERS CAN GO AFTER DECIMAL POINT
            }
        }
        else if (state == STATE_FLOATING_POINT) {

            if ((input-'0') >= 0 && (input-'0') <= 9) {

                //strcat(stringin, input);
                char stringTemp[2] = {input};
                strcat(stringin, stringTemp);
                stringInputPosition++;
            }
            else {

                tempToken.type = TYPE_FLOATING_POINT;
                tempToken.attribute = tokenCount[TYPE_FLOATING_POINT];
                tokenTable[TYPE_FLOATING_POINT][tokenCount[TYPE_FLOATING_POINT]] = atof(stringin);
                //tokenTable[TYPE_FLOATING_POINT][tokenCount[TYPE_FLOATING_POINT]] = stringin;
                tokenCount[TYPE_FLOATING_POINT]++;
                //stringInputPosition++;
                return tempToken;
            }
        }
        /*else if (input == '=') {

            printf("Incoming equals:");

            if (state == STATE_DECIMAL) {

                printf("But something went wrong");
                return tempToken;//ERROR DECIMAL POINT MUST BE FOLLOWED BY NUMBER
            }
            else {

                printf("Equals");
                tempToken.type = EQUALS;
                return tempToken;
            }
        }*/
        else {

            return tempToken;//ERROR UNEXPECTED STATE
        }
    }
    return tempToken;//ERROR UNEXPECTED STATE
}
