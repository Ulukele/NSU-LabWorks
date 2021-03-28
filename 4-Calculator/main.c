#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

#define EXPRESSION_LENGTH 1000

// Result
typedef enum {Ok, BadSyntax, ZeroDivision} TStatus;
struct Result {
    int Value;
    TStatus Status;
};
typedef struct Result TResult;

// Token
typedef enum {OpenBracket, CloseBracket, Num, Add, Sub, Mul, Div, None} TokenType;

struct Token {
    TokenType Type;
    int Value;
};
typedef struct Token TToken;

// Stack
struct StackItem {
    TToken Value;
    struct StackItem* Next;
};
typedef struct StackItem* TStack;
typedef struct StackItem* TStackItem;

// Stack methods
TStack CreateStack() {
    return NULL;
}

void PushValue(TStack* top, TToken value) {
    TStackItem new_element = malloc((int)sizeof(*new_element));
    new_element->Value = value;
    new_element->Next = *top;
    *top = new_element;
}

TToken GetValue(TStack* top) {
    if (*top == NULL) {
        return (TToken){None, 0};
    }
    return (*top)->Value;
}

TToken PopValue(TStack* top) {
    if (*top == NULL) {
        return (TToken){None, 0};
    }
    TStackItem delete = *top;
    *top = (*top)->Next;
    TToken delete_value = delete->Value;
    free(delete);
    return delete_value;
}

void FreeStack(TStack top) {
    if (top == NULL) {
        return;
    }
    FreeStack(top->Next);
    free(top);
}

void GetLine(char* str, int max_len) {
    int symbol;
    int last_symbol_index = 0;

    while ((symbol = fgetc(stdin)) != EOF && symbol != '\n' && last_symbol_index < max_len) {
        str[last_symbol_index] = (char)symbol;
        last_symbol_index++;
    }

    //last symbol should be '\0'
    str[last_symbol_index] = '\0';
}

bool IsBetween(char symbol, char left, char right) {
    return (symbol >= left && symbol <= right);
}

bool IsAction(char symbol) {
    return (symbol == '-' || symbol == '+' || symbol == '*' || symbol == '/');
}

bool IsActionToken(TokenType token) {
    return  (token == Add || token == Sub || token == Mul || token == Div);
}

bool IsExpressionSymbol(char symbol) {
    return ((symbol >= '0' && symbol <= '9') || IsAction(symbol) || symbol == '(' || symbol == ')');
}

int GetPriority(TokenType action) {
    if (action == Mul || action == Div) {
        return 2;
    }
    if (action == Add || action == Sub) {
        return 1;
    }
    return 0;
}

bool ValidExpression(const char* expression) {
    int expr_len = (int)strlen(expression);
    int open_brackets = 0;

    for (int i = 0; i < expr_len; ++i) {
        if (expression[i] == '(') {
            ++open_brackets;
        }
        if (expression[i] == ')') {
            --open_brackets;
        }
        if (open_brackets < 0) {
            return false;
        }
        if (IsAction(expression[i])) {
            if (i + 1 >= expr_len || IsAction(expression[i + 1])) {
                return false;
            }
        }
        if (IsBetween(expression[i], '0', '9')) {
            if (i + 1 < expr_len && expression[i + 1] == '(') {
                return false;
            }
        }
        if (expression[i] == ')') {
            if (i + 1 < expr_len && IsBetween(expression[i + 1], '0', '9')) {
                return false;
            }
        }
        if (expression[i] == '(') {
            if (i + 1 < expr_len && IsAction(expression[i + 1])) {
                return false;
            }
        }
    }
    return (open_brackets == 0);
}

TToken MakeToken(const char* expression, int* index_p) {
    TToken token = {None, 0};
    int expr_len = (int)strlen(expression);
    int index = *index_p;
    if (expression[index] == '(') { // Open bracket
        token.Type = OpenBracket;
    }
    if (expression[index] == ')') { // Close bracket
        token.Type = CloseBracket;
    }
    if (expression[index] == '+') { // Action addition
        token.Type = Add;
    }
    if (expression[index] == '-') { // Action subtraction
        token.Type = Sub;
    }
    if (expression[index] == '*') { // Action multiplication
        token.Type = Mul;
    }
    if (expression[index] == '/') { // Action division
        token.Type = Div;
    }
    if (IsBetween(expression[index], '0', '9')) { // Number
        token.Type = Num;
        int value = expression[index] - '0';
        while (index + 1 < expr_len && IsBetween(expression[index + 1], '0', '9')) {
            value *= 10;
            value += expression[index + 1] - '0';
            ++index;
        }
        token.Value = value;
    }
    *index_p = index;
    return token;
}

