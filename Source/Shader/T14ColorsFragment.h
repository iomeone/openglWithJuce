out vec4 FragColor;

uniform sampler2D material_diffuse;
uniform sampler2D material_specular;
uniform float material_shininess;



uniform vec3 light_position;


uniform vec3 light_direction;
uniform float light_cutOff;
uniform float light_outerCutOff;

uniform vec3 light_diffuse;
uniform vec3 light_specular;
uniform vec3 light_ambient;

uniform float light_constant;
uniform float light_linear;
uniform float light_quadratic;



in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform vec3 viewPos;



void main()
{
	// ambient
	vec3 ambient = light_ambient * texture(material_diffuse, TexCoords).rgb;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light_position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light_diffuse * diff * texture(material_diffuse, TexCoords).rgb;

	// specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
	vec3 specular = light_specular * spec * texture(material_specular, TexCoords).rgb;

	// spotlight (soft edges)
	float theta = dot(lightDir, normalize(-light_direction));
	float epsilon = (light_cutOff - light_outerCutOff);
	float intensity = clamp((theta - light_outerCutOff) / epsilon, 0.0, 1.0);
	diffuse *= intensity;
	specular *= intensity;

	// attenuation
	float distance = length(light_position - FragPos);
	float attenuation = 1.0 / (light_constant + light_linear * distance + light_quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
