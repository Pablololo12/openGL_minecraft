#ifndef _ENGINE_H_
#define _ENGINE_H_

int init_engine();
int display(double *time);
int end_engine();
int refresh();
int* get_height(int *size);
int get_keys(int* k);
int set_pos(float x, float y, float z);
int get_dir(float* ya);

inline float toRad(float deg)
{
	return deg* 3.14159265359 / 180.0;
}



#endif
