/* Includes required for bulk_extractor plugin */
#include <cstring>
#include <cstdlib>

/* Includes required for this program */
#include <iostream>
#include <cstdio>

/* Only required in validate_numbers.cpp */
#include <ctime>

/*
* Check if a number is a valid NZ IRD number
* Compile using: g++ validate_numbers.cpp -o validate_numbers
* Permissions using: chmod u+x validate_numbers
* Run using: ./validate_numbers
*/
static int validate(const char *digits) {
    // cout << "  > Entering validate function...\n";
    // cout << "  > Processing number: " << digits << '\n';

    /* Specify IRD number weightings */
    static int weightings1 [8] = {3, 2, 7, 6, 5, 4, 3, 2};
    static int weightings2 [8] = {7, 4, 3, 2, 5, 2, 7, 6};

    /* Duplicate input array of digits */
    char digits_process[strlen(digits) + 1];
    strncpy(digits_process, digits, strlen(digits) + 1);

    /* Convert the extracted string from bulk_extractor to an int */
    int digits_int = atoi(digits);

    if ( digits_int > 10000000 && digits_int < 150000000 ) {
        /*
        * Valid IRD numbers are only between:
        * 10000000 and 150000000
        * Only continue if this is true
        */

        /* Set the checksum number */
        int checksum = digits_process[strlen(digits_process) - 2] - '0';
        // cout << "CHECKSUM: " << checksum << '\n';

        if ( strlen(digits_process) == 8 ) {
            /* Check length of the IRD number */
            /* Pad to 8 digits if the IRD number is only 7 */
            memmove(
                digits_process + 0 + 1,
                digits_process + 0,
                strlen(digits_process) - (0 + 1)
            );
            digits_process[0] = '0';
        }
        else if ( strlen(digits_process) == 9 ) {
            digits_process[strlen(digits_process) - 1] = '\0';
        }

        /* WEIGHTINGS: Round 1 */
        int total = 0;

        for (int i = 0; i < strlen(digits_process); i++) {
            /* Convert specific digit string index to an int */
            int num = digits_process[i] - '0';
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

        else if ( R1 == 10 ) {
            /* If remainder equals 10 */
            /* Reset total to 0 */
            total = 0;

            int len = strlen(digits_process);

            for (int i = 0; i < len; i++){
                /* Convert digits index to int */
                int num = digits_process[i] - '0';
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
            if ( checksum == R2 ) {
                /* Determine if the checksum is same as remainder */
                /* If so, input is an IRD number: return 2 */
                return 2;
            }
        }
    }
    return -1;
}

/*
* Function to test a set of four simple debugging numbers
*/
static int debugging_tests() {
    int result;

    /* Quick test for valid numbers: */
    std::cout << ">>> Starting debugging tests...\n";
    char test1[] = {'1', '0', '1', '8', '7', '4', '0', '4', '\0'};
    int result1 = validate(test1);

    char test2[] = {'1', '0', '1', '8', '7', '1', '3', '0', '\0'};
    int result2 = validate(test2);

    char test3[] = {'1', '0', '0', '9', '6', '4', '3', '3', '\0'};
    int result3 = validate(test3);

    char test4[] = {'1', '0', '0', '9', '6', '4', '3', '5', '\0'};
    int result4 = validate(test4);

    if ( result1 == 1 &&
         result2 == 1 &&
         result3 == 2 &&
         result4 == 2) {
            std::cout << ">>> SUCCESS: Debugging tests all passed\n";
         }
    else {
        std::cout << ">>> ERROR: Debugging tests failed\n";
    }

    return 1;
}

/*
* Function to test all potential numbers
* Should return 13884380
* Current time: 23 seconds (in VM)
*/
static int count_valid_numbers() {
    int count = 0;

    std::cout << ">>> Starting validation test...\n";

    for( int i = 10000000; i < 150000000; i = i + 1 ) {
        /* Test the available IRD number range */
        /* This tests all 140 million possible combinations */
        char input[10];
        sprintf(input, "%d", i);

        int result;
        result = validate(input);
        if ( result == 1 || result == 2 ) {
            /* If valid, increase the count */
            count += 1;
        }
    }
    return count;
}

int main() {
    /* Execute the debugging test */
    int debug = debugging_tests();

    /* Start the clock */
    clock_t begin = clock();

    /* Execute the number validity test */
    int result = count_valid_numbers();
    std::cout << "Total valid numbers: " << result << "\n";

    /* Stop the clock */
    clock_t end = clock();

    /* Print the elapsed time (in seconds) */
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    std::cout << "Time elapsed: " << elapsed_secs << "\n";

    return 0;
}
