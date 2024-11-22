#version 410  
in vec3 colorForFragmentShader;  
uniform float ambientFactor;
out vec4 colorOfMyChoice;

void main()  
{  
    colorOfMyChoice = vec4(ambientFactor*colorForFragmentShader,1.0);  
};
 