#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"

float * open_image(char *name, int *width, int *height)
{
	FILE *f;
	char magic[4];
	float *img;
	int max;
	float maxf;
	unsigned char *buf;
	int i;

	f = fopen(name, "r");
	if (f==NULL)
		return NULL;
	fscanf(f,"%s\n%d %d\n%d\n", magic, width, height, &max);
	printf("%s\n%d %d\n%d\n", magic, *width, *height, max);
	maxf = (float)max;
	
	buf = (unsigned char *)malloc((*width)*(*height)*3*sizeof(unsigned char));
	img = (float *)malloc((*width)*(*height)*3*sizeof(float));

	fread(buf, 1, (*width)*(*height)*3, f);

	for (i=0; i<(*width)*(*height)*3; i+=3) {
		img[i] = (float)(buf[i])/maxf;
		img[i+1] = (float)(buf[i+1])/maxf;
		img[i+2] = (float)(buf[i+2])/maxf;
	}
	fclose(f);

	return img;
}
