#version 330 core
/*
 * Vertex shader to show final image
 */
layout(location=0) in vec3 vpos;
layout(location=1) in vec3 vnos;
layout(location=2) in vec2 vUV;

uniform mat4 view;
out vec3 pos;
out vec3 norm;
out vec2 UV;

void main()
{
	pos = vec3(vpos.x,vpos.y,vpos.z);
	norm = vec3(-vnos.x,vnos.z,vnos.y);
	UV = vUV;
	gl_Position = view * vec4(vpos,1.0);
}
