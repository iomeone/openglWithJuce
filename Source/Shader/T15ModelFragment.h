out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
vec3 light = vec3(0.2f, 0.6f, 0.8f);
uniform Material material;


void main()
{    
	FragColor = texture(texture_diffuse1, TexCoords);
/*   	FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
	vec3 result = light * material.ambient + light * material.diffuse + light * material.specular;
	result += material.shininess*0.0000001;
	
	FragColor = vec4(result, 1.0f);*/

}