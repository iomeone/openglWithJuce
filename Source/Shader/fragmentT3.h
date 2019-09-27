out vec4 FragColor;

uniform vec4 uniformColor;
uniform sampler2D ourTexture;

in vec3 ourColor;
in vec2 ourTexCoord;

void main()
{

	//FragColor = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0) + uniformColor;
	FragColor = texture(ourTexture, ourTexCoord);
}


