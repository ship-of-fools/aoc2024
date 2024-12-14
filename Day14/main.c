#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

#define num_robots 500
#define seconds 100
#define map_w 101
#define map_h 103

#define map_w_c ((map_w - 1) / 2)
#define map_h_c ((map_h - 1) / 2)

typedef struct
{
    int8_t x;
    int8_t y; 
    int8_t vel_x;
    int8_t vel_y;
} robot;

int main(int argc, char** argv){
    // int fd = open("example.txt", O_RDONLY);
    int fd = open("input2.txt", O_RDONLY);

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    robot robots[num_robots];
    uint16_t i = 0;
    while(idx < size){

        while ((char)ptr[idx] != '\n' & (char)ptr[idx] != 0){
            idx += 2;
            robot pt = {.x=0, .y=0, .vel_x=0, .vel_y=0};
            while((char)ptr[idx] != ',')
            {
                pt.x = pt.x * 10 + ((char)ptr[idx++]-'0');
            }
            idx++;
            while((char)ptr[idx] != ' ')
            {
                pt.y = pt.y * 10 + ((char)ptr[idx++]-'0');
            }
            idx+=3;
            uint8_t minus_x = 0;
            uint8_t minus_y = 0;
            
            if ((char)ptr[idx] == '-'){
                minus_x = 1;
                idx++;
            }
            while((char)ptr[idx] != ',')
            {
                pt.vel_x = pt.vel_x * 10 + ((char)ptr[idx++]-'0');
            }
            if (minus_x){
                pt.vel_x = -pt.vel_x;
            }
            idx++;
            if ((char)ptr[idx] == '-'){
                minus_y = 1;
                idx++;
            }
            while(isdigit((char)ptr[idx]))
            {
                pt.vel_y = pt.vel_y * 10 + ((char)ptr[idx++]-'0');
            }
            if (minus_y){
                pt.vel_y = -pt.vel_y;
            }
            robots[i] = pt;
            i++;
            idx++;
        }
    }

    uint32_t quadrants[4] = {0, 0, 0, 0};

    for (uint16_t i = 0; i < num_robots; ++i){
        int64_t x_pos_end = robots[i].x + (robots[i].vel_x*seconds);
        int64_t y_pos_end = robots[i].y + (robots[i].vel_y*seconds);
        x_pos_end = ((x_pos_end % map_w) + map_w) % map_w;
        y_pos_end = ((y_pos_end % map_h) + map_h) % map_h;

        if ((x_pos_end < map_w_c) & (y_pos_end < map_h_c)){
            quadrants[0] += 1;
        } else if ((x_pos_end > map_w_c) & (y_pos_end < map_h_c)){
            quadrants[1] += 1;
        } else if ((x_pos_end < map_w_c) & (y_pos_end > map_h_c)){
            quadrants[2] += 1;
        } else if ((x_pos_end > map_w_c) & (y_pos_end > map_h_c)){
            quadrants[3] += 1;
        }
    }

    uint64_t sum1 = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
    
	
    // Part 2
    uint64_t sum2 = 0;
    for (uint16_t i = 0; i < 65535; ++i){

        uint8_t rows_count[map_h];
        memset(rows_count, 0, sizeof(uint8_t)*map_h);
        uint8_t cols_count[map_w];
        memset(cols_count, 0, sizeof(uint8_t)*map_w);

        // Do one move
        for (uint16_t j = 0; j < 500; j++){
            robots[j].x = (((robots[j].x + robots[j].vel_x) % map_w) + map_w) % map_w;
            robots[j].y = (((robots[j].y + robots[j].vel_y) % map_h) + map_h) % map_h;
            rows_count[robots[j].x] += 1;
            cols_count[robots[j].y] += 1;
        }

        uint8_t first_row = 0;
        uint8_t first_row_idx = 0;
        uint8_t second_row = 0;
        uint8_t second_row_idx = 0;
        for (uint8_t j = 0; j < map_h; j++){
            if (rows_count[j] > first_row){
                second_row = first_row;
                first_row = rows_count[j];
                first_row_idx = j;
            } else if (rows_count[j] > second_row){
                second_row = rows_count[j];
                second_row_idx = j;
            }
        }
        uint8_t first_col = 0;
        uint8_t first_col_idx = 0;
        uint8_t second_col = 0;
        uint8_t second_col_idx = 0;
        for (uint8_t j = 0; j < map_w; j++){
            if (cols_count[j] > first_col){
                second_col = first_col;
                first_col = cols_count[j];
                first_col_idx = j;
            } else if (cols_count[j] > second_col){
                second_col = cols_count[j];
                second_col_idx = j;
            }
        }

        if ((first_row > 30) & (second_row > 30) & (first_col > 30) & (second_col > 30)){
            sum2 = i+1;
            break;
            // uint8_t row[2][map_w];
            // memset(row, 0, sizeof(uint8_t)*map_w*2);
            // for (uint16_t j = 0; j < 500; ++j){
            //     if (robots[j].x == first_idx){
            //         row[0][robots[j].y] = 1;
            //     }
            //     if (robots[j].x == second_idx){
            //         row[1][robots[j].y] = 1;
            //     }
            // }
            // uint8_t longest_line_length = 0;
            // uint8_t longest_line_idx = 0;
            // uint8_t on_line = 0;
            // uint8_t current_line_length = 0;
            // uint8_t current_line_idx = 0;
            // for (uint8_t j = 0; j < map_w; ++j){
            //     if (!on_line){
            //         if (row[0][j] == 0)continue;
            //         if (row[0][j] == 1){
            //             on_line = 1;
            //             current_line_idx = j;
            //             current_line_length = 1;
            //             continue;
            //         }
            //     }
            //     if (on_line){
            //         if (row[0][j] == 1){
            //             current_line_length++;
            //             continue;
            //         }
            //         if (row[0][j] == 0){
            //             on_line = 0;
            //             if (current_line_length > longest_line_length){
            //                 longest_line_length = current_line_length;
            //                 longest_line_idx = current_line_idx;
            //             }
            //         }
            //     }
            // }
            // uint8_t row_0_longest_line_length = longest_line_length;
            // uint8_t row_0_longest_line_idx = longest_line_idx;

            // longest_line_length = 0;
            // longest_line_idx = 0;
            // on_line = 0;
            // current_line_length = 0;
            // current_line_idx = 0;
            // for (uint8_t j = 0; j < map_w; ++j){
            //     if (!on_line){
            //         if (row[1][j] == 0)continue;
            //         if (row[1][j] == 1){
            //             on_line = 1;
            //             current_line_idx = j;
            //             current_line_length = 1;
            //             continue;
            //         }
            //     }
            //     if (on_line){
            //         if (row[1][j] == 1){
            //             current_line_length++;
            //             continue;
            //         }
            //         if (row[1][j] == 0){
            //             on_line = 0;
            //             if (current_line_length > longest_line_length){
            //                 longest_line_length = current_line_length;
            //                 longest_line_idx = current_line_idx;
            //             }
            //         }
            //     }
            // }
            // uint8_t row_1_longest_line_length = longest_line_length;
            // uint8_t row_1_longest_line_idx = longest_line_idx;

            // if (row_0_longest_line_length > 20){
            //     if ((row_0_longest_line_idx == row_1_longest_line_idx) &
            //         (row_0_longest_line_length == row_1_longest_line_length)){
            //             sum2 = i+1;
            //             break;
            //         }
            // }
        }
    }
    printf("sum1: %lu\nsum2: %lu\n", sum1, sum2);
}