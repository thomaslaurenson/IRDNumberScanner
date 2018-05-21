/*
* A simple re-implementation of the IRD number scanner in C++
* Useful for testing and development
* Compile using: g++ validate_numbers.cpp -o validate_numbers
* Permissions using: chmod u+x validate_numbers
* Run using: ./validate_numbers
*/

/* Includes required for bulk_extractor plugin */
#include <cstring>
#include <cstdlib>

/* Includes required for this program (validate_numbers.cpp) */
#include <iostream>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <string>

/* 
* Check if the extracted digits result in a valid IRD number
* Refer to IRD validation sheet to see what else is implemented (starting on page 33):
* http://www.ird.govt.nz/resources/e/c/ec5956fb-7427-4aba-97f6-74f54e0623c0/payroll_payday_filing_spec_2019_+v1.0.pdf
* Return statement summary:
* 0: failed IRD number validation check
* 1: passed IRD number validation check
*/ 
static int validate_digits(char *digits) {
    /* Specify IRD number weightings */
    static int weightings1 [8] = {3, 2, 7, 6, 5, 4, 3, 2};
    static int weightings2 [8] = {7, 4, 3, 2, 5, 2, 7, 6};

    /* Convert the extracted string from bulk_extractor to an int */
    int digits_int = atoi(digits);
 
    /* CHECK PHASE 1 */
    if ( digits_int > 10000000 && digits_int < 150000000 ) {
        /*
        * Valid IRD numbers are only between:
        * 10000000 and 150000000
        * Only continue if this is true
        */

        /* CHECK PHASE 2 */
        
        /* Set the checksum number */
        int checksum = digits[strlen(digits) - 1] - '0';

        /* Removing the trailing check digit */
        digits[strlen(digits) - 1] = '\0';
        
        /* If digits length is 7, pad with a leading zero */
        if ( strlen(digits) == 7 ) {
            for (int i = strlen(digits); i > 0; i--) {
                digits[i] = digits[i-1];
            }
            digits[0] = '0';
        }

        /* CHECK PHASE 3 */
        int total = 0;

        for (int i = 0; i < strlen(digits); i++) {
            /* Convert specific digit index to an int */
            int num = digits[i] - '0';
            /* Keep a running count of the total */
            /* This is the extracted number times the IRD weighting scheme */
            total += num * weightings1[i];
        }

        /* Divide total weightings by 11 and get remainder */
        int R1 = total % 11;

        if( R1 > 0 ) {
            /* If remainder is greater than 0, remainder = 11 - remainder */
            R1 = 11 - R1;
        }

        if ( checksum == R1 ) {
            /* Determine if the checksum is same as remainder */
            /* If so, input is an IRD number: return 1 */
            return 1;
        }

        /* CHECK PHASE 4 */
        else if ( R1 == 10 ) {
            /* If remainder equals 10 */
            /* Reset total to 0 */
            total = 0;

            int len = strlen(digits);

            for (int i = 0; i < len; i++){
                /* Convert digits index to int */
                int num = digits[i] - '0';
                /* Keep a running count of the total */
                /* This is the extracted number times the IRD weighting scheme */
                total += num * weightings2[i];
            }

            /* Divide total weightings by 11 and get remainder */
            int R2 = total % 11;

            /* If remainder is greater than 0, take away 11 */
            if( R2 > 0 ) {
                /* If remainder is greater than 0, remainder = 11 - remainder */
                R2 = 11 - R2;
            }

            /* CHECK PHASE 5 */
            if ( checksum == R2 ) {
                /* Determine if the checksum is same as remainder */
                /* If so, input is an IRD number: return 1 */
                return 1;
            }
        }
    }
    return 0;
}

