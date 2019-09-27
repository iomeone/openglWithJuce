out vec4 FragColor;

//uniform float mixValue;
uniform sampler2D ourTextureBox;
uniform sampler2D ourTextureFace;

in vec3 ourColor;
in vec2 ourTexCoord;

void main()
{

	//FragColor = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0) + uniformColor;
	//FragColor = texture(ourTextureBox, ourTexCoord);

	FragColor = mix(texture(ourTextureBox, ourTexCoord), texture(ourTextureFace, ourTexCoord), 0.2);
}


