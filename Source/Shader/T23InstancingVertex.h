layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aOffset;

out vec3 fColor;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
	fColor = aColor;
	
	gl_Position = projection * view * model * vec4( vec3(aPos + aOffset, 0.0), 1.0);

	//gl_Position = vec4(aPos + aOffset, 0.0, 1.0);
}



