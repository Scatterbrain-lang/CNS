#include <string.h> // strlen(), strcpy(), and strcat()
#include <stdlib.h> // malloc()
#include <ctype.h> // isspace()
#include <stdbool.h> // true and false
#include "C_Headers/error.h" // err() and errorTypes
#include "C_Headers/global.h" // __MAX_SECTION_LENGTH 
#include "C_Headers/stringops.h"
#define __INIT_RETURN_STRING char *returnString = (char *)malloc(strlen(string)*sizeof(char))
#define __RETURN_RS return returnString

char *strreverse(const char *string) {
	__INIT_RETURN_STRING;
	for (int i = strlen(string); i > -1; --i) returnString[strlen(string) - 1 - i] = string[i];
	__RETURN_RS;
}

char *lower(const char *string) {
	__INIT_RETURN_STRING;
	for (int i = 0; i < strlen(string); i++) returnString[i] = tolower(string[i]);
	__RETURN_RS;
}

int frequency(const char *string, char substring) {
	int charCount;
	for (int i = 0; i < strlen(string); i++) if (string[i] == substring) charCount++;
	return charCount;
}

int findPos(const char *string, char substring) {
	for (int i = 0; i < strlen(string); i++) if (string[i] == substring) return i;
	return -1;
}	

char *strip(const char *string) {
	unsigned short whitespaceCharCountBefore = 0;	
	unsigned short whitespaceCharCountAfter = 0;
	unsigned short strip__stringLength = strlen(string);
	unsigned short newLen;
	
	for (; isspace(string[whitespaceCharCountBefore]); whitespaceCharCountBefore++)   if (whitespaceCharCountBefore == strip__stringLength)   return (char*)malloc(0); //Count whitspace characters
	for (; isspace(string[strip__stringLength - whitespaceCharCountAfter - 1]); whitespaceCharCountAfter++);
	
	newLen = strip__stringLength - (whitespaceCharCountBefore + whitespaceCharCountAfter);
	
	char *returnString = (char*)malloc((newLen+1)*sizeof(char));
	memcpy( returnString, &string[whitespaceCharCountBefore], newLen);
	returnString[newLen] = '\0';
	__RETURN_RS;
}


char *reduce(const char *string, int reduceAllWhitespace) {
	unsigned short reduce__stringLength = strlen(string);
	char *intermediaryString = (char*)malloc(reduce__stringLength*sizeof(char));
	
	
	if(reduceAllWhitespace) {
		for(int i = 0; i < reduce__stringLength; i++) {
			if(isspace(string[i])) {
				intermediaryString[i] = ' ';
			} else {
				intermediaryString[i] = string[i];
			}
		}
	} else {
		memcpy(intermediaryString, string, reduce__stringLength + 1);
	}
	
	
	unsigned short newLen = 0;
	bool prevCharIsSpace = false;
	for(int i = 0; i < reduce__stringLength; i++) {
		if(intermediaryString[i] == ' ') {
			if(!prevCharIsSpace) {
				intermediaryString[newLen] = ' ';
				newLen++;
			}
			prevCharIsSpace = true;
		} else {
			intermediaryString[newLen] = intermediaryString[i];
			newLen++;
			prevCharIsSpace = false;
		}
	}
	
	
	char *returnString = (char*)malloc((newLen+1)*sizeof(char));
	memcpy(returnString, intermediaryString, newLen);
	returnString[newLen] = '\0';
	
	__RETURN_RS;
}

char *subStr(const char *string, unsigned int startPos, unsigned int endPos) {
	char *returnString = (char*)malloc((endPos-startPos)*sizeof(char));
	memcpy(returnString, &string[startPos], endPos-startPos);
	__RETURN_RS;
}

char *concat(const char *prefix, const char *suffix) {
	char *concattedStr = (char*)malloc(strlen(prefix) + strlen(suffix));
	strcpy(concattedStr, prefix);
	strcat(concattedStr, suffix);
	return concattedStr;
}

char **tokenize(const char *string) {
	// Init
	char **tokens = (char**)malloc(__MAX_SECTION_LENGTH*sizeof(char*));
	int t = 0; // token count
	// Parantheses handling TODO: Ensure that function handling takes place before this
	for (int i = 0; i < frequency(string, '('); i++) {
		if (findPos(string, ')') == -1) err(Generic, concat(string, "\nGiven opening paranthesis does not have a matching closing paranthesis."));
		tokens[t++] = subStr(string, findPos(string, '(') + 1, strlen(string) - 1 - findPos(strreverse(string), ')')); 
		// ^ t increments because it needs to subtract from itself for array formatting but also needs to add to itself; instead, use current value and then add afterwards with t++. The line loops through all pairs of parantheses and tokenizes the value
		if (findPos(tokens[t - 1], ')') != -1) {
			char **tempTokenize = tokenize(tokens[t - 1]);
			for (int j = 0; j < sizeof(tempTokenize) / sizeof(char*); j++) tokens[t++] = tempTokenize[j];
		}
	}
	return tokens;
}
