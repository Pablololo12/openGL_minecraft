#version 330 core
/*
 * Fragment shader to show final image
 */
out vec4 color;

uniform vec3 light;
uniform sampler2D textur[1];

in vec3 pos;
in vec3 norm;
in vec2 UV;

void main()
{
	vec3 vec_light = normalize(light-pos);
	vec3 vec_v = normalize(pos-light);
	vec3 specular_dir = reflect(-light,norm);
	vec3 reflected_dir = reflect(vec_v,norm);
	float l_power = 1/pow(distance(light,pos),2);
	float dot_l = abs(dot(vec_light,norm));

	vec3 difs = dot_l*l_power*vec3(1.0,1.0,1.0); //Kd(N*L)Cd
	vec3 amb = vec3(1.0,1.0,1.0); //ka Cd Cl
	vec3 spe = dot_l*pow(dot(specular_dir,vec_v),2.0)*vec3(1.0,1.0,1.0)*l_power; //ks (N*L)(R*V)Cs Cl

	vec3 obj = difs*0.15 + amb*0.4 + spe*0.0;
	obj = clamp(obj,0.0,1.0);
	//vec3 cube_col = vec3(0.2,0.8,0.3);
	vec3 cube_col = texture(textur[0], UV).rgb;
	color = vec4(obj*cube_col, 1.0);
}
