uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

varying vec3 Normal;
varying vec3 FragPos;




void main()
{
	// caculate the diffuse color
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0);

	//gl_FragColor = vec4(Normal, 1.0);
	//if(diff <= 0.)
	//	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	//else 
	//	gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);

	vec3 diffuse = diff * lightColor;



	// caculate the specular color 
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;



	// caculate the ambient color
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;



	//return the final color
	gl_FragColor = vec4( (ambient + diffuse + specular) * objectColor, 1.0);


}