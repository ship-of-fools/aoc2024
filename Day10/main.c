#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')
#define ismarker(c) ((c) == '.' || (c) == '#' || (c) == '^')

const uint8_t map_w = 64;
const uint8_t map_h = 64;

void progress(const char map[][map_w], uint8_t pos[2], uint8_t end_map[][map_w]){
    uint8_t current_height = map[pos[0]][pos[1]];
    if (current_height == 9){
        end_map[pos[0]][pos[1]] += 1;
        return;
    }
    if (map[pos[0]][pos[1]+1] == current_height+1){
        uint8_t new_pos[2] = {pos[0], pos[1]+1};
        progress(map, new_pos, end_map);
    }
    if (map[pos[0]][pos[1]-1] == current_height+1){
        uint8_t new_pos[2] = {pos[0], pos[1]-1};
        progress(map, new_pos, end_map);
    }
    if (map[pos[0]+1][pos[1]] == current_height+1){
        uint8_t new_pos[2] = {pos[0]+1, pos[1]};
        progress(map, new_pos, end_map);
    }
    if (map[pos[0]-1][pos[1]] == current_height+1){
        uint8_t new_pos[2] = {pos[0]-1, pos[1]};
        progress(map, new_pos, end_map);
    }
}

int main(int argc, char** argv){
    
    // int fd = open("example.txt", O_RDONLY);
    int fd = open("input.txt", O_RDONLY);
    char map[map_h][map_w];
    memset(map, -1, sizeof(char)*(map_h)*(map_w));

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    size_t map_row_idx = 1;
    size_t map_col_idx = 1;

    uint8_t trail_start_locs[256][2];
    uint8_t trail_start_num = 0;

    uint8_t trail_end_locs[256][2];
    uint8_t trail_end_num = 0; 

	while(idx < size){
        if (isdigit((char)ptr[idx])){
            if ((char)ptr[idx]-'0' == 0){
                trail_start_locs[trail_start_num][0] = map_row_idx;
                trail_start_locs[trail_start_num++][1] = map_col_idx;
            }
            if ((char)ptr[idx]-'0' == 9){
                trail_end_locs[trail_end_num][0] = map_row_idx;
                trail_end_locs[trail_end_num++][1] = map_col_idx;
            }
            map[map_row_idx][map_col_idx++] = (char)ptr[idx++]-'0';
            continue;
        }
        if ((char)ptr[idx] == '\n'){
            idx++;
            map_row_idx += 1;
            map_col_idx = 1;
            continue;
        }
        idx++;
    }

    uint16_t sum1 = 0;
    uint16_t sum2 = 0;
    for (uint8_t i = 0; i < trail_start_num; ++i){
        uint8_t end_map[map_h][map_w];
        memset(end_map, 0, sizeof(uint8_t)*(map_h)*(map_w));
        progress(map, &trail_start_locs[i][0], end_map);
        uint8_t score = 0;
        uint8_t rating = 0;
        for (uint8_t j = 0; j < trail_end_num; ++j){
            if (end_map[trail_end_locs[j][0]][trail_end_locs[j][1]] > 0){
                score++;
                rating += end_map[trail_end_locs[j][0]][trail_end_locs[j][1]];
            }
        }
        sum1 += score;
        sum2 += rating;
    }
    printf("sum1: %u\nsum2: %u\n", sum1, sum2); 

}