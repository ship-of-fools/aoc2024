#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define ismarker(c) ((c) == '.' || (c) == '#' || (c) == '^')

size_t start_row_idx = 0;
size_t start_col_idx = 0;

char hashmap[65535];

typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} DIRECTION;

void print_map(char map[256][256]){
    size_t row_idx = 0;
    size_t col_idx = 0;
    size_t width = 130;
    size_t height = 130;
    for (row_idx = 1; row_idx < height; row_idx++){
        for (col_idx = 1; col_idx < width; col_idx++){
            printf("%c", map[row_idx][col_idx]);
        }
        printf("\n");
    }
    printf("\n");
}

inline char is_loop(char map[256][256]){
    memset(hashmap, 0, sizeof(char)*65535);
    DIRECTION direction = NORTH;
    char map_marker = '^';
    unsigned char row_idx = start_row_idx;
    unsigned char col_idx = start_col_idx;
    while(map_marker != 0){
        if (direction == NORTH){
            map_marker = map[--row_idx][col_idx];
            if (map_marker == '#'){
                direction = EAST;
                ++row_idx;
                size_t h = ((row_idx) << 8) | col_idx;
                if (hashmap[h] > 1){
                    return 1;
                } else {
                    hashmap[h] += 1;
                }
            }
        } else if (direction == EAST){
            map_marker = map[row_idx][++col_idx];
            if (map_marker == '#'){
                direction = SOUTH;
                --col_idx;

                size_t h = ((row_idx) << 8) | col_idx;
                if (hashmap[h] > 1){
                    return 1;
                } else {
                    hashmap[h] += 1;
                }
            }
        } else if (direction == SOUTH){
            map_marker = map[++row_idx][col_idx];
            if (map_marker == '#'){
                direction = WEST;
                --row_idx;
                
                size_t h = ((row_idx) << 8) | col_idx;
                if (hashmap[h] > 1){
                    return 1;
                } else {
                    hashmap[h] += 1;
                }
            }
        } else if (direction == WEST){
            map_marker = map[row_idx][--col_idx];
            if (map_marker == '#'){
                direction = NORTH;
                ++col_idx;
                
                size_t h = ((row_idx) << 8) | col_idx;
                if (hashmap[h] > 1){
                    return 1;
                } else {
                    hashmap[h] += 1;
                }
            }
        }
    }
    return 0;
}

int main(int argc, char** argv){

    int fd = open("input.txt", O_RDONLY);
	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;
	char map[256][256];
    memset(map, 0, sizeof(char)*256*256);
    size_t map_row_idx = 1;
    size_t map_col_idx = 1;

	while(idx < size){
        while(ismarker((unsigned int)ptr[idx])){
            map[map_row_idx][map_col_idx++] = (unsigned int)ptr[idx++];
            if ((unsigned int)ptr[idx] == '^'){
                start_row_idx = map_row_idx;
                start_col_idx = map_col_idx;
            }
        }
        while((unsigned int)ptr[idx] == '\n'){
            idx++;
        }
        map_row_idx++;
        map_col_idx = 1;
    }

    DIRECTION direction = NORTH;
    char map_marker = '^';
    size_t row_idx = start_row_idx;
    size_t col_idx = start_col_idx;

    int sum1 = 0;
    while(map_marker != 0){
        map[row_idx][col_idx] = 'X';
        if (direction == NORTH){
            map_marker = map[--row_idx][col_idx];
            if (map_marker == '#'){
                direction = EAST;
                ++row_idx;
            }
        } else if (direction == EAST){
            map_marker = map[row_idx][++col_idx];
            if (map_marker == '#'){
                direction = SOUTH;
                --col_idx;
            }
        } else if (direction == SOUTH){
            map_marker = map[++row_idx][col_idx];
            if (map_marker == '#'){
                direction = WEST;
                --row_idx;
            }
        } else if (direction == WEST){
            map_marker = map[row_idx][--col_idx];
            if (map_marker == '#'){
                direction = NORTH;
                ++col_idx;
            }
        }
    }

    row_idx = 0;
    col_idx = 0;
    size_t width = 131;
    size_t height = 131;
    for (row_idx = 1; row_idx < height; row_idx++){
        for (col_idx = 1; col_idx < width; col_idx++){
            if (map[row_idx][col_idx] == 'X'){
                sum1 += 1;
            }
        }
    }
    
    int sum2 = 0;
    for (row_idx = 1; row_idx < height; row_idx++){
        for (col_idx = 1; col_idx < width; col_idx++){
            if (map[row_idx][col_idx] == '#' || map[row_idx][col_idx] == '^'){
                continue;
            }
            map[row_idx][col_idx] = '#';
            char loop = is_loop(&map[0]);
            if (loop){
                sum2 += 1;
            }
            map[row_idx][col_idx] = '.';
        }
    }
    printf("sum1: %d\nsum2: %d\n", sum1, sum2);

}