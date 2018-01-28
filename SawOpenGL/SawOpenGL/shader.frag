#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture0;
uniform vec3 lightColor;
uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform float ambientLightIntensity;
uniform float specularLightIntensity;

void main()
{
    color = texture(Texture0, TexCoord);
    vec3 ambient = ambientLightIntensity * lightColor;

  	vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosition - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPosition - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularLightIntensity * spec * lightColor;
            
    vec3 result = (ambient + diffuse + specular) * vec3(color);
    color = vec4(result, 1.0);
}
