#include <string>
#include <string.h>
#include <iostream>
using namespace std;

/* Only required in validate_numbers.cpp */
#include <ctime>

/* 
* Check if a number is a valid NZ IRD number
* Compile using: g++ validate_numbers.cpp -o validate_numbers
* Permissions using: chmod u+x validate_numbers
* Run using: ./validate_numbers
*/ 
static int validate(const char *digits) {
    /* IRD number as a string */
    std::string IRD_string;
    
    /* IRD number as an int */
    int IRD_number;

    /* Specify IRD number weightings */
    static int weightings1 [8] = {3, 2, 7, 6, 5, 4, 3, 2};
    static int weightings2 [8] = {7, 4, 3, 2, 5, 2, 7, 6};

    /* Length of the IRD number */
    int len = strlen(digits) - 1;    
    
    /* Populate IRD_string with bulk_extractor output */
    for(size_t i = 0; i <= len; i++){
        IRD_string += digits[i];
    }

    /* Convert the extracted string from bulk_extractor to an int */
    IRD_number = atoi(IRD_string.c_str());  

    if ( IRD_number > 10000000 && IRD_number < 150000000 ) {
        /* 
        * Valid IRD numbers are only between:
        * 10000000 and 150000000
        * Only continue if this is true
        */
        std::string temp;
        int checksum;
        
        /* Remove check number at end of IRD string */
        IRD_string = IRD_string.substr(0, IRD_string.size() - 1); 

        /* Set checksum number */
        /* Fetch last digit, and convert to int */
        temp = IRD_string[IRD_string.size()-1];
        checksum = atoi(temp.c_str());
        //checksum = IRD_string[IRD_string.size()-1];

        if ( IRD_string.size() == 7) {
            /* Check length of the IRD number */
            /* Pad to 8 digits if the IRD number is only 7 */
            IRD_string = "0" + IRD_string;
        }

        /* Perform first round of weightings */
        int total = 0;
        int	num = 0;
        int counter;

        for (int i = 0; i < IRD_string.length(); i++) {
            /* Convert specific IRD string index to an int */
            temp = IRD_string[i];
            num = atoi(temp.c_str());
            /* Keep a running count of the total */
            /* This is the extracted number times the IRD weighting scheme */
            total += num * weightings1[i];
        }

        /* Divide by 11 and get remainder */
        int remainder = total % 11;

        if( remainder > 0 ) {
            /* If remainder is greater than 0, take away 11 */
            remainder = 11 - remainder;
        }

        if ( checksum == remainder ) {
            /* Check if the checksum is same as remainder */
            /* If so, input is an IRD number: return 0 */
            return 1;
        } 
        else if ( remainder == 10 ) {
            /* Does weights set 2 and total */
            //reset values to 0
            total = 0;
            num = 0;
            int R2;
            
            len = IRD_string.length();
            
            for (int i = 0; i < len; i++){
                temp = IRD_string[i];
                /* Convert string to int */
                num = atoi(temp.c_str());
                /* Increase total based on counter */                
                total += num * weightings2[i];
            }

            R2 = total%11; //Divide by 11 and get remainder

            /* if value is greater than 0. Take away 11 */
            if(R2 > 0){
                R2 = 11 - R2; // 11 minus remainder
            }
            if ( checksum == R2 ) {
                return 2;
            }
        } 
    } 
    return -1;	
}

static int debugging_tests() {
    int result;

    // Quick test for valid numbers:
    //10187404 - validity 1
    //10187130 - validity 1
    //10096433 - validity 2
    //10096435 - validity 2
    
    char test1[] = {'1', '0', '1', '8', '7', '4', '0', '4', '\0'};
    result = validate(test1);
    cout << "Test 1: " << result << "\n";
    
    char test2[] = {'1', '0', '1', '8', '7', '1', '3', '0', '\0'};    
    result = validate(test2);
    cout << "Test 2: " << result << "\n";

    char test3[] = {'1', '0', '0', '9', '6', '4', '3', '3', '\0'};
    result = validate(test3);
    cout << "Test 3: " << result << "\n";

    char test4[] = {'1', '0', '0', '9', '6', '4', '3', '5', '\0'};
    result = validate(test4);
    cout << "Test 4: " << result << "\n";    
    
    return 1;
}

static int count_valid_numbers() {
    int count = 0;
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
    clock_t begin = clock();

    int debug = debugging_tests();
    int result = count_valid_numbers();
    
    /* Should return 13884380 */
    /* Current time: 78 seconds */
    cout << "Total valid numbers: " << result << "\n";
    
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;        
    
    std::cout << "Time elapsed: " << elapsed_secs << "\n";
    return 0;
}