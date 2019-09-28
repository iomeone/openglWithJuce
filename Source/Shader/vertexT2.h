attribute vec3  aPos;
attribute vec3  aColor;
varying vec3 ourColor;
void main()
{
	ourColor = aColor;
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}

