#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    FILE *fp;
    fp = fopen("input.txt", "r");
    size_t input_length = 1000;
    size_t report_max_width = 8;

    char s[1024];
    unsigned int x = 0;
    int y = 0;

    char c = 0;
    char ss[1024];
    int i = 0;
    size_t row = 0;
    size_t column = 0;
    char data[input_length][report_max_width+1]; //plus 1 for terminator
    while((c = fgetc(fp)) != EOF){
        if (c == ' ' || c == '\n'){
            ss[i] = '\0';
            y = atoi(ss);
            data[row][column++] = (char)(y);
            i = 0;
        } else {
            ss[i++] = c;
        }
        if (c == '\n'){
            data[row][column] = -1;
            row++;
            column = 0;
        }
    }
    // Part 1
    int sum1 = 0;

    for (size_t i = 0; i < input_length; i++){
        char safe = 1;
        char increasing = 0;
        for (size_t j = 0; j < report_max_width; j++){
            if (j == 0){
                if (data[i][1] > data[i][0]){
                    increasing = 1;
                }
            } else {
                char x = data[i][j-1];
                char y = data[i][j];
                if (y==-1)break;
                // Check still increasing/decreasing
                char this_increasing = y > x ? 1 : 0;
                if (this_increasing != increasing){
                    safe = 0;
                    break;
                }
                // Check change is >= 1 | <= 3
                char change = abs(x-y);
                if (change < 1 || change > 3){
                    safe = 0;
                    break;
                }
            }
        }
        if (safe){
            sum1++;
        }
    }
    printf("sum1: %d\n", sum1);

    // Part 2
    int sum2 = 0;

    char data_all[input_length][report_max_width+1][report_max_width+1];
    memset(data_all, -1, sizeof data_all);
    for (size_t i = 0; i < input_length; i++){
        char len = 0;
        for (size_t w = 0; w < report_max_width + 1; w++){
                if (data[i][w] == -1){
                    len = w;
                    break;
                }
            }
        for (size_t j = 0; j < report_max_width; j++){
            for (size_t k = 0; k < len + 1; k++){
                char skip = 0;       
                for (size_t w = 0; w < len; w++){
                    char x = data[i][w+skip];
                    if (k == 0){
                        data_all[i][k][w] = x;
                    } else {
                        if (w == k - 1){
                            skip = 1;
                            x = data[i][w+skip];
                            data_all[i][k][w] = x;
                        } else {
                            data_all[i][k][w] = x;
                        }
                    }
                }
            }
        }
    }

    for (size_t i = 0; i < input_length; i++){
        char safe = 0;
        for (size_t j = 0; j < report_max_width + 1; j++){
            char failed = 0;
            char increasing = (data_all[i][j][1] > data_all[i][j][0]) ? 1 : 0;
            for (size_t k = 1; k < report_max_width + 1; k++){
                char x = data_all[i][j][k];
                char y = data_all[i][j][k-1];
                if (x == -1){
                    if (k == 1){
                        failed = 1;
                    }
                    break;
                }
                char inc = (x > y) ? 1 : 0;
                char diff = abs(x - y);
                if (diff < 1 || diff > 3){
                    failed = 1;
                    break;
                } else if (inc != increasing){
                    failed = 1;
                    break;
                }
            }
            if (!failed){
                safe = 1;
            } 
        }
        if (safe){
            sum2 += 1;
        }
    }
    printf("sum2: %d\n", sum2);
}