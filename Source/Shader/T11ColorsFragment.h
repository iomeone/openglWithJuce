uniform vec3 viewPos;

varying vec3 Normal;
varying vec3 FragPos;



uniform vec3 material_ambient;
uniform vec3 material_diffuse;
uniform vec3 material_specular;
uniform float material_shininess;

uniform vec3 light_position;
uniform vec3 light_diffuse;
uniform vec3 light_specular;
uniform vec3 light_ambient;


void main()
{
	// caculate the diffuse color
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_position - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);

	//gl_FragColor = vec4(Normal, 1.0);
	//if(diff <= 0.)
	//	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//else 
	//	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);

	vec3 diffuse = light_diffuse * (diff * material_diffuse);



	// caculate the specular color 
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
	vec3 specular = light_specular * (spec * material_specular) ;



	// caculate the ambient color
	float ambientStrength = 0.1;
	vec3 ambient = material_ambient * light_ambient;



	//return the final color
	gl_FragColor = vec4( (ambient + diffuse + specular), 1.0);


}