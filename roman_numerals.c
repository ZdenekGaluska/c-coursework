#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

// Converts a positive integer (1–3999) to its Roman numeral string.
// Note: uses a static buffer — result is overwritten on each call.
static char* intToRoman(long number) {
    static char roman[20];

    const char* symbols[] = {
        "M", "CM", "D", "CD", "C", "XC", "L", "XL", "X", "IX", "V", "IV", "I"
    };
    const int values[] = {
        1000, 900, 500, 400, 100, 90, 50, 40, 10, 9, 5, 4, 1
    };

    roman[0] = '\0';
    for (int j = 0; j < 13; j++) {
        // Subtract the largest symbol that fits, repeat until remainder is smaller
        while (number >= values[j]) {
            strcat(roman, symbols[j]);
            number -= values[j];
        }
    }
    return roman;
}

// Scans a string and replaces every standalone integer in the range 1–3999
// with its Roman numeral equivalent. A number qualifies if it is preceded by
// the start of the string, whitespace, or punctuation, and followed by the
// end of the string, whitespace, or punctuation.
// Returns a newly allocated string — caller is responsible for freeing it.
char* arabicToRoman(const char* str) {
    int len = strlen(str);
    int i = 0, j = 0;

    // Allocate enough space: worst case each digit expands to multiple Roman chars
    char* result = (char*)malloc(sizeof(char) * (len * 2 + 1));

    while (str[i] != '\0') {
        // Check for start of a number: first character, or preceded by whitespace/punctuation
        int preceded_ok = (i == 0 || isspace(str[i - 1]) || ispunct(str[i - 1]));

        if (isdigit(str[i]) && preceded_ok) {
            long number = 0;

            // Read the full integer value
            while (i < len && isdigit(str[i])) {
                number = 10 * number + (str[i] - '0');
                i++;
            }

            // Convert only values in range 1–3999 followed by end-of-string,
            // whitespace, or punctuation
            int followed_ok = (str[i] == '\0' || isspace(str[i]) || ispunct(str[i]));
            if (number > 0 && number < 4000 && followed_ok) {
                char* roman = intToRoman(number);
                while (*roman)
                    result[j++] = *roman++;
            } else {
                // Number out of range or not a standalone token: copy unchanged
                sprintf(result + j, "%ld", number);
                j += strlen(result + j);
            }
        } else {
            // Copy non-numeric characters verbatim
            result[j++] = str[i++];
        }
    }

    result[j] = '\0';
    return result;
}


// --- Tests ---

int main(void) {
    char* r;

    r = arabicToRoman("CVUT FIT was founded on July 1-st 2009");
    assert(!strcmp(r, "CVUT FIT was founded on July I-st MMIX"));
    free(r);

    r = arabicToRoman("PA1 is my favorite subject, rating 5 out of 5stars");
    assert(!strcmp(r, "PA1 is my favorite subject, rating V out of 5stars"));
    free(r);

    r = arabicToRoman("The range of int data type is -2147483648 to 2147483647 inclusive.");
    assert(!strcmp(r, "The range of int data type is -2147483648 to 2147483647 inclusive."));
    free(r);

    r = arabicToRoman("There are 11 integers in closed interval 10-20");
    assert(!strcmp(r, "There are XI integers in closed interval X-XX"));
    free(r);

    r = arabicToRoman("Chuck Norris and agent 007 are able to solve all Progtest homework problems on the 1-st try");
    assert(!strcmp(r, "Chuck Norris and agent VII are able to solve all Progtest homework problems on the I-st try"));
    free(r);

    r = arabicToRoman("0");
    assert(!strcmp(r, "0"));
    free(r);

    printf("All tests passed.\n");
    return EXIT_SUCCESS;
}
