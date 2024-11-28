#version 410

uniform mat4 matrix;
uniform vec3 lightDir;

in vec3 color;
in vec3 position;
in vec3 normals;

out vec3 colorForFragmentShader;
out vec3 outNormals;

out vec3 fragPosition;
out vec3 lightDirection;

void main()
{
    gl_Position = matrix * vec4(position, 1.0);
    fragPosition = (matrix * vec4(position, 1.0)).xyz;
       
    colorForFragmentShader = color;
    outNormals = (matrix * vec4(normals, 1.0)).xyz;
    lightDirection = (matrix * vec4(lightDir, 1.0)).xyz;
};