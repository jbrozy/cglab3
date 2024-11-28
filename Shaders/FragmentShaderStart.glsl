#version 410  

uniform float ambientFactor;
uniform float diffuseFactor;
uniform int shininessFactor;
uniform float specularFactor;

out vec4 colorOfMyChoice;

in vec3 outNormals;
in vec3 fragPosition;
in vec3 colorForFragmentShader;  

uniform vec3 lightPos = vec3(0.0, 0.0, 1.0);
uniform vec3 cameraPosition = vec3(0.0, 0.0, 1.0);

void main()  
{
    vec3 lightDir = normalize(lightPos - fragPosition);
    vec3 viewDir = normalize(cameraPosition - fragPosition);

    vec3 reflectDir = reflect(-lightDir, outNormals);
    float diffuse = (max(dot(outNormals, lightDir), 0.0)) * max(diffuseFactor, 0.0);
    float specular = pow(max(dot(viewDir, reflectDir), 0.0), max(shininessFactor, 1.0)) * max(specularFactor, 0.0);
    colorOfMyChoice = vec4((ambientFactor + diffuse + specular) * colorForFragmentShader, 1.0);  
};
 