%{
#include "../config.h"
#include "be13_api/bulk_extractor_i.h"
#include "../src/histogram.h"
#include "../src/sbuf_flex_scanner.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "be13_api/utils.h"
#include <iostream>
#include <string>
#include <algorithm>

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

#define YY_EXTRA_TYPE ird_scanner *             /* holds our class pointer */
 
inline class ird_scanner *get_extra(yyscan_t yyscanner) {
  /* placing decl here avoids redundent declaration warning  */ 
  YY_EXTRA_TYPE yyird_get_extra (yyscan_t yyscanner ); 
  return yyird_get_extra(yyscanner);
}

/** extract the digits from a buffer of a given length
* into a null-termianted array (which must be at least len+1).
* Return 0 if extract is successful and if the count of non-digit
* numbers is either 0, then start counting for IRD numbers
**/
static int extract_digits_and_test(const char *buf,int len,char *digits){
	int nondigit_count = 0;
	while(*buf && len){
		if(isdigit(*buf)){ 
	
			*digits++ = *buf;
		} else nondigit_count++;
		buf++;
		len--;
	}
	
	*digits = 0;			// null-terminate
	if(nondigit_count==2 || nondigit_count==0) return 0;

	return -1;
}

/* 
* Check if range is valid and embeds on other checks afterwards such as
* weighted values. Refer to IRD validation sheet to see what else is embedded
* inside.
*/ 
static int validate(const char *digits){

	int IRD;
	int len = strlen(digits);

	static int Weights1 [8] = {3, 2, 7, 6, 5, 4, 3, 2};
	static int Weights2 [8] = {7, 4, 3, 2, 5, 2, 7, 6};
	std::string extractedIRD;

	/* extracts digits from hdd and places into string named extractedIRD */
	for(size_t i = 0; i <= len-1; i++){
		extractedIRD += digits[i];
	}
	IRD = atoi(extractedIRD.c_str()); //converts string to int 

	if(IRD>10000000 && IRD<150000000){
		std::string temp;
		std::string IR_num; 
		int CheckSum;

		/* Remove check number at end */
		IR_num = extractedIRD.substr(0, extractedIRD.size()-1); //remove last number of extractedIRD
		/* Sets CheckSum number */
		temp = extractedIRD[extractedIRD.size()-1]; //gets last digit of number in IRD
		CheckSum = atoi(temp.c_str()); // changes string to int

		
		/* Pads 7 digits after removing check number */
		if(IR_num.size() == 7){ //If IRD is only 7 digits
			IR_num = "0"+IR_num; //pads 0 to the front of IRD
		}

		
		/* 
		* Does weights set 1 and total 
		*/
		int total = 0;
		int	num = 0;
		int counter;
		for (size_t i = 0; i < IR_num.length(); i++){
			temp = IR_num[i]; //temporary varbile to be used for storing each individual IRD number
			num = atoi(temp.c_str()); //Change IRD number to int
			counter = num * Weights1[i]; //IRD number * weight 
			
			total += counter;
		}

		
		
		int Remainder = total%11; //Divide by 11 and get remainder

		/* if value is greater than 0. Take away 11 */
		if(Remainder > 0)
		{
			Remainder = 11 - Remainder; // 11 minus remainder
			
		}

		/* Checksum if CheckSum is same as remainder */
		if(CheckSum == Remainder){
			return 0;
		} else if (Remainder == 10){
			/* 
			* Does weights set 2 and total 
			*/
			//reset values to 0
			total = 0;
			num = 0;
			counter = 0;
			int R2;
			for (size_t i = 0; i < IR_num.length(); i++){
				temp = IR_num[i];
				num = atoi(temp.c_str()); //change to int
				counter = num * Weights2[i]; //IRD number * weight 
				total += counter;
			}
			
			
			R2 = total%11; //Divide by 11 and get remainder
			
			/* if value is greater than 0. Take away 11 */
			if(R2 > 0){
				R2 = 11 - R2; // 11 minus remainder
				
			}
			if(CheckSum == R2){
				
				return 0;
			} else {
				return -1;
			}
		} else {
			return -1;
		}
	} else {
		
		return -1;	
	}
}

/**
* Checks to see if its valid or not
* 0 is failed
* 1 is passed
**/
bool check(const char *buf,int len){
	if(len>11) { 
		return 0;
	}

	char digits[11];

	memset(digits, 0, sizeof(digits));

	if (extract_digits_and_test(buf, len, digits)){
			return 0;
	}
	if (validate(digits)){
		
		return 0;
	}

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
void scan_ird(const class scanner_params &sp,const recursion_control_block &rcb)
{
    assert(sp.sp_version==scanner_params::CURRENT_SP_VERSION);      
    if(sp.phase==scanner_params::PHASE_STARTUP){
	assert(sp.info->si_version==scanner_info::CURRENT_SI_VERSION);
	sp.info->name  = "IRDS";
	sp.info->author		= "Henry Gee";
	sp.info->description	= "scans for IRDs";
	sp.info->scanner_version= "1.0";
    sp.info->feature_names.insert("ird");

	sp.info->histogram_defs.insert(histogram_def("ird","","histogram"));


	/*scan_ird_valid_debugg = sp.info->config->debug;*/           // get debug value
	return;
    }
    if(sp.phase==scanner_params::PHASE_SCAN){
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
    if(sp.phase==scanner_params::PHASE_NONE){                 // avoids defined but not used
	(void)yyunput;			
        (void)yy_fatal_error;
    }
}
