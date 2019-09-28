attribute vec3  aPos;
attribute vec3  aColor;
attribute vec2  aTexCoord;


varying vec3 ourColor;
varying vec2 ourTexCoord;
void main()
{
	ourColor = aColor;
	ourTexCoord = aTexCoord;


	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

