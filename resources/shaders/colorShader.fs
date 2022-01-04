#version 330 core
out vec4 FragColor;

in vec3 color;

//uniform bool renderTexture;


void main()
{
    FragColor = vec4(color, 1.0f);	// color output from vertex shader
 //   FragColor = vec4(0.0f,1.0f, 0.0f, 1.0f);	// color output from vertex shader
}