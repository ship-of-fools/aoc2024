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

// #define input_file "example.txt"
// #define map_w 8
// #define input_file "example_big.txt"
// #define map_w 10
#define input_file "input.txt"
#define map_w 50

uint8_t sr;
uint8_t sc;
uint8_t srp2;
uint8_t scp2;

uint8_t in_queue(uint8_t Q[32][2], uint8_t r, uint8_t c, uint8_t len){
    for (uint8_t i = 0; i<len; i++){
        if (Q[i][0] == r){
            if (Q[i][1] == c){
                return 1;
            }
        }
    }
    return 0;
}
uint8_t in_queuep2(uint8_t Q[32][3], uint8_t r, uint8_t c, uint8_t len){
    for (uint8_t i = 0; i<len; i++){
        if (Q[i][0] == r){
            if (Q[i][1] == c){
                return 1;
            }
        }
    }
    return 0;
}
void print_map(uint8_t map[map_w][map_w]){
    for (uint8_t i = 0; i < map_w; i++){
        for (uint8_t j = 0; j < map_w; j++){
            printf("%c ", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
void print_map2(uint8_t map[map_w][map_w*2]){
    for (uint8_t i = 0; i < map_w; i++){
        for (uint8_t j = 0; j < map_w*2; j++){
            // if (map[i][j] == '.'){
            //     printf("  ")
            // 
            printf("%c", map[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv){
    int fd = open(input_file, O_RDONLY);
	struct stat st;
	size_t size;
	int status = fstat(fd, &st);
	size = st.st_size;
	char *ptr = (char*) mmap(0,size,PROT_READ,MAP_PRIVATE,fd,0);
	size_t idx = 0;
    uint8_t map[map_w][map_w];
    uint8_t map_p2[map_w][map_w*2];
    uint8_t directions_list[65535];
    uint16_t directions_list_len = 0;

    while(idx < size){
        for (uint8_t r = 0; r < map_w; r++){
            for (uint8_t c = 0; c < map_w; c++){
                map[r][c] = (char)ptr[idx];
                if ((char)ptr[idx]=='@'){
                    sr = r;
                    sc = c;
                    srp2 = r;
                    scp2 = c*2;
                    map_p2[r][c*2] = '@';
                    map_p2[r][c*2+1] = '.';
                } else if ((char)ptr[idx]=='.'){
                    map_p2[r][c*2] = '.';
                    map_p2[r][c*2+1] = '.';
                } else if ((char)ptr[idx]=='#'){
                    map_p2[r][c*2] = '#';
                    map_p2[r][c*2+1] = '#';
                } else if ((char)ptr[idx]=='O'){
                    map_p2[r][c*2] = '[';
                    map_p2[r][c*2+1] = ']';
                }
                idx++;
            }
            
            if (r == map_w-1)break;
            while((char)ptr[++idx]!= '#');
        }
        while(1){
            if ((char)ptr[idx] == '<' | (char)ptr[idx] == '>' | (char)ptr[idx] == '^' | (char)ptr[idx] == 'v')break;
            idx++;
        }

        while((char)ptr[idx] != '\0'){
            if ((char)ptr[idx] == '<' | (char)ptr[idx] == '>' | (char)ptr[idx] == '^' | (char)ptr[idx] == 'v'){
                directions_list[directions_list_len++] = (char)ptr[idx];
            }
            idx++;
        }
        break;
    }

    // PART 1
    uint8_t pos_q[32][2];
    uint8_t pos_q_len = 0;

    uint8_t batch_q[32][2];
    uint8_t batch_q_len = 0;

    pos_q[pos_q_len][0] = sr;
    pos_q[pos_q_len++][1] = sc;

    uint8_t cr = sr;
    uint8_t cc = sc;
    uint8_t do_move = 0;
    for (uint16_t i = 0; i < directions_list_len; i++){
        do_move = 1;
        int8_t dr = 0;
        int8_t dc = 0;
        pos_q_len = 0;
        pos_q[pos_q_len][0] = sr;
        pos_q[pos_q_len++][1] = sc; // Append current position to Q
        batch_q_len = 0;
        batch_q[batch_q_len][0] = sr;
        batch_q[batch_q_len++][1] = sc; // Append current position to batch Q
        if (directions_list[i] == '<'){dr =  0; dc = -1;}
        if (directions_list[i] == '>'){dr =  0; dc =  1;}
        if (directions_list[i] == '^'){dr = -1; dc =  0;}
        if (directions_list[i] == 'v'){dr =  1; dc =  0;}

        while (pos_q_len > 0){
            cr = pos_q[pos_q_len-1][0];
            cc = pos_q[pos_q_len-1][1];
            pos_q_len--;
            uint8_t nr = cr + dr;
            uint8_t nc = cc + dc;
            if (map[nr][nc] == '#'){
                do_move = 0;
                break;
            };
            if (map[nr][nc] == 'O'){
                batch_q[batch_q_len][0] = nr;
                batch_q[batch_q_len++][1] = nc;
                pos_q[pos_q_len][0] = nr;
                pos_q[pos_q_len++][1] = nc;
            }
        }
        if (!do_move)continue;
        for (int8_t i=batch_q_len-1; i>=0; i--){
            if (i > 0){
                map[batch_q[i][0]+dr][batch_q[i][1]+dc] = 'O';
                map[batch_q[i][0]][batch_q[i][1]] = '.';
            } else {
                map[batch_q[i][0]+dr][batch_q[i][1]+dc] = '@';
                map[batch_q[i][0]][batch_q[i][1]] = '.';
            }
        }
        sr = sr + dr;
        sc = sc + dc;

    }

    uint64_t sum1 = 0;
    for (uint8_t i=0; i<map_w; i++){
        for (uint8_t j=0; j<map_w; j++){
            if (map[i][j] == 'O'){
                sum1 = sum1 + (100*i + j);
            }
        }
    }

    // PART 2:
    sr = srp2;
    sc = scp2;

    pos_q_len = 0;

    uint8_t batch_qp2[32][3];
    batch_q_len = 0;

    pos_q[pos_q_len][0] = sr;
    pos_q[pos_q_len++][1] = sc;

    cr = sr;
    cc = sc;
    do_move = 0;
    for (uint16_t i = 0; i < directions_list_len; i++){
        do_move = 1;
        int8_t dr = 0;
        int8_t dc = 0;
        pos_q_len = 0;
        pos_q[pos_q_len][0] = sr;
        pos_q[pos_q_len++][1] = sc; // Append current position to Q
        batch_q_len = 0;
        batch_qp2[batch_q_len][0] = sr;
        batch_qp2[batch_q_len][1] = sc; // Append current position to batch Q
        batch_qp2[batch_q_len++][2] = '@';
        if (directions_list[i] == '<'){dr =  0; dc = -1;}
        if (directions_list[i] == '>'){dr =  0; dc =  1;}
        if (directions_list[i] == '^'){dr = -1; dc =  0;}
        if (directions_list[i] == 'v'){dr =  1; dc =  0;}

        while (pos_q_len > 0){
            cr = pos_q[pos_q_len-1][0];
            cc = pos_q[pos_q_len-1][1];
            pos_q_len--;
            uint8_t nr = cr + dr;
            uint8_t nc = cc + dc;
            if (map_p2[nr][nc] == '#'){
                do_move = 0;
                break;
            };
            if (map_p2[nr][nc] == ']'){
                if (dr == 0 & dc == -1){  // < add both sizes too batch Q, skip right side in pos Q.
                    batch_qp2[batch_q_len][0] = nr;
                    batch_qp2[batch_q_len][1] = nc;
                    batch_qp2[batch_q_len++][2] = ']';
                    pos_q[pos_q_len][0] = nr;
                    pos_q[pos_q_len++][1] = nc-1;
                    batch_qp2[batch_q_len][0] = nr;
                    batch_qp2[batch_q_len][1] = nc-1;
                    batch_qp2[batch_q_len++][2] = '[';
                }
                if ((dr == -1 & dc == 0) | (dr == 1 & dc == 0)){ // ^ | v
                    batch_qp2[batch_q_len][0] = nr;
                    batch_qp2[batch_q_len][1] = nc;
                    batch_qp2[batch_q_len++][2] = ']';
                    pos_q[pos_q_len][0] = nr;
                    pos_q[pos_q_len++][1] = nc;
                    if (!in_queuep2(batch_qp2, nr, nc-1, batch_q_len)){ // add [ if it's not already in Q
                        batch_qp2[batch_q_len][0] = nr;                 // TODO: These queues are still getting too big
                        batch_qp2[batch_q_len][1] = nc-1;               
                        batch_qp2[batch_q_len++][2] = '[';
                        pos_q[pos_q_len][0] = nr;
                        pos_q[pos_q_len++][1] = nc-1;
                    }
                }
            }
            if (map_p2[nr][nc] == '['){
                if (dr == 0 & dc == 1){  // > add both sizes too batch Q, skip right side in pos Q.
                    batch_qp2[batch_q_len][0] = nr;
                    batch_qp2[batch_q_len][1] = nc;
                    batch_qp2[batch_q_len++][2] = '[';
                    pos_q[pos_q_len][0] = nr;
                    pos_q[pos_q_len++][1] = nc+1;
                    batch_qp2[batch_q_len][0] = nr;
                    batch_qp2[batch_q_len][1] = nc+1;
                    batch_qp2[batch_q_len++][2] = ']';
                }
                if ((dr == -1 & dc == 0) | (dr == 1 & dc == 0)){ // ^ | v
                    batch_qp2[batch_q_len][0] = nr;
                    batch_qp2[batch_q_len][1] = nc;
                    batch_qp2[batch_q_len++][2] = '[';
                    pos_q[pos_q_len][0] = nr;
                    pos_q[pos_q_len++][1] = nc;
                    if (!in_queuep2(batch_qp2, nr, nc+1, batch_q_len)){ // add ] if it's not already in Q
                        batch_qp2[batch_q_len][0] = nr;
                        batch_qp2[batch_q_len][1] = nc+1;
                        batch_qp2[batch_q_len++][2] = ']';
                        pos_q[pos_q_len][0] = nr;
                        pos_q[pos_q_len++][1] = nc+1;
                    }
                }
            }
        }
        if (!do_move)continue;
        for (int8_t i=batch_q_len-1; i>=0; i--){ // Inefficient as we're looping through batch of boxes twice, batch is small though
            map_p2[batch_qp2[i][0]][batch_qp2[i][1]] = '.';
        }
        for (int8_t i=batch_q_len-1; i>=0; i--){
            map_p2[batch_qp2[i][0]+dr][batch_qp2[i][1]+dc] = batch_qp2[i][2];
        }
    
        sr = sr + dr;
        sc = sc + dc;
        // if (i >= 17504){
        //     print_map2(map_p2);

        //     printf("%u\n", i);
        //     char t = getchar();
        // }
    }

    uint64_t sum2 = 0;
    for (uint8_t i=0; i<map_w; i++){
        for (uint8_t j=0; j<map_w*2; j++){
            if (map_p2[i][j] == '['){
                sum2 = sum2 + (100*i + j);
            }
        }
    }

    printf("sum1: %lu\nsum2: %lu\n", sum1, sum2);

}