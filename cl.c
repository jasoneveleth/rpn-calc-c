#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX (num)999999999999999999
#define WORD 18
// #define SIZE 1
// ^ usage: myint->attr & SIZE

#define BSIZE 80
#define HELP                                                                \
    "HELP GUIDE\n"                                                          \
    "This is a stack-based reverse polish notation calculator,\n"           \
    "usage: <bigint> <bigint> <operator>\n"                                 \
    "for example:\n"                                                        \
    "\n"                                                                    \
    "69 420 *\n"                                                            \
    "returns: 28980\n"                                                      \
    "\n"                                                                    \
    "binary operators: + - * / %%\n"                                        \
    "unary operators: ! ~\n"                                                \
    "commands:\n"                                                           \
    "? \t\t- bring back this help guide\n"                                  \
    "p \t\t- prints the top of the stack\n"                                 \
    "s \t\t- prints the whole stack\n"                                      \
    "c \t\t- clear the stack\n"                                             \
    "q, ^D, ^C \t- quits calc\n"

typedef uint64_t num;
typedef int64_t snum; // signed num
typedef struct {
    num size;
    num *arr;
    // num attr;
} bigint;

num read_input(char **);
num parse(char *, char ***);
num execute(char **, bigint **);
void print(bigint *);
void str2bigint(bigint *, char *);
void add(bigint *, bigint *);
void rshift(bigint *, num);
// lshift
// mul
// factorial (name '!')
// modulo
// sub
// div
// negate (name '~')
// cmp
// times ten to power of (name 'e'?)

static num inline pow10(snum pow) {
    num acc = 1;
    while (pow--) acc *= 10;
    return acc;
}

// fprintf(stderr, "\n");
// fprintf(stderr, "slen: %lu, dlen %lu\n", src->size, dest->size);
// fprintf(stderr, "del: 000000000000000001");
// fprintf(stderr, "000000000000000001");
// fprintf(stderr, "000000000000000001");
// fprintf(stderr, "000000000000000001");
// fprintf(stderr, "000000000000000001\n");
// fprintf(stderr, "dst: %18lu%18lu%18lu%18lu%18lu\n", dest->arr[4] % MAX+1, dest->arr[3] % MAX+1, dest->arr[2] % MAX+1, dest->arr[1] % MAX+1, dest->arr[0] % MAX+1);
// fprintf(stderr, "src: %18lu%18lu%18lu%18lu%18lu\n", src->arr[4] % MAX+1, src->arr[3] % MAX+1, src->arr[2] % MAX+1, src->arr[1] % MAX+1, src->arr[0] % MAX+1);

// not finished or working
void rshift(bigint *n, num pow) {
    if (n->arr[n->size-1] >= pow10(WORD)) {
        n->arr = realloc(n->arr, n->size + 1);
        n->size++;
    }
    num size = n->size;
    num *arr = n->arr;

    // just to use pow, not helpful
    if (pow == 1) return; 

    for (num i = 0; i < size; i++) {
        arr[i] *= 10;
        if (arr[i] > MAX) {
            arr[i] -= MAX + 1;
            arr[i+1] += arr[i]/(MAX + 1); // carry
        }
    }
    return;
}

void add(bigint *src, bigint *dest) {
    num maxlen = (src->size > dest->size ? src->size : dest->size) + 1;
    dest->arr = realloc(dest->arr, maxlen * sizeof(num));
    memset(&dest->arr[dest->size], 0, (maxlen-dest->size) * sizeof(num));
    dest->size = maxlen;

    num *sarr = src->arr;
    num *darr = dest->arr;

    for (num i = 0; i < src->size; ++i) {
        darr[i] += sarr[i];
        if (darr[i] > MAX) {
            darr[i] -= MAX + 1;
            darr[i+1] += 1; // carry
        }
    }

    free(src->arr);
}

void str2bigint(bigint *n, char *str) {
    num slen = strlen(str);
    n->size = (slen-1)/WORD + 1; // maybe cast (num)1
    n->arr = malloc((n->size) * sizeof(num));
    memset(n->arr, 0, (n->size) * sizeof(num));
    for (num i = 0; i < slen; i++) {
        n->arr[i/WORD] += (num)(str[slen - 1 - i] - 48) * pow10(i % WORD);
    }
}

void print(bigint *n) {
    num slen = WORD * n->size;
    char *str = malloc((slen + 1) * sizeof(char)); // +1 for NUL byte
    for (num i = 0; i < slen; i++) {
        str[i] = (n->arr[(slen-1-i)/WORD] / pow10((slen-1-i) % WORD) % 10) + 48;
    }
    
    str[sizeof(char)*slen] = '\0';
    snum leading0s = -1;
    while (str[++leading0s] == 48);
    puts(&str[leading0s]);
    free(str);
}

num read_input(char **bufptr) {
    char *buf = *bufptr;
    static num space = BSIZE;

    if (!fgets(buf, (int)space, stdin)) return 0;
    char lastchar = buf[strlen(buf)-1];
    while (lastchar != '\n') {
        space *= 2;
        buf = realloc(buf, space * sizeof(char));
        if (!fgets(&buf[(space/2)-1], (int)(space/2), stdin)) return 0;
        lastchar = buf[strlen(buf)-1];
    }
    *bufptr = buf;
    return space;
}

num parse(char *buf, char ***tokensptr) {
    char **tokens = *tokensptr;
    num i = 0;
    tokens[i] = strtok(buf, " \t\n");
    while (tokens[i]) {
        static num size = BSIZE*sizeof(char);
        if (i+1 >= size) tokens = realloc(tokens, (size*=2)*sizeof(char *));
        tokens[++i] = strtok(NULL, " \t\n");
    }
    if (tokens[0] == NULL) return 0;
    *tokensptr = tokens;
    return i;
}

num execute(char **tokens, bigint **stackptr) {
    bigint *stack = *stackptr;
    static num pos = 0;
    for (num i = 0; tokens[i]; ++i) {
        static num size = 1;
        char c = tokens[i][0];
        if (pos >= size) stack = realloc(stack, (size*=2)*sizeof(bigint));

        if ((47 < c) && (c < 58)) {
            str2bigint(&stack[pos++], tokens[i]);
        } else if (strlen(tokens[i]) > 1) {
            fputs("syntax error\n", stderr);
        } else if (c == 'p') {
            if (pos > 0) print(&stack[pos-1]);
        } else if (c == '?' || c == 'h') {
            puts(HELP);
        } else if (c == 'q') {
            exit(0);
        } else if (c == 'c') {
            for (num j = 0; j < pos; j++) free(stack[j].arr);
            pos = 0;
        } else if (c == 's') {
            for (num j = 0; j < pos; j++) print(&stack[j]);
        } else if (c == '+') {
            if (pos > 1) {
                --pos;
                add(&stack[pos], &stack[pos-1]);
            } else fputs("stack too small\n", stderr);
        } else {
            fputs("syntax error\n", stderr);
        }
    }
    *stackptr = stack;
    return pos;
}

int main() {
    char *buf = malloc(BSIZE*sizeof(char));
    char **tokens = malloc(BSIZE/3*sizeof(char *));
    bigint *stack = malloc(sizeof(bigint));
    num stacklen = 0;
    while (1) { // '&' passed because we want main's pointers reallocd
        if (read_input(&buf) <= 0) break;
        if (parse(buf, &tokens) <= 0) continue;
        stacklen = execute(tokens, &stack);
    }
    for (num j = 0; j < stacklen; j++) free(stack[j].arr);
    free(stack);
    free(buf);
    free(tokens);
    return 0;
}

