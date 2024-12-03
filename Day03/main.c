#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

int main(int argc, char** argv){
    FILE *fp;
    fp = fopen("input.txt", "r");
    char s[32768];
    char c = 0;
    size_t i = 0;
    // Read entire text file into char array
    while((c = fgetc(fp)) != EOF){
        s[i++] = c;
    }
    size_t num_chars = i;

    int sum1 = 0;
    int sum2 = 0;
    char enabled = 1;
    for (size_t i = 6; i < num_chars; i++){
        char c[7] = {s[i-6], s[i-5], s[i-4], s[i-3], s[i-2], s[i-1], s[i]};
        if (c[0] == 'd' && c[1] == 'o' && c[2] == '(' && c[3] == ')') enabled = 1;
        if (c[0] == 'd' && c[1] == 'o' && c[2] == 'n' && c[3] == '\'' && c[4] == 't' && c[5] == '(' && c[6] == ')') enabled = 0;
        if (c[0] == 'm' && c[1] == 'u' && c[2] == 'l' && c[3] == '('){

            char n[9] = {s[i-2], s[i-1], s[i], s[i+1], s[i+2], s[i+3], s[i+4], s[i+5], '\0'};

            unsigned int x = 0;
            unsigned int y = 0;
            char z = 0;
            char zz = 0;

            sscanf(&n, "%u%c%u%c", &x, &z, &y, &zz);

            if (z != ',' || zz != ')') continue;
            sum1 += x * y;
            if (enabled) sum2 += x * y;
        }
    }
    printf("sum1: %d\n", sum1);
    printf("sum2: %d\n", sum2);
    
}