#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//maximal input number size
#define MAX_IN_NUM_SIZE 14
//maximal input number part size
#define MAX_IN_NUM_PART_SIZE 13
//maximal output number part size
#define MAX_OUT_NUM_PART_SIZE 50
//maximal fractional number part size 
#define MAX_FRACT_NUM_PART_SIZE 13


void swap(char* a, char* b) {
    *a += *b;
    *b = *a - *b;
    *a = *a - *b;
}

bool is_between(int low, int high, int value) {
    return (value >= low) && (value <= high);
}

void convert_to_lowercase(char* str) {
    for (int i = 0; i < (int)strlen(str); ++i) {
        str[i] = (char)tolower(str[i]);
    }
}

void reverse_string(char* str) {
    for (int i = 0; i < (int)strlen(str)/2; ++i) {
        int opposite_digit_index = (int)strlen(str) - (i + 1);
        swap(str + i, str + opposite_digit_index);
    }
}

//convert char numeral to int number
int numeral_to_int(char numeral) {
    if (numeral >= 'a') {
        return numeral + 10 - 'a';
    }
    return numeral - '0';
}

//convert int number to char numeral
char int_to_numeral(int num) {
    if (num >= 10) {
        return num - 10 + 'a';
    }
    return num + '0';
}

//insert digit in specified index
//next symbol will be '\0'
void append_digit(int digit, int idx, char* str) {
    str[idx] = int_to_numeral(digit);
    str[idx + 1] = '\0';
}

//check that all symbols valid ('0' - '9', 'a' - 'f', '.')
bool check_symbols(char* number) {
    for (int i = 0; i < (int)strlen(number); ++i) {
        if (!(isdigit(number[i]) || isxdigit(number[i]) || (number[i] == '.'))) {
            return false;
        }
    }
    return true;
}

//check that number have <= 1 dots
//check that number don't starts or ends with dot
bool check_dots(char* number) {

    int dots_count = 0;
    for (int i = 0; i < (int)strlen(number); ++i) {
        if (number[i] == '.') {
            dots_count++;
        }
    }

    return (dots_count <= 1 && number[strlen(number) - 1] != '.' && number[0] != '.');
}

//check that all numeral in number < base
bool check_base(char* number, int base) {
    for (int i = 0; i < (int)strlen(number); ++i) {
        if (number[i] == '.') {
            continue;
        }
        if (numeral_to_int(number[i]) >= base) {
            return false;
        }
    }
    return true;
}

//find first dot symbol in string
//return it's index
//if there is no dots in string, then will return the index that out of bounds of the given string
int get_dot_index(char* str) {
    char* ptr_to_dot = strchr(str, '.');
    int dot_index;

    //if number doesn't contain dot we can imagine that dot in the end of the number
    if (ptr_to_dot == NULL) {
        dot_index = (int)strlen(str);
    }
    else {
        dot_index = (int)(ptr_to_dot - str);
    }

    return dot_index;
}

//separate number to integer_part and fractional_part
void separate_num(char* number, char* integer_part, char* fractional_part) {

    int dot_index = get_dot_index(number);

    //copy all numerals before dot
    //from number to integer_part
    memcpy(integer_part, number, dot_index);
    integer_part[dot_index] = '\0'; //last symbol should be \0


    //checks that there are numerals after the dot
    if (dot_index < (int)strlen(number) - 1) {
        int fractional_part_size = (int)strlen(number) - dot_index - 1;

        //copy all numerals after dot
        //from number to fractional_part
        memcpy(fractional_part, number + dot_index + 1, fractional_part_size);
        fractional_part[fractional_part_size] = '\0'; //last symbol should be \0
    }
    else {
        fractional_part[0] = '\0'; //fractional part is absent
    }
}

//convert integer string number to integer number
//considering base
long int convert_to_int_part(char* s_num, int base) {
    long int num = 0;

    for (int i = 0; i < (int)strlen(s_num); ++i) {
        num *= base;
        num += numeral_to_int(s_num[i]);
    }

    return num;
}

//convert fractional string number to fractional number
//considering base
long double convert_to_fract_part(char* s_num, int base) {
    long double num = 0;

    for (int i = (int)strlen(s_num) - 1; i >= 0; --i) {
        num += numeral_to_int(s_num[i]);
        num /= base;
    }

    return num;
}

//convert integer number to string number in some base
void convert_int_to_str(long int num, int base, char* s_num) {

    if (num == 0) {
        append_digit(0, 0, s_num);
        return;
    }

    //i < MAX_OUT_NUM_PART_SIZE: we should check that
    //the index does not go out of array
    for (int i = 0; i < MAX_OUT_NUM_PART_SIZE && num > 0; ++i) {
        int digit = (int)(num % base);
        append_digit(digit, i, s_num);
        num /= base;
    }

    reverse_string(s_num);
}

//convert fractional number to string number in some base
void convert_fract_to_str(long double num, int base, char* s_num) {

    if (num == 0) {
        s_num[0] = '\0';
        return;
    }

    s_num[0] = '.'; //first symbol should be .

    for (int i = 1; i < MAX_FRACT_NUM_PART_SIZE && num > 0; ++i) {
        num *= base;
        int digit = (int)num;
        append_digit(digit, i, s_num);
        num -= digit;
    }
}

//struct that contain integer and fractional parts
//of some number
struct Number {
    long double num_fractional;
    long int num_integer;
};

//convert number in some base, written in string
//to integer and fractional parts
//and put it in number
void create_number(struct Number* number, char* int_part, char* fract_part, int base) {
    number->num_integer = convert_to_int_part(int_part, base);
    number->num_fractional = convert_to_fract_part(fract_part, base);
}

//prints number according to its base
void print_number(struct Number* number, int base) {
    char left_part[MAX_OUT_NUM_PART_SIZE];
    char right_part[MAX_OUT_NUM_PART_SIZE];

    convert_int_to_str(number->num_integer, base, left_part);
    convert_fract_to_str(number->num_fractional, base, right_part);

    printf("%s%s", left_part, right_part);
}

int main() {
    //bases
    //in the notation of the problem condition
    //base_input - b1
    //base_output - b2
    int base_input, base_output;
    if (scanf("%d%d", &base_input, &base_output) != 2) {
        printf("bad input");
        return 0;
    }
    //string with number
    char number[MAX_IN_NUM_SIZE];
    if (!scanf("%13s", number)) {
        printf("bad input");
        return 0;
    }
    //if number contains A,B,...,E,F
    //convert it to a,b,...,ef
    convert_to_lowercase(number);
    if (!check_symbols(number)) {
        printf("bad input");
        return 0;
    }
    //check that bases in [2;16]
    if (!is_between(2, 16, base_input) || !is_between(2, 16, base_output)) {
        printf("bad input");
        return 0;
    }
    //check that all digits smaller than base
    if (!check_base(number, base_input)) {
        printf("bad input");
        return 0;
    }
    //check that there is 1 or 0 dots
    //check that num don't starts or ends with dot
    if (!check_dots(number)) {
        printf("bad input");
        return 0;
    }
    //integer_part - string contains integer part of number
    //fractional_part - string contains fractional part of number
    char integer_part[MAX_IN_NUM_PART_SIZE], fractional_part[MAX_IN_NUM_PART_SIZE];
    separate_num(number, integer_part, fractional_part);
    //contains integer and fractional part
    struct Number num;
    create_number(&num, integer_part, fractional_part, base_input);
    print_number(&num, base_output);

    return 0;
}
