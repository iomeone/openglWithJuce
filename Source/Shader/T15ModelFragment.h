out vec4 FragColor;

uniform sampler2D texture_diffuse1;
//uniform sampler2D texture_diffuse2;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

// we add an light!
struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


uniform vec3 viewPos;
uniform Material material;
uniform Light light;


void main()
{    
	vec3 modelDiffuse = texture(texture_diffuse1, TexCoords).rgb;
	modelDiffuse = modelDiffuse * 10.0f;

	// ambient
	vec3 ambient = light.ambient *material.ambient;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * modelDiffuse);

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);




	//FragColor = texture(texture_diffuse1, TexCoords) * 6.0;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
	//FragColor = vec4(ambient, 1.0);
	//FragColor = vec4(diffuse, 1.0);
   	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	
	/*vec3 result = light * material.ambient + light * material.diffuse + light * material.specular;
	result += material.shininess*0.0000001;
	
	FragColor = vec4(result, 1.0f);*/

}