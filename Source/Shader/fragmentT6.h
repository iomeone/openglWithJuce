//uniform float mixValue;
uniform sampler2D ourTextureBox;
uniform sampler2D ourTextureFace;

varying vec2 ourTexCoord;

void main()
{

	//FragColor = vec4(ourColor.r, ourColor.g, ourColor.b, 1.0) + uniformColor;
	//FragColor = texture(ourTextureBox, ourTexCoord);

	gl_FragColor = mix(texture2D(ourTextureBox, ourTexCoord), texture2D(ourTextureFace, ourTexCoord), 0.2);
}


