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
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

uniform Material material;

uniform vec3 directionalLight;
uniform vec3 spotLightDirection;

void main()
{
	// Directional light
	float directionalLightStrength = max(dot(vertNormal, -normalize(directionalLight)), 0.0);
	vec3 directionalLight = directionalLightStrength * lightColor;

	vec3 directional = vec3(texture(material.diffuse, tCoord)) * directionalLightStrength;

	// Point light
	float k = 0.5;
	float lightToFragDistance = length(fragWorldPosition - lightPosition);
	float lightIntensity = 1.0/(k*lightToFragDistance); // Linear falloff

	// Spot light
	float innerCutOffAngle = cos(radians(10.0));
	float outerCutOffAngle = cos(radians(15.0));
	
	vec3 eyeToFragVector = normalize(fragWorldPosition - cameraPosition);

	float cosineAngle = dot(spotLightDirection, eyeToFragVector);
	float epsilon = innerCutOffAngle - outerCutOffAngle;
	float spotLightIntensity = clamp((cosineAngle - outerCutOffAngle)/epsilon, 0.0, 1.0);

	vec3 spotLight = vec3(texture(material.diffuse, tCoord)) * spotLightIntensity;

	// Ambient light
	float ambientStrength = 0.2f;
	vec3 ambientLight = lightColor * ambientStrength;

	vec3 ambient = ambientLight * vec3(texture(material.diffuse, tCoord));

	// Diffuse light
	vec3 fragmentToLightVector = normalize(lightPosition - fragWorldPosition);
	vec3 normalVector = vertNormal;
	float diffuseIntensity = max(dot(fragmentToLightVector, normalVector), 0.0);
	vec3 diffuseLight = diffuseIntensity * lightColor;
	
	vec3 diffuse = diffuseLight * vec3(texture(material.diffuse, tCoord));
	diffuse *= lightIntensity;
	
	// Specular light
	vec3 fragToEyeVector = normalize(cameraPosition - fragWorldPosition);
	vec3 lightToFragVector = normalize(fragWorldPosition - lightPosition);
	vec3 reflectedVector = reflect(lightToFragVector, vertNormal);
	float specularStrength = pow(max(dot(reflectedVector, fragToEyeVector), 0.0), material.shininess);

	vec3 specular = specularStrength * vec3(texture(material.specular, tCoord));
	specular *= lightIntensity;

	color = vec4(ambient + diffuse + specular + directional + spotLight, 1.0f);
}