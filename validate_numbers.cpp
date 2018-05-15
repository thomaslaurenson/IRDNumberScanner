#include <string.h>
using namespace std;

/* 
* Check is a number is a valid NZ IRD number
* Compile using: g++ validate_numbers.cpp -o validate_numbers
* Run using: ./validate_numbers
* Permissions using: chmod u+x validate_numbers
*/ 
static int validate(const char *digits){
    /* IRD number as a string */
    std::string IRD_string;
    /* IRD number as an int */
	int IRD_number;

    /* Length of the IRD number */
	int len = strlen(digits);

    /* Specify IRD number weightings */
	static int weightings1 [8] = {3, 2, 7, 6, 5, 4, 3, 2};
	static int weightings2 [8] = {7, 4, 3, 2, 5, 2, 7, 6};
	

	/* Populate IRD_string with bulk_extractor output */
	for(size_t i = 0; i <= len-1; i++){
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
		int checksum;
        std::string temp;
		/* Remove check number at end */
		IRD_string = IRD_string.substr(0, IRD_string.size() - 1); 
        
		/* Sets checksum number */
        /* Fetch last digit, and convert to int */
        temp = IRD_string[IRD_string.size()-1];

        checksum = atoi(temp.c_str());

		if ( IRD_string.size() == 7) {
            /* Check length of the IRD number */
            /* Pads 7 digits if the IRD number is only 6 */
			IRD_string = "0" + IRD_string;
		}

		/* Perform first round of weightings */
		int total = 0;
		int	num = 0;
		int counter;
        
		for (size_t i = 0; i < IRD_string.length(); i++){
            /* Convert specific index to an int */
            temp = IRD_string[i];
			num = atoi(temp.c_str());
            /* Keep a running count of the total */
            /* This is the extracted number * the IRD weighting scheme */
			total += num * weightings1[i];
		}

		/* Divide by 11 and get remainder */
        int remainder = total % 11;

		/* If remainder is greater than 0, take away 11 */
		if( remainder > 0 ) {
			remainder = 11 - remainder;
		}

		/* Check if the checksum is same as remainder */
		if ( checksum == remainder ) {
			return 0;
		} else if ( remainder == 10 ) {
			/* 
			* Does weights set 2 and total 
			*/
			//reset values to 0
			total = 0;
			num = 0;
			counter = 0;
			int R2;
			for (size_t i = 0; i < IRD_string.length(); i++){
				temp = IRD_string[i];
                /* Convert string to int */
				num = atoi(temp.c_str());
                /* Set counter to be IRD * Weight */
				counter = num * weightings2[i];
                /* Increase total based on counter */                
				total += counter;
			}
			
			R2 = total%11; //Divide by 11 and get remainder
			
			/* if value is greater than 0. Take away 11 */
			if(R2 > 0){
				R2 = 11 - R2; // 11 minus remainder
			}
			if ( checksum == R2 ) {
				return 0;
			} 
		} 
	} 
    return -1;	
}

int main() {
	for( int i = 0; i < 999999999; i = i + 1 ) {
		char input[10];
		sprintf(input, "%d", i);

		int result;
		result = validate(input);
		if ( result == 0 ) {
		    /* If valid, print the number */
			cout << input << "\n";
		}
	}
	return 0;
}