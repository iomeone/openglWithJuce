uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	gl_FragColor = vec4(ambient  * objectColor, 1.0);
}