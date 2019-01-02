#include <stdio.h>
#include <time.h>
#include "engine.h"
#include <math.h>

int main(int argc, char **argv)
{
	int i,d;
	int ret, size;
	float aux;
	float xpos, ypos, zpos;
	float xpos_prev, ypos_prev, zpos_prev;
	double time, prev_time, diff_t;
	float yaw, yaw_rad, yaw_right_rad;
	int keys[5];
	float dir[3] = {0};
	float dir_right[3] = {0};
	float dir_sum[3] = {0};
	float yacc = 0.0;
	int jumping = 0;

	if(init_engine()==0) return 1;

	prev_time=0.0;

	int *height = get_height(&size);
	xpos = (float) size / 2.0 * 0.1;
	zpos = -(float) size / 2.0 * 0.1;
	ypos = 0.1 * height[size*(size/2)+(size/2)] + 0.4;

	set_pos(xpos,ypos,zpos);
	refresh();

	while (1) {
		ret = display(&time);
		if (ret == 1) break;
		diff_t = time-prev_time;
		prev_time = time;
		get_keys(keys);
		get_dir(&yaw);
		
		yaw_rad = toRad(yaw);
		yaw_right_rad = toRad(yaw-90.0);
		dir[0] = cos(yaw_rad) * diff_t * 0.5;
		dir[2] = sin(yaw_rad) * diff_t * 0.5;

		dir_right[0] = cos(yaw_right_rad) * diff_t * 0.5;
		dir_right[2] = sin(yaw_right_rad) * diff_t * 0.5;

		dir_sum[0] = dir[0]+dir_right[0];
		dir_sum[2] = dir[2]+dir_right[2];
		aux = dir_sum[0]*dir_sum[0]+dir_sum[2]*dir_sum[2];
		aux = sqrtf(aux);
		dir_sum[0] = dir_sum[0]/aux * diff_t * 0.5;
		dir_sum[2] = dir_sum[2]/aux * diff_t * 0.5;

		xpos_prev = xpos;
		ypos_prev = ypos;
		zpos_prev = zpos;

		if (keys[0] && !keys[2] && !keys[3]) {
			xpos -= dir[0];
			zpos -= dir[2];
		} else if (keys[1] && !keys[2] && !keys[3]) {
			xpos += dir[0];
			zpos += dir[2];
		} else if (keys[2] && !keys[0] && !keys[1]) {
			xpos -= dir_right[0];
			zpos -= dir_right[2];
		} else if (keys[3] && !keys[0] && !keys[1]) {
			xpos += dir_right[0];
			zpos += dir_right[2];
		} else if (keys[0] && keys[2]) {
			xpos -= dir_sum[0];
			zpos -= dir_sum[2];
		} else if (keys[0] && keys[3]) {
			xpos += dir_sum[0];
			zpos -= dir_sum[2];
		} else if (keys[1] && keys[2]) {
			xpos -= dir_sum[0];
			zpos += dir_sum[2];
		} else if (keys[1] && keys[3]) {
			xpos += dir_sum[0];
			zpos += dir_sum[2];
		}

		if (keys[4] && jumping==0) {
			yacc = 3.0;
			jumping = 1;
		}
		i = (int)(fabs(zpos/0.1-1.0));
		d = (int)(xpos/0.1);

		aux = 0.1*height[size*i+d] + 0.2;

		if (fabs(aux-ypos) < 0.03){
			set_pos(xpos,ypos,zpos);
			if (yacc < 3.0) jumping = 0;
			if (yacc < 1.0) {
				yacc = -0.1;
			}
		} else if (aux > ypos) {
			xpos = xpos_prev;
			ypos = ypos_prev;
			zpos = zpos_prev;
		} 
		
		if (aux < ypos || jumping) {
			ypos += diff_t * yacc;
			yacc -= 20.0 * diff_t;
			if (yacc < -3.0) {
				yacc = -3.0;
			}
			set_pos(xpos,ypos,zpos);
		}

	}
	end_engine();
	return 0;
}
