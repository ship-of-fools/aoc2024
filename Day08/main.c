#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define ismarker(c) ((c) == '.' || (c) == '#' || (c) == '^')

int main(int argc, char** argv){

    int fd = open("input.txt", O_RDONLY);
    char width = 50;
    // int fd = open("example.txt", O_RDONLY);
    // char width = 12;
	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;
	char map[50][50];
    memset(map, 0, sizeof(char)*50*50);
    char map_p2[50][50];
    memset(map_p2, 0, sizeof(char)*50*50);

    char locations[255][16][2];
    memset(locations, -1, sizeof(char)*255*16*2);

    char symbols_count[255];
    memset(symbols_count, 0, sizeof(char)*255);

    char seen_symbols_list[255];
    unsigned char seen_symbols_list_len = 0;

    size_t map_row_idx = 0;
    size_t map_col_idx = 0;

	while(idx < size){
        if ((unsigned char)ptr[idx] == '\n'){
            idx++;
            map_row_idx += 1;
            map_col_idx = 0;
            continue;
        }
        if ((unsigned char)ptr[idx] == '.'){
            idx++;
            map_col_idx += 1;
            continue;
        }
        unsigned char c = (unsigned char)ptr[idx];

        if (symbols_count[c] == 0){
            seen_symbols_list[seen_symbols_list_len++] = c;
        }
        locations[(unsigned char)ptr[idx]][symbols_count[c]][0] = map_row_idx;
        locations[(unsigned char)ptr[idx++]][symbols_count[c]][1] = map_col_idx;
        map_col_idx += 1;
        symbols_count[c] += 1;
    }

    for (unsigned char i = 0; i < seen_symbols_list_len; i++){
        unsigned char c = seen_symbols_list[i];
        for (unsigned char j = 0; j < symbols_count[c]; j++){
            for (unsigned char k = j+1; k < symbols_count[c]; k++){
                char first_row = locations[c][j][0];
                char first_col = locations[c][j][1];
                char second_row = locations[c][k][0];
                char second_col = locations[c][k][1];

                for (unsigned char w = 0; w < 255; w++){
                    char first_antinode_row = first_row + (w*(first_row - second_row));
                    char first_antinode_col = first_col + (w*(first_col - second_col));

                    if (first_antinode_row >= 0 & first_antinode_row < width & first_antinode_col >= 0 & first_antinode_col < width){
                        if (w == 1){
                            map[first_antinode_row][first_antinode_col] += 1;
                        }
                        map_p2[first_antinode_row][first_antinode_col] += 1;
                    } else {
                        break;
                    }
                }

                for (unsigned char w = 0; w < 255; w++){
                    char second_antinode_row = second_row + (w*(second_row - first_row));
                    char second_antinode_col = second_col + (w*(second_col - first_col));

                    if (second_antinode_row >= 0 & second_antinode_row < width & second_antinode_col >= 0 & second_antinode_col < width){
                        if (w == 1){
                            map[second_antinode_row][second_antinode_col] += 1;
                        }
                        map_p2[second_antinode_row][second_antinode_col] += 1;
                    } else {

                        break;
                    }
                }
            }
        }
    }

    int sum1 = 0;
    for (unsigned char i = 0; i < width; i++){
        for (unsigned char j = 0; j < width; j++){
            if (map[i][j] > 0){
                sum1 += 1;
            }
        }
    }
    int sum2 = 0;
    for (unsigned char i = 0; i < width; i++){
        for (unsigned char j = 0; j < width; j++){
            if (map_p2[i][j] > 0){
                sum2 += 1;
            }
        }
    }
    printf("sum1: %d\nsum2: %d\n", sum1, sum2); 


}