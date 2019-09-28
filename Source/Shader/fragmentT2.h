uniform vec4 uniformColor;
in vec3 ourColor;

void main()
{

	gl_FragColor = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0) + uniformColor;
}


