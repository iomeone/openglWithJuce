attribute vec3  aPos;
attribute vec2  aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

varying vec2 ourTexCoord;
void main()
{
	ourTexCoord = aTexCoord;


	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
