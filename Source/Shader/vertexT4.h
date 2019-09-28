attribute vec3  aPos;
attribute vec3  aColor;
attribute vec2  aTexCoord;

uniform mat4  transform;

varying vec3 ourColor;
varying vec2 ourTexCoord;
void main()
{
	ourColor = aColor;
	ourTexCoord = aTexCoord;


	gl_Position = transform * vec4(aPos, 1.0);
}
