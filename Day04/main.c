#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

int main(int argc, char** argv){
    FILE *fp;
    fp = fopen("input.txt", "r");
    const unsigned char rows = 140;
    const unsigned char columns = 140; 
    
    char s[rows+6][columns+6]; // Add 3 character buffer around grid
    char c = 0;
    size_t i = 0;
    // Read entire text file into char array
    unsigned char row = 3;
    unsigned char col = 3;
    while((c = fgetc(fp)) != EOF){
        if (c == '\n'){
            row += 1;
            col = 3;
        }else{
            s[row][col++] = c;
        }
        
    }
    int sum1 = 0;
    int sum2 = 0;
    for (unsigned char row = 3; row < rows+3; row++){
        for (unsigned char col = 3; col < columns+3; col++){
            char current_char = s[row][col];
            if (current_char == 'X'){
                char north[3] = {s[row-1][col], s[row-2][col], s[row-3][col]};
                char south[3] = {s[row+1][col], s[row+2][col], s[row+3][col]};
                char east[3] = {s[row][col+1], s[row][col+2], s[row][col+3]};
                char west[3] = {s[row][col-1], s[row][col-2], s[row][col-3]};
                char north_east[3] = {s[row-1][col+1], s[row-2][col+2], s[row-3][col+3]};
                char south_east[3] = {s[row+1][col+1], s[row+2][col+2], s[row+3][col+3]};
                char north_west[3] = {s[row-1][col-1], s[row-2][col-2], s[row-3][col-3]};
                char south_west[3] = {s[row+1][col-1], s[row+2][col-2], s[row+3][col-3]};
                if (north[0] == 'M' && north[1] == 'A' && north[2] == 'S'){
                    sum1 += 1;
                }
                if (south[0] == 'M' && south[1] == 'A' && south[2] == 'S'){
                    sum1 += 1;
                }
                if (east[0] == 'M' && east[1] == 'A' && east[2] == 'S'){
                    sum1 += 1;
                }
                if (west[0] == 'M' && west[1] == 'A' && west[2] == 'S'){
                    sum1 += 1;
                }
                if (north_east[0] == 'M' && north_east[1] == 'A' && north_east[2] == 'S'){
                    sum1 += 1;
                }
                if (south_east[0] == 'M' && south_east[1] == 'A' && south_east[2] == 'S'){
                    sum1 += 1;
                }
                if (north_west[0] == 'M' && north_west[1] == 'A' && north_west[2] == 'S'){
                    sum1 += 1;
                }
                if (south_west[0] == 'M' && south_west[1] == 'A' && south_west[2] == 'S'){
                    sum1 += 1;
                }
            } else if (current_char == 'A'){
                char diag_0[2] = {s[row-1][col-1], s[row+1][col+1]};
                char diag_1[2] = {s[row-1][col+1], s[row+1][col-1]};
                if (((diag_0[0] == 'S' && diag_0[1] == 'M') || (diag_0[0] == 'M' && diag_0[1] == 'S'))
                    && ((diag_1[0] == 'S' && diag_1[1] == 'M') || (diag_1[0] == 'M' && diag_1[1] == 'S'))){
                    sum2 += 1;
                }
            }
        }
    }
    printf("sum1: %d\n", sum1);
    printf("sum2: %d\n", sum2);
}