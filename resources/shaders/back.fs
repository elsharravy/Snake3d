#version 330 core
out vec4 FragColor;

in vec3 fragCoord;
in vec2 texCoord;
in vec3 normal;
//uniform bool renderTexture;

uniform sampler2D texture_diffuse1;

void main()
{
    FragColor = texture( texture_diffuse1 , texCoord );	// color output from vertex shader
//    FragColor = vec4(1.0f, 0.0f, 0.0f , 1.0f);	// color output from vertex shader
//    FragColor = texture(ourTexture, texCoord);	// texture from sampler
//      FragColor = texture(ourTexture, texCoord) * vec4(color, 1.0f);	// mix of both
}