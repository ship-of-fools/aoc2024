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

// #define map_w (7+2)
// #define first_fall 12
// #define input_name "example.txt"
#define map_w (71+2)
#define first_fall 1024
#define input_name "input.txt"
#define binary_search

uint8_t sr = 1;
uint8_t sc = 1;
uint8_t er = map_w-2;
uint8_t ec = map_w-2;

void print_map(uint8_t map[map_w][map_w]){
    for (uint8_t r=0; r<map_w; ++r){
        for (uint8_t c=0; c<map_w; ++c){
            printf("%c ", map[r][c]);
        }
        printf("\n");
    }
}

uint8_t find_exit(uint8_t map[map_w][map_w]){
    uint16_t q[map_w*map_w][2];
    uint16_t q_len = 0;
    uint16_t shortest_map[map_w][map_w];
    memset(shortest_map, 65535, sizeof(shortest_map[0])*map_w);

    uint16_t shortest_path = 65535;

    q[0][0] = sr;
    q[0][1] = sc;
    q_len = 1;
    shortest_map[sr][sc] = 0;

    while(q_len > 0){
        uint8_t cr = q[q_len-1][0];
        uint8_t cc = q[q_len-1][1];
        --q_len;
        uint16_t c_cost = shortest_map[cr][cc] + 1;
        // printf("%u, %u\n", cr, cc);
        if (cr == er & cc == ec){
            return 1;
        }
        int8_t dr = -1; // up
        int8_t dc = 0;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
        dr = +1; // down
        dc = 0;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
        dr = 0; // left
        dc = -1;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
        dr = 0; // right
        dc = 1;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
    }
    return 0; 
}

int main(int argc, char** argv){

    int fd = open(input_name, O_RDONLY);

	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;

    uint8_t map[map_w][map_w];
    memset(map, '.', sizeof(map[0])*map_w);
    uint8_t bytes_list[16384][2];
    uint16_t bytes_list_idx = 0;

    while(idx < size){
            uint8_t n = 0;
            uint8_t n2 = 0;
            while(isdigit((char)ptr[idx])){
                n = n * 10 + ((char)ptr[idx++]-'0');
            }
            ++idx;
            while(isdigit((char)ptr[idx])){
                n2 = n2 * 10 + ((char)ptr[idx++]-'0');
            }
            bytes_list[bytes_list_idx][0] = n+1;
            bytes_list[bytes_list_idx++][1] = n2+1;
            ++idx;
    }
    uint16_t num_fallen = first_fall;
    for (uint16_t i=0; i < num_fallen; ++i){
        map[bytes_list[i][1]][bytes_list[i][0]] = '#';
    }
    // Do edges
    for (uint8_t r = 0; r < map_w; ++r){
        for (uint8_t c = 0; c < map_w; ++c){
            if (r == 0 | r == map_w-1){
                map[r][c] = 'X';
            } else if (c == 0 | c == map_w-1){
                map[r][c] = 'X';
            }
        }
    }

    uint16_t q[map_w*map_w][2];
    uint16_t q_len = 0;
    uint16_t shortest_map[map_w][map_w];
    memset(shortest_map, 65535, sizeof(shortest_map[0])*map_w);

    uint16_t shortest_path = 65535;

    q[0][0] = sr;
    q[0][1] = sc;
    q_len = 1;
    shortest_map[sr][sc] = 0;

    while(q_len > 0){
        uint8_t cr = q[q_len-1][0];
        uint8_t cc = q[q_len-1][1];
        --q_len;
        uint16_t c_cost = shortest_map[cr][cc] + 1;
        if (cr == er & cc == ec){
            if (c_cost-1 < shortest_path){
                shortest_path = c_cost-1;
            }
            continue;
        }
        int8_t dr = -1; // up
        int8_t dc = 0;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
        dr = +1; // down
        dc = 0;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
        dr = 0; // left
        dc = -1;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
        dr = 0; // right
        dc = 1;
        if (map[cr+dr][cc+dc] == '.'){
            if (shortest_map[cr+dr][cc+dc] > c_cost){
                shortest_map[cr+dr][cc+dc] = c_cost;
                q[q_len][0] = cr+dr;
                q[q_len++][1] = cc+dc;
            }
        }
    } 

    uint8_t bb_r = bytes_list[num_fallen][1]-1;
    uint8_t bb_c = bytes_list[num_fallen][0]-1;

#ifdef binary_search    
    uint16_t bytes_list_idx_l = first_fall;
    uint16_t bytes_list_idx_h = bytes_list_idx-1;
    uint16_t bytes_list_idx_m = (bytes_list_idx_l + bytes_list_idx_h) / 2;

    uint8_t middle_map[map_w][map_w];
    memcpy(middle_map, map, sizeof(map[0])*map_w);
    uint8_t found_exit = 1;
    uint16_t highest_good = bytes_list_idx_l;
    uint16_t lowest_bad = bytes_list_idx_h;

    uint16_t loop = 0;
    while(1){
        for (uint16_t i = bytes_list_idx_l; i <= bytes_list_idx_m; ++i){
            middle_map[bytes_list[i][1]][bytes_list[i][0]] = '#';
        }
        found_exit = find_exit(middle_map);
        if (found_exit){
            highest_good = bytes_list_idx_m;
            memcpy(map, middle_map, sizeof(map[0])*map_w);
            bytes_list_idx_l = bytes_list_idx_m;
        } else {
            lowest_bad = bytes_list_idx_m;
            memcpy(middle_map, map, sizeof(map[0])*map_w);
            bytes_list_idx_h = bytes_list_idx_m;
        }
        bytes_list_idx_m = (bytes_list_idx_l + bytes_list_idx_h) / 2;
        if (lowest_bad == highest_good + 1){
            bb_r = bytes_list[lowest_bad][1]-1;
            bb_c = bytes_list[lowest_bad][0]-1;
            break;
        }
        if (loop >= bytes_list_idx){ // Checking for infinite loop, crazy
            printf("donegoofed\n");
        } else {
            ++loop;
        }
    }   

#else    
    uint8_t found_exit = 1;
    while(found_exit == 1){
        map[bytes_list[num_fallen][1]][bytes_list[num_fallen++][0]] = '#';
        found_exit = find_exit(map);
    }
    --num_fallen;
    bb_r = bytes_list[num_fallen][1]-1;
    bb_c = bytes_list[num_fallen][0]-1;
#endif

    printf("%u\n",shortest_path);
    printf("%u,%u\n", bb_c, bb_r);
}