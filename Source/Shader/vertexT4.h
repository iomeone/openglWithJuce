attribute vec3  aPos;
attribute vec3  aColor;
attribute vec2  aTexCoord;

uniform mat4  transform;

out vec3 ourColor;
out vec2 ourTexCoord;
void main()
{
	ourColor = aColor;
	ourTexCoord = aTexCoord;


	gl_Position = transform * vec4(aPos, 1.0);
}
