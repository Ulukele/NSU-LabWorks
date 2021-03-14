#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAX_TEMPLATE_LENGTH 16
#define ALPHABET_LENGTH 256

void get_line(char* str, int max_len) {
    int symbol;
    int last_symbol_index = 0;

    while ((symbol = fgetc(stdin)) != EOF && symbol != '\n' && last_symbol_index < max_len) {
        str[last_symbol_index] = (char)symbol;
        last_symbol_index++;
    }

    //last symbol should be '\0'
    str[last_symbol_index] = '\0';
}

//convert char to its alphabet index
//some symbols have negative code, so
//we need to shift their code on come constant
int convert_char_to_alph_index(char symbol) {
    return  (unsigned char)symbol;
}


//dynamic string that represents stdin symbols
//can 'touch' any symbol from stdin (by it's index) that wasn't scanned yet
//can store 'buffer_len' elements
struct DynamicString {
    //buffer - something like a window of width 'buffer_len' through which we can look at the stdin
    //example:
    // ####|ab..ef|****
    //we can't restore # symbols
    //we know ab..ef symbols
    //we can see * symbols if update buffer (shift it to the right)
    char* buffer;

    //maximal length of buffer
    int max_buffer_len;

    //length of buffer
    int buffer_len;

    //true if buffer has reached the end of the string
    bool is_ended;

    //where buffer starts in whole string
    int buffer_position;
};


//returns pointer to dynamic string
//need to free (use free_ds) when work with it finished
struct DynamicString* create_dynamic_string(int max_buffer_len) {
    struct DynamicString* dynamic_string = malloc(sizeof(struct DynamicString));
    dynamic_string->max_buffer_len = max_buffer_len;
    dynamic_string->buffer_len = 0;
    dynamic_string->is_ended = false;
    dynamic_string->buffer = malloc(sizeof(dynamic_string->buffer[0]) * (size_t)max_buffer_len);
    dynamic_string->buffer_position = 0;

    //fill buffer
    for (int i = 0; i < max_buffer_len; ++i) {
        int symbol;
        if ((symbol = fgetc(stdin)) != EOF) {
            dynamic_string->buffer[i] = (char)symbol;
            dynamic_string->buffer_len++;
        }
        else {
            //buffer has reached the end of the string
            dynamic_string->is_ended = true;
            break;
        }
    }

    return dynamic_string;
}

void free_ds(struct DynamicString* dyna_string) {
    free(dyna_string->buffer);
    free(dyna_string);
}

//returns true if ds_index in buffer
//returns false in ds_index not in buffer
bool check_ds_element(struct DynamicString* dyna_string, int ds_index) {
    int left_bound = dyna_string->buffer_position;
    int right_bound = dyna_string->buffer_position + dyna_string->buffer_len - 1;
    return (ds_index >= left_bound && ds_index <= right_bound);
}

//returns true if shifted correct
//returns false if target bigger than string len
void update_buffer(struct DynamicString* dyna_string, int target_index) {
    //check if buffer has already reached the end of the string
    if (dyna_string->is_ended) {
        return;
    }

    int right_bound = dyna_string->buffer_position + dyna_string->buffer_len - 1;

    //shift buffer to the right
    //to achieve the target
    int shift = target_index - right_bound;

    //shift shouldn't be bigger than buffer_len
    assert(shift <= dyna_string->max_buffer_len);

    //some first elements of shifted buffer
    //equal to known elements
    for (int i = 0; i + shift < dyna_string->buffer_len; ++i) {
        dyna_string->buffer[i] = dyna_string->buffer[i + shift];
    }

    //first element that we need to get from stdin
    int first_empty = dyna_string->buffer_len - shift;
    dyna_string->buffer_len = first_empty;
    for (int i = first_empty; i < dyna_string->max_buffer_len; ++i) {
        int symbol = fgetc(stdin);
        //buffer has reached the end of the string
        if (symbol == EOF) {
            dyna_string->is_ended = true;
            break;
        }
        dyna_string->buffer[i] = (char)symbol;

        //update buffer length and position
        dyna_string->buffer_len++;
        dyna_string->buffer_position++;
    }
}

char get_ds_element(struct DynamicString* dyna_string, int ds_index) {

    //needed element already in buffer
    if (check_ds_element(dyna_string, ds_index)) {
        int buffer_index = ds_index - dyna_string->buffer_position;
        return dyna_string->buffer[buffer_index];
    }
        //needed element not in buffer
    else {
        update_buffer(dyna_string, ds_index);
        if (check_ds_element(dyna_string, ds_index)) {
            return dyna_string->buffer[ds_index - dyna_string->buffer_position];
        }
        else {
            //we need to return something anyway
            return '\0';
        }
    }
}

//return true if DynamicString elements is equal to some symbol
//return false if DynamicString elements is not equal to some symbol
//send log info about each comparison
bool compare_ds(struct DynamicString* dyna_string, int ds_index, char symbol) {

    char ds_element = get_ds_element(dyna_string, ds_index);
    //if such index too big (string ended earlier)
    //we can think that there is 'emptiness' after string
    if (ds_element == '\0') {
        return -1;
    }

    //send logs to stdout
    printf("%d ", ds_index + 1);

    return ds_element == symbol;

}

void fill_bad_symbols_array(int* array, char* template, int alphabet_len) {

    int template_len = (int)strlen(template);

    //fill everything with template_length by default
    for (int i = 0; i < alphabet_len; ++i) {
        array[i] = template_len;
    }

    //fill each symbol that is in template (instead of last) as minimal mirrored index
    //mirrored index - index of symbol if count from end
    for (int i = 0; i < template_len - 1; ++i) {
        int mirrored_index = template_len - i - 1;
        //symbol index in alphabet
        int symbol_index = convert_char_to_alph_index(template[i]);
        array[symbol_index] = mirrored_index;
    }

}

//use Boyer Moore algorithm to find all substrings
void find_substrings(struct DynamicString* str, char* template, int alphabet_len) {

    int template_len = (int)strlen(template);

    //array that contains shifts
    //shift depends on symbol
    int bad_symbols_array[ALPHABET_LENGTH];
    fill_bad_symbols_array(bad_symbols_array, template, alphabet_len);

    int str_index = template_len - 1;
    int template_index = template_len - 1;


    //we should stop searching for substring
    //if string ended
    while (check_ds_element(str, str_index)) {

        template_index = template_len - 1;
        int str_entrypoint = str_index;

        while (compare_ds(str, str_index, template[template_index])) {
            --str_index;
            --template_index;
            if (template_index == -1) {
                //str_index - index of substring
                break;
            }
        }

        //index of dynamic string entrypoint in alphabet
        int bad_symbol = convert_char_to_alph_index(get_ds_element(str, str_entrypoint));

        //move index according to Boyer Moore algorithm
        str_index = str_entrypoint + bad_symbols_array[bad_symbol];
        //if str_index not in buffer
        if (!check_ds_element(str, str_index)) {
            update_buffer(str, str_index);
        }
    }
}


int main() {

    char template[MAX_TEMPLATE_LENGTH + 1];
    get_line(template, MAX_TEMPLATE_LENGTH);
    int template_len = (int)strlen(template);

    struct DynamicString* text_string = create_dynamic_string(template_len);


    find_substrings(text_string, template, ALPHABET_LENGTH);

    free_ds(text_string);
    return 0;
}
