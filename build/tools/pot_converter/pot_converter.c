// FIXME: stbir_resize_uint8() segfault
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"        /* Loading Textures */
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h" /* Resizing Textures */
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"  /* Outputting Textures */

typedef struct{
	stbi_uc * data;
	int32_t w, h, c;
} Texture;

Texture load_texture(const char * filename);
int8_t is_power_of_2(int32_t number);
int32_t closest_power_of_2(int32_t number);

int main(int argc, char * argv[]){
	if (argc < 2){return -1;}
	Texture tex = load_texture(argv[1]);
	int32_t w, h, s, c;
	stbi_info(argv[1], &w, &h, &c);
	printf("[%s] : [%dx%d, %d]\n", argv[1], w, h, c);
	if (closest_power_of_2(w) == w && closest_power_of_2(h) == h){
		printf("Image is already power of 2\n");
		return 0;
	}
	printf("%dx%d => %dx%d\n", w, h, closest_power_of_2(w), closest_power_of_2(h));
	unsigned char * data;
	stbir_resize_uint8(tex.data, tex.w, tex.h, 0, data, closest_power_of_2(w), closest_power_of_2(h), 0, c);
	stbi_write_png("out.png", closest_power_of_2(w), closest_power_of_2(h), c, data, closest_power_of_2(w) * c);
	stbi_info("out.png", &w, &h, &c);
	printf("[out.png] : [%dx%d, %d]\n", w, h, c);
	stbi_image_free(tex.data);
	return 0;
}

Texture load_texture(const char * filename){
	int32_t w, h, c;
	Texture tex = {
		.data = stbi_load(filename, &w, &h, &c, 0),
		.w = w,
		.h = w,
		.c = c
	};
	if (tex.data){
		printf("Successfully loaded [%s]\n", filename);
	}else{
		printf("Failed loading [%s]\n", filename);
		exit(-1);
	}
	return tex;
}

int8_t is_power_of_2(int32_t number){
    int oldNumber = 0;
    int suma = 0;
    int i, number_bezEdno;
    //char number_char[20] =          { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
    char *number_char = NULL;
    //char number_char_bezEdno[20] =  { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' };
    char *number_char_bezEdno = NULL;
    int NumberOfBitsNeeded = 1;

    /* First we have to determine how many bits are needed to store the user input and allocate memory for the arrays */
    /* The constarains of this cycle is 64 because this is the maximum size of INT in 64bit mashine */

    oldNumber = number;
    for (i = 0; i < 64; i++)
    {
        oldNumber /= 2;
        if (oldNumber) NumberOfBitsNeeded++;
    }

    /* Allocate memory */
    number_char = (char *)calloc(NumberOfBitsNeeded, sizeof(char));
    number_char_bezEdno = (char *)calloc(NumberOfBitsNeeded, sizeof(char));
    if (number_char == NULL || number_char_bezEdno == NULL){
        return -1;
    }

    oldNumber = number;
    for (i = 0; i < NumberOfBitsNeeded; i++)
    {
        if (number % 2 == 1)
            number_char[i] = '1';
        else if (number % 2 == 0)
            number_char[i] = '0';
        number /= 2;
        if (number == 0) break;
    }

    number_bezEdno = oldNumber - 1;
    for (i = 0; i < NumberOfBitsNeeded; i++)
    {
        if (number_bezEdno % 2 == 1)
            number_char_bezEdno[i] = '1';
        else if (number_bezEdno % 2 == 0)
            number_char_bezEdno[i] = '0';
        number_bezEdno /= 2;
        if (number_bezEdno == 0) break;
    }

    for (i = 0; i < NumberOfBitsNeeded; i++)
    {
        if ((number_char_bezEdno[i] - 48) & (number_char[i] - 48)) suma++;
    }
    if (!suma){
		return 0;
	}

	return -1;
}

int32_t closest_power_of_2(int32_t number){
	if (is_power_of_2(number) == 0){
		return number;
	}
	int32_t min = number-1;
	int32_t max = number+1;
	for (; min > 0; --min){
		if (is_power_of_2(min) == 0){
			break;
		}
	}
	for (; max < 10000; ++max){
		if (is_power_of_2(max) == 0){
			break;
		}
	}
	if (abs((max - number)) < abs((min - number))){
		return max;
	}else{
		return min;
	}
}