TStack CreatePrefix(const char* expression) {
    int expr_len = (int)strlen(expression);

    TStack result = CreateStack();
    TStack stack = CreateStack();

    TToken input[expr_len];
    int input_p = 0;

    for (int i = 0; i < expr_len; ++i) {
        TToken token = {None, 0};

        if (!IsExpressionSymbol(expression[i])) { // Bad situation!
            PushValue(&result, token);
            return result;
        }

        token = MakeToken(expression, &i);

        input[input_p++] = token;
    }

    for (int i = 0; i < input_p;) {
        TToken token = input[i];
        TToken last = GetValue(&stack);

        if (token.Type == OpenBracket) { // Open bracket
            PushValue(&stack, token);
            ++i;
        }
        if (token.Type == CloseBracket) { // Close bracket
            if (last.Type == OpenBracket) {
                PopValue(&stack);
                ++i;
            }
            else {
                PopValue(&stack);
                PushValue(&result, last);
            }
        }
        if (token.Type == Num) { // Number
            PushValue(&result, token);
            ++i;
        }
        if (IsActionToken(token.Type)) { // Actions
            if (last.Type == None || last.Type == OpenBracket) {
                PushValue(&stack, token);
                ++i;
                continue;
            }
            int last_priority = GetPriority(last.Type);
            int token_priority = GetPriority(token.Type);
            if (token_priority <= last_priority) {
                PopValue(&stack);
                PushValue(&result, last);
            }
            else {
                PushValue(&stack, token);
                ++i;
            }
        }
    }

    TToken last = GetValue(&stack);
    while (last.Type != None) {
        if (last.Type == OpenBracket) { // Bad case!
            PushValue(&result, (TToken){None, 0});
            break;
        }
        PopValue(&stack);
        PushValue(&result, last);
        last = GetValue(&stack);
    }

    FreeStack(stack);
    return result;
}

TResult CalculatePrefix(TStack* prefix) {

    TToken last = GetValue(prefix);
    PopValue(prefix);
    if (last.Type == Num) {
        return (TResult){last.Value, Ok};
    }
    else {
        TResult first = CalculatePrefix(prefix);
        TResult second = CalculatePrefix(prefix);
        if (first.Status != Ok || second.Status != Ok) {
            if (first.Status == BadSyntax || second.Status == BadSyntax) {
                return (TResult){0, BadSyntax};
            }
            return (TResult){0, ZeroDivision};
        }
        if (last.Type == Add) {
            return (TResult){second.Value + first.Value, Ok};
        }
        if (last.Type == Sub) {
            return (TResult){second.Value - first.Value, Ok};
        }
        if (last.Type == Mul) {
            return (TResult){second.Value * first.Value, Ok};
        }
        if (last.Type == Div) {
            if (first.Value == 0) {
                return (TResult){0, ZeroDivision};
            }
            return (TResult){second.Value / first.Value, Ok};
        }
    }
    return (TResult){0, BadSyntax};
}

int main() {

    char expression[EXPRESSION_LENGTH + 1];

    GetLine(expression, EXPRESSION_LENGTH);

    if (!ValidExpression(expression)) {
        printf("syntax error");
        return 0;
    }

    TStack prefix = CreatePrefix(expression);
    if (GetValue(&prefix).Type == None) {
        printf("syntax error");
        FreeStack(prefix);
        return 0;
    }

    TResult result = CalculatePrefix(&prefix);
    FreeStack(prefix);
    if (result.Status == ZeroDivision) {
        printf("division by zero");
        return 0;
    }
    if (result.Status == BadSyntax) {
        printf("syntax error");
        return 0;
    }

    printf("%d", result.Value);

    return 0;
}
