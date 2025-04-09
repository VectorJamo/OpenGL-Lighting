#version 330 core

in vec3 vertNormal;
in vec3 fragWorldPosition;
in vec2 tCoord;

out vec4 color;

uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 objectColor;

uniform vec3 cameraPosition;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

uniform Material material;

void main()
{
	// Ambient light
	float ambientStrength = 0.2f;
	vec3 ambientLight = lightColor * ambientStrength;

	vec3 ambient = ambientLight * material.ambient;

	// Diffuse light
	vec3 fragmentToLightVector = normalize(lightPosition - fragWorldPosition);
	vec3 normalVector = vertNormal;
	float diffuseIntensity = max(dot(fragmentToLightVector, normalVector), 0.0);
	vec3 diffuseLight = diffuseIntensity * lightColor;
	
	vec3 diffuse = diffuseLight * material.diffuse;
	
	// Specular light
	vec3 fragToEyeVector = normalize(cameraPosition - fragWorldPosition);
	vec3 lightToFragVector = normalize(fragWorldPosition - lightPosition);
	vec3 reflectedVector = reflect(lightToFragVector, vertNormal);
	float specularStrength = pow(max(dot(reflectedVector, fragToEyeVector), 0.0), material.shininess);

	vec3 specularLight = specularStrength * material.specular;

	color = vec4(ambient + diffuse + specularLight, 1.0f);
}