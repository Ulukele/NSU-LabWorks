#include <stdio.h>
#include <stdbool.h>
#include <memory.h>

#define MAX_TEMPLATE_LEN 16
#define HASH_POWER 3
#define HASH_MODULO 3
#define MAX_STRING_LEN 16

int get_unsigned_strlen(unsigned char* str) {
    int length = 0;
    while (length < MAX_STRING_LEN && str[length] != '\0') {
        ++length;
    }
    return length;
}

void get_line(unsigned char* str, int max_len) {
    int last_symbol_index = 0;

    while (true) {
        int symbol = fgetc(stdin);
        if (symbol == EOF || symbol == '\n' || last_symbol_index >= max_len) {
            break;
        }
        str[last_symbol_index] = (unsigned char)symbol;
        last_symbol_index++;
    }

    //last symbol should be '\0'
    str[last_symbol_index] = '\0';
}

int pow_number(int num, int degree) {
    int result = 1;
    for (int i = 0; i < degree; ++i) {
        result *= num;
    }
    return result;
}

//returns true if first string equal to second
//logs indexes of unsigned characters that was compared (with shift = log_shift)
bool compare_strings(unsigned char* first, unsigned char* second, int log_shift) {
    int first_len = (int)get_unsigned_strlen(first);
    int second_len = (int)get_unsigned_strlen(second);
    if (first_len != second_len) {
        return false;
    }
    for (int i = 0; i < first_len; ++i) {
        printf("%d ", i + log_shift);
        if (first[i] != second[i]) {
            return false;
        }
    }
    return true;
}

//shifts string with symbol from stdin
//returns true if shift completed successfully
//recount its hash
bool shift_string(unsigned char* str, int* hash, int hash_factor) {

    int new_hash = *hash - (int)str[0] % HASH_MODULO;
    new_hash /= HASH_POWER;

    int last_index = (int)get_unsigned_strlen(str) - 1;

    memmove(str, str + 1, last_index);

    int new_symbol = fgetc(stdin);
    if (new_symbol!= EOF) {
        str[last_index] = (unsigned char)new_symbol;
        new_hash += (new_symbol % HASH_MODULO) * hash_factor;
        *hash = new_hash;
        return true;
    }
    else {
        return false;
    }
}

//calculate hash of some string
//calculate_hash(c[0],...,c[N-1]) == sum((c[i] % 3) * (3^i))
int calculate_hash(unsigned char* str) {

    int hash_value = 0;
    int factor = 1; //means some degree of 3

    for (int i = 0; i < (int)get_unsigned_strlen(str); ++i) {
        hash_value += ((int)str[i] % HASH_MODULO) * factor;
        factor *= HASH_POWER;
    }

    return hash_value;
}

void find_substrings(unsigned char* template, unsigned char* text) {

    int template_len = (int)get_unsigned_strlen(template);
    int template_hash = calculate_hash(template);
    int hash_factor = pow_number(HASH_POWER, template_len - 1);

    printf("%d ", template_hash);

    int text_hash = calculate_hash(text);
    int text_shift = 1;

    bool can_compare = true;
    while (can_compare) {
        if (text_hash == template_hash) {
            compare_strings(text, template, text_shift);
        }
        can_compare = shift_string(text, &text_hash, hash_factor);
        ++text_shift;
    }

}

int main() {

    unsigned char template[MAX_TEMPLATE_LEN + 1];
    get_line(template, MAX_TEMPLATE_LEN);

    unsigned char text_part[MAX_TEMPLATE_LEN + 1];
    for (int i = 0; i < (int)get_unsigned_strlen(template); ++i) {
        int symbol = fgetc(stdin);
        if (symbol == EOF) {
            //text length less than template len
            printf("0");
            return 0;
        }
        text_part[i] = (unsigned char)symbol;
        text_part[i+1] = '\0';
    }

    find_substrings(template, text_part);

    return 0;
}
