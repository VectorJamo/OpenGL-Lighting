#version 330 core

in vec2 tCoord;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 color;

// Uniforms
uniform vec3 directionalLightDir;
uniform vec3 pointLight1Pos;
uniform vec3 pointLight2Pos;
uniform vec3 eyePos;

uniform vec3 cameraDirection;
uniform vec3 spotLightColor;
//uniform vec3 cutOffAngleInside, cutOffAngleOutside;

uniform vec3 pointLightColor;
uniform vec3 pointLightColor2; // Another color for testing purposes
uniform vec3 directionalLightColor;

// Materials
struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	int shininess;
};

uniform Material material;

// Functions
vec3 CalculateDirectionalLight();
vec3 CalculatePointLight(vec3 pointLightPosition, vec3 pointLightColor);
vec3 CalculateSpotLight(float innerCutOffAngle, float outerCutOffAngle);

void main()
{
	vec3 finalColor = CalculateDirectionalLight() + CalculatePointLight(pointLight1Pos, pointLightColor) + CalculatePointLight(pointLight2Pos, pointLightColor2) + CalculateSpotLight(radians(10.0), radians(12.0));

	color = vec4(finalColor, 1.0);
}

vec3 CalculateSpotLight(float innerCutOffAngle, float outerCutOffAngle)
{
	vec3 fragmentColor = vec3(0.0, 0.0, 0.0);

	float innerCutOff = cos(innerCutOffAngle);
	float outerCutOff = cos(outerCutOffAngle);
	float epsilon = innerCutOff - outerCutOff;

	vec3 eyeToFragVector = fragWorldPos - eyePos;
	float cosineAngle = dot(normalize(cameraDirection), normalize(eyeToFragVector));

	float dist = abs(length(eyeToFragVector));
	float spotLightIntensity = clamp((cosineAngle - outerCutOff)/epsilon, 0.0, 1.0);

	// Diffuse light
	float diffuseStrength = spotLightIntensity * max(dot(normalize(-eyeToFragVector), fragNormal), 0.0);
	vec3 diffuseLight = diffuseStrength * spotLightColor;

	vec3 diffuse = diffuseLight * vec3(texture(material.diffuse, tCoord));

	fragmentColor += diffuse;

	// Specular light
	vec3 eyeToFragNormalized = normalize(eyeToFragVector);
	vec3 fragToEyeVector = -eyeToFragNormalized;
	vec3 reflectedLight = reflect(eyeToFragNormalized, fragNormal);
	
	float specularStrength = spotLightIntensity * pow(max(dot(reflectedLight, fragToEyeVector), 0.0), material.shininess);
	vec3 specular = specularStrength * vec3(texture(material.specular, tCoord));

	fragmentColor += specular;

	return fragmentColor;
}

vec3 CalculatePointLight(vec3 pointLightPosition, vec3 pointLightColor)
{
	vec3 fragmentColor;

	float fragToLight1Distance = length(fragWorldPos - pointLightPosition);
	float k = 0.1;
	float intensity = 1.0 / (k*fragToLight1Distance*fragToLight1Distance); 
	
	// Ambient light
	vec3 ambientLight = intensity * pointLightColor;

	fragmentColor = ambientLight * vec3(texture(material.diffuse, tCoord));

	// Diffuse light
	vec3 fragToLight1Vector = normalize(pointLightPosition - fragWorldPos);
	
	float diffuseIntensity = dot(fragToLight1Vector, fragNormal);
	vec3 diffuseLight = intensity * diffuseIntensity * pointLightColor;

	vec3 diffuse = diffuseLight * vec3(texture(material.diffuse, tCoord));

	fragmentColor += diffuse;

	// Specular light
	vec3 fragToEyeVector = normalize(eyePos - fragWorldPos);
	vec3 light1ToFragVector = normalize(fragWorldPos - pointLightPosition);

	vec3 reflectedLight = normalize(reflect(light1ToFragVector, fragNormal));

	float specularStrength = pow(max(dot(fragToEyeVector, reflectedLight), 0.0), material.shininess);
	vec3 specularLight = intensity * specularStrength * pointLightColor;
	
	vec3 specular = specularLight * vec3(texture(material.specular, tCoord));

	fragmentColor += specular;

	return fragmentColor;
}

vec3 CalculateDirectionalLight()
{
	vec3 fragmentColor;
	
	// Ambient light
	float ambientLightStrength = 0.2;
	vec3 ambientLight = ambientLightStrength * directionalLightColor;

	fragmentColor = ambientLight * vec3(texture(material.diffuse, tCoord));

	// Diffuse light
	float diffuseStrength = max(dot(normalize(-directionalLightDir), fragNormal), 0.0);
	vec3 diffuseLight = directionalLightColor * diffuseStrength;

	vec3 diffuse = diffuseLight * vec3(texture(material.diffuse, tCoord));

	fragmentColor += diffuse;

	return fragmentColor;
}