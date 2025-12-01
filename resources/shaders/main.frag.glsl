#version 330 core

out vec4 FragColor;

// Inputs from vertex shader
in vec3 FragPos; 
in vec3 Normal;
in vec2 TexCoord;

// Textures
uniform sampler2D texture0;
uniform sampler2D texture1;

// Lighting uniforms
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

// Attenuation uniforms
uniform float constant;
uniform float linear;
uniform float quadratic;

void main()
{
  // 1. Ambient Lighting
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // 2. Diffuse Lighting
  vec3 norm = normalize(Normal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;
  
  // 3. Specular Lighting
  float specularStrength = 0.5;
  int shininess = 32;

  // Direction from frag to camera
  vec3 viewDir = normalize(viewPos - FragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
  vec3 specular = specularStrength * spec * lightColor;

  // 4. Attenuation
  // Calculate distance from light to this fragment
  float distance = length(lightPos - FragPos);

  // Apply attenuation formula
  float attenuation = 1.0 / (constant + linear + distance + quadratic * distance * distance);

  // Scale all lighting components by attenuation
  ambient *= attenuation; 
  diffuse *= attenuation;
  specular *= attenuation;


  // Combine Everything
  vec3 lighting = ambient + diffuse + specular;
  vec4 texColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);
  vec3 result = lighting * texColor.rgb;

  FragColor = vec4(result, 1.0);
}
