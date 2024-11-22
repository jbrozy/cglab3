#version 410
uniform mat4 matrix;
in vec3 color;
in vec3 position;
out vec3 colorForFragmentShader;

void main()
{
    gl_Position = matrix * vec4(position, 1.0);
       
    colorForFragmentShader = color;
};