/*
* Function to test a set of four simple debugging numbers
*/
static int debugging_tests() {
    std::cout << ">>> Starting debugging tests...\n";

    /* Specificy documented IR test numbers */
    char test1[] = {'4', '9', '0', '9', '1', '8', '5', '0', '\0'}; // valid
    int result1 = validate_digits(test1);

    char test2[] = {'3', '5', '9', '0', '1', '9', '8', '1', '\0'}; // valid
    int result2 = validate_digits(test2);

    char test3[] = {'4', '9', '0', '9', '8', '5', '7', '6', '\0'}; // valid
    int result3 = validate_digits(test3);

    char test4[] = {'1', '3', '6', '4', '1', '0', '1', '3', '2', '\0'}; // valid
    int result4 = validate_digits(test4);

    char test5[] = {'1', '3', '6', '4', '1', '0', '1', '3', '3', '\0'}; // invalid
    int result5 = validate_digits(test5);

    char test6[] = {'9', '1', '2', '5', '5', '6', '8' , '\0'}; // invalid
    int result6 = validate_digits(test6);
    
    if ( result1 == 1 &&
         result2 == 1 &&
         result3 == 1 &&
         result4 == 1 &&
         result5 == 0 &&
         result6 == 0 ) {
            std::cout << ">>> SUCCESS: Debugging tests all passed\n";
            return 1;
         }
    else {
        std::cout << ">>> ERROR: Debugging tests failed\n";
        return 0;
    }
}

/*
* Function to read a txt file of potential numbers
* Validate each number using the validate_digits function
*/
static int validate_numbers(std::string filename) {
    std::ifstream infile(filename);
    std::string line; 
    
    /* Process input file one line at a time */
    while (std::getline(infile, line)) {
        std::cout << "  > Processing: " << line << "\n";

        /* Convert string to char array for validate_digits input */
        char input[line.length()+1]; 
     
        /* Copy string to char array */
        strcpy(input, line.c_str()); 

        /* Run validate_digits on input line */ 
        int result = validate_digits(input);

        /* Print results to user */
        if (result == 0) {
            std::cout << "  > Result: Failed\n";
        }
        else if (result == 1) {
            std::cout << "  > Result: Passed\n";
        }
    }
}

/*
* Function to test all potential numbers
* Should return 13884380
* Current time: 23 seconds (in VM)
*/
void generate_valid_numbers() {
    int count = 0;

    for( int i = 10000000; i < 150000000; i = i + 1 ) {
        /* Test the available IRD number range */
        /* This tests all 140 million possible combinations */
        char input[10];
        sprintf(input, "%d", i);
        char saved[10];
        sprintf(saved, "%d", i);

        int result = validate_digits(input);
        if ( result == 1 ) {
            /* If valid, increase the count */
            std::cout << saved << "\n";
        }
    }
}

void help_menu() {
    std::cout << "Description: validate_numbers is a simple New Zealand Inland Revenue number\n";
    std::cout << "             validator. It is the pure C++ code implementation from the\n";
    std::cout << "             IRDNumberScanner plug-in for the bulk_extractor tool.\n";
    std::cout << "      Usage: validate_numbers [options]\n\n";
    std::cout << "    Options: validate - input a text file of potential IRD numbers to validate\n";
    std::cout << "             generate - output a list of all potentially valid URD numbers\n";
    std::cout << "   Examples: ./validate_numbers validate file.txt\n";
    std::cout << "             ./validate_numbers generate\n";  
    std::cout << "             ./validate_numbers test\n";
}

int main(int argc, char** argv) {
    
    /* Start the clock */
    clock_t begin = clock();    

    if (argc == 1) {
        help_menu();
        return 0;
    }
    else if (argc > 1) {
        if (std::string(argv[1]) == "test") {
            /* Execute the debugging test */
            int debug = debugging_tests();
            std::cout << ">>> Exiting.\n";
        }
        else if (std::string(argv[1]) == "generate") {
            /* Execute the number validity test */
            generate_valid_numbers();
            return 0;
        }
        else if (std::string(argv[1]) == "validate") {
            /* Validate a file of IRD numbers */
            /* Format of file: one IRD number per line, using LF line endings */
            if (argv[2] != NULL) {
                std::string filename = argv[2];
                std::cout << ">>> Processing file: " << filename << "\n";
                validate_numbers(filename);
            }
        }
        else {
            help_menu();
            return 0;
        }
    }
    else {
        help_menu();
        return 0;
    }

    /* Stop the clock */
    clock_t end = clock();

    /* Print the elapsed time (in seconds) */
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << ">>> Time elapsed: " << elapsed_secs << "\n";

    return 0;
}
