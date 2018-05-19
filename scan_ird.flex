%{

/* bulk_extractor include statements */
#include "../config.h"
#include "be13_api/bulk_extractor_i.h"

/* C include statements */
#include <cstring>
#include <cstdlib>

/* Include the flex scanner */
#include "sbuf_flex_scanner.h"

#define FEATURE_NAME "ird"

class ird_scanner : public sbuf_scanner {
public:
	ird_scanner(const scanner_params &sp):
		sbuf_scanner(&sp.sbuf),
		ird_recorder(sp.fs.get_name("ird")),
		alert_recorder(sp.fs.get_alert_recorder()){
		}

		class feature_recorder *ird_recorder;		
		class feature_recorder *alert_recorder;
};

#define YY_EXTRA_TYPE ird_scanner * /* holds our class pointer */
 
inline class ird_scanner *get_extra(yyscan_t yyscanner) {
  /* Placing decl here avoids redundent declaration warning  */ 
  YY_EXTRA_TYPE yyird_get_extra (yyscan_t yyscanner ); 
  return yyird_get_extra(yyscanner);
}

/*
* Extract only digits from the buffer.
* Return statement summary:
* 0: failed extraction check
* 1: passed extraction check
*/
static int extract_digits(const char *buf, int len, char *digits){
	/* Specifiy variable for counting anything but a digit */
	int non_digit_count = 0;
    int digit_count = 0;
    
    /* Loop through the buffer */
	while ( *buf && len ) {
        if ( isdigit(*buf) ) { 
            /* If a digit, add to the char array and increase digit count*/
			*digits++ = *buf;
            digit_count++;
		} else 
            /* Else, increase non-digit count */
            non_digit_count++;
            buf++;
            len--;
	}
	
    /* Null-terminate the extracted string of digits */
	*digits = 0;
    
    /* Check the non-digit count */
    /* Potential IRD numbers will only have 2 delimiters */
	if ( non_digit_count == 2 || non_digit_count == 0 ) {
        /* Return true (1) if the non-digit count in 0, or 2 */
        return 0;
    }
    
    /* Return false (0) if extraction is not sucessful */
	return -1;
}

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

    if ( digits_int > 10000000 && digits_int < 150000000 ) {
        /*
        * Valid IRD numbers are only between:
        * 10000000 and 150000000
        * Only continue if this is true
        */

        /* Set the checksum number */
        int checksum = digits[strlen(digits) - 2] - '0';

        if ( strlen(digits) == 8 ) {
            /* Check length of the IRD number */
            /* Pad to 8 digits if the IRD number is only 7 */
            memmove(
                digits + 0 + 1,
                digits + 0,
                strlen(digits) - (0 + 1)
            );
            digits[0] = '0';
        }
        else if ( strlen(digits) == 9 ) {
            digits[strlen(digits) - 1] = '\0';
        }

        /* WEIGHTINGS: Round 1 */
        int total = 0;

        for (int i = 0; i < strlen(digits); i++) {
            /* Convert specific digit string index to an int */
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

        /* WEIGHTINGS: Round 2 */
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
* Validity check. This is the first function called.
* Checks length, performs digit extraction and validates IRD numbers

* Return statement summary:
* 0: failed validity check
* 1: passed validity check
*/
bool check(const char *buf, int len) {
	if ( len > 11 ) { 
		/* Return false (0) if length of buffer is greater than 11 */
        return 0;
	}

    /* Maximum length of IR number is 11 */
    /* This includes 8 digit max length, 2 delimiters, and a null */
	char digits[11];

    /* Initialize digits array with zeroes */
	memset(digits, 0, sizeof(digits));

	if (extract_digits(buf, len, digits)) {
        /* Call the extract digits function */
        /* Return false (0) if extract_digits test failed */
        return 0;
	}
	if (validate_digits(digits)) {
		/* Call the validate digits function */
        /* Return false (0) if validate test failed */
        return 0;
	}
    /* Return true (1) both tests passed */
	return 1;
}


#define SCANNER "scan_ird"

%}

%option reentrant
%option noyywrap
%option 8bit
%option batch
%option case-insensitive
%option pointer
%option noyymore
%option prefix="yyird_"

DELIM	 ([- ])
START8	 [0-9]{2}
BLOCK	 [0-9]{3}

%%
[^0-9]{START8}{DELIM}{BLOCK}{DELIM}{BLOCK} {
	/* 8 Digit Default IRD */
	/* ##-###-###	or	## ### ### */
	ird_scanner &s = *yyird_get_extra(yyscanner);
    if(check(yytext+1,yyleng-1)){
        s.ird_recorder->write_buf(SBUF,s.pos+1,yyleng-1);
    }	
    s.pos += yyleng;
}

[^0-9][1]{START8}{DELIM}{BLOCK}{DELIM}{BLOCK} {
	/* 9 Digit Default IRD */
	/* 1##-###-###	or	### ### ### */
	ird_scanner &s = *yyird_get_extra(yyscanner);
    if(check(yytext+1,yyleng-1)){
        s.ird_recorder->write_buf(SBUF,s.pos+1,yyleng-1);
    }	
    s.pos += yyleng;
}

[^0-9]{START8}{BLOCK}{BLOCK} {
	/* 8 Digit NO SPACE IRD */
	/* ########	*/
	ird_scanner &s = *yyird_get_extra(yyscanner);
    if(check(yytext+1,yyleng-1)){
        s.ird_recorder->write_buf(SBUF,s.pos+1,yyleng-1);
    }	
    s.pos += yyleng;
}

[^0-9]{BLOCK}{BLOCK}{BLOCK} {
	/* 9 Digit NO SPACE IRD */
	/* ######### */
	ird_scanner &s = *yyird_get_extra(yyscanner);
    if(check(yytext+1,yyleng-1)){
        s.ird_recorder->write_buf(SBUF,s.pos+1,yyleng-1);
    }	
    s.pos += yyleng;
}

.|\n { 
     /**
      * The no-match rule.
      * If we are beyond the end of the margin, call it quits.
      */
     ird_scanner &s = *yyird_get_extra(yyscanner);
     /* putchar(yytext[0]); */ /* Uncomment for debugging */
     s.pos++; 
}

%%

extern "C"
void scan_ird(const class scanner_params &sp,const recursion_control_block &rcb) {
    assert(sp.sp_version==scanner_params::CURRENT_SP_VERSION);
    if ( sp.phase==scanner_params::PHASE_STARTUP ) {
        assert(sp.info->si_version==scanner_info::CURRENT_SI_VERSION);
        sp.info->name           = "ird";
        sp.info->author         = "Thomas Laurenson, Henry Gee";
        sp.info->description    = "Scans for New Zealand IRD numbers (similar to a US Social Security Number)";
        sp.info->scanner_version= "1.1";
        
        /* Define the feature files this scanner created */
        sp.info->feature_names.insert(FEATURE_NAME);

        /* Define the histograms to make */
        //sp.info->histogram_defs.insert(histogram_def("url","","histogram"));


        /*scan_ird_valid_debugg = sp.info->config->debug;*/  // get debug value
        return;
    }
    if ( sp.phase==scanner_params::PHASE_SCAN ) {
        ird_scanner lexer(sp);
		yyscan_t scanner;
        yyird_lex_init(&scanner);
		yyird_set_extra(&lexer,scanner);
        try {
            yyird_lex(scanner);
        }
        catch (sbuf_scanner::sbuf_scanner_exception *e ) {
            std::cerr << "Scanner " << SCANNER << "Exception " << e->what() << " processing " << sp.sbuf.pos0 << "\n";
            delete e;
        }
                
        yyird_lex_destroy(scanner);
    }
    if ( sp.phase==scanner_params::PHASE_NONE ) {
        // avoids defined but not used
        (void)yyunput;			
        (void)yy_fatal_error;
    }
}
