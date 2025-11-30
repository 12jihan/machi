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
uniform vec3 objectColor;

void main()
{
  // 1. Ambient Lighting
  // The "Everything" lighting
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;
  
  // 2. Diffuse Lighting
  // Normalizing the normal
  vec3 norm = normalize(Normal);
  // Calculating direction from frag to the light
  vec3 lightDir = normalize(lightPos - FragPos);
  // Dot product
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // Combine Everything
  // Adding ambient and diffuse together
  vec3 lighting = ambient + diffuse;

  // Texture color
  vec4 texColor = mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.5);

  // Final color = lighting * texture
  vec3 result = lighting * texColor.rgb;

  FragColor = vec4(result, 1.0);
}
