#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SEQUENCE_LEN 10
#define MAX_INPUT_LEN 100

void GetLine(char* str, int max_len) {
    int last_symbol_index = 0;

    while (true) {
        int symbol = fgetc(stdin);
        if (symbol == EOF || symbol == '\n' || last_symbol_index >= max_len) {
            break;
        }
        str[last_symbol_index] =  (char)symbol;
        last_symbol_index++;
    }

    //last symbol should be '\0'
    str[last_symbol_index] = '\0';
}


void Swap(char* a, char* b) {
    *a += *b;
    *b = *a - *b;
    *a = *a - *b;
}


// Reverses subsequence from left to right

void ReverseSubsequence(char* sequence, int left, int right) {

    if (left >= right) {
        return;
    }

    int middle = (left + right) / 2;

    for(int i = left; i <= middle; ++i) {
        int mirrored_index = right - (i - left);
        Swap(&sequence[i], &sequence[mirrored_index]);
    }

}


// Change sequence to next position in lexicographic order

bool MoveNext(char* sequence) {

    int seq_length = (int)strlen(sequence);

    for (int i = seq_length - 1; i > 0; --i) {
        if (sequence[i] > sequence[i-1]) {
            int minimal_idx = i;
            for (int j = i; j < seq_length; ++j) {
                if (sequence[j] < sequence[minimal_idx] && sequence[j] > sequence[i-1]) {
                    minimal_idx = j;
                }
            }
            Swap(&sequence[minimal_idx], &sequence[i-1]);
            ReverseSubsequence(sequence, i, seq_length - 1);
            return true;
        }
    }

    return false;
}


// Returns true if sequence is valid
// Returns false if sequence is not valid

bool CheckSequence(char* sequence) {

    int seq_length = (int)strlen(sequence);

    int symbol_used[MAX_SEQUENCE_LEN];
    for (int i = 0; i < MAX_SEQUENCE_LEN; ++i) {
        symbol_used[i] = 0;
    }

    for (int i = 0; i < seq_length; ++i) {
        if (sequence[i] < '0' || sequence[i] > '9') {
            return false;
        }
        int index = sequence[i] - '0';
        symbol_used[index]++;

        if (symbol_used[index] > 1) {
            return false;
        }
    }

    return true;
}


int main() {

    char sequence[MAX_INPUT_LEN + 1];
    GetLine(sequence, MAX_INPUT_LEN);

    int moves_number;
    if (scanf("%d", &moves_number) != 1) {
        printf("bad input");
        return 0;
    }

    bool is_valid = CheckSequence(sequence);
    if (!is_valid) {
        printf("bad input");
        return 0;
    }

    for (int i = 0; i < moves_number; ++i) {
        //operation status
        bool status = MoveNext(sequence);

        if (!status) {
            break;
        }

        printf("%s\n", sequence);
    }

    return 0;
}
