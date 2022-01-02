#version 330 core
out vec4 FragColor;

struct Light
{
vec3 ambient;
vec3 diffuse;
vec3 specular;

vec3 position;

float range;
};

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

in vec3 fragCoord;
in vec3 color;
in vec2 texCoord;
in vec3 normal;

uniform sampler2D ourTexture;

uniform vec3 cameraPosition;

uniform Light light1;
uniform Material material;
//uniform bool renderTexture;


void main()
{
// ambient light
vec3 ambient1Light = light1.ambient * material.ambient;

float distanceToLight = length(light1.position - fragCoord);

vec3 diffuse1Light;
vec3 specular;

//if(distanceToLight <= light1.range)
//{
float distanceLightModificator = 1.0f / distanceToLight;

// diffuse light
vec3 distanceToLight1 = normalize(light1.position - fragCoord);
vec3 normalizedNormal = normalize(normal);
float diffuse1Arc = max(dot( distanceToLight1 , normalizedNormal ) , 0.0 );

 diffuse1Light =  ( material.diffuse * light1.diffuse * diffuse1Arc);
// diffuse1Light = distanceLightModificator * diffuse1Light;

// specular light
vec3 distanceToCamera = normalize(cameraPosition - fragCoord);
vec3 reflectDir = reflect(-distanceToLight1, normalizedNormal);  

float specular1Light = pow(max(dot(distanceToCamera, reflectDir), 0.0), material.shininess);
 specular = light1.specular * material.specular * specular1Light;

// specular = specular * distanceLightModificator;
//}
//else
//{
//specular = vec3(0.0);
//diffuse1Light = vec3(0.0);
//}



// final light
vec3 finalLight =  diffuse1Light + specular;
 finalLight = finalLight * distanceLightModificator + ambient1Light ;

//    FragColor = vec4(color, 1.0f);	// color output from vertex shader
//    FragColor = texture(ourTexture, texCoord);	// texture from sampler
//      FragColor = texture(ourTexture, texCoord) * vec4(color * finalLight , 1.0f);	// mix of both
      FragColor =  vec4(color * finalLight , 1.0f);	// mix of both
}