#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragVertexPos;

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

void main()
{
	// ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(1, 1, 1);
  	
    // diffuse 
	// vec3 lightPos (0.0f,0.0f,0.0f);
	// vec3 lightColor (1.0f, 1.0f, 1.0f);

    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize( vec3(0, 2, 0) - fragVertexPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1, 1, 1);

	vec3 result = (ambient + diffuse) * fragColor;
    outColor = texture(texSampler, fragTexCoord) * vec4(result, 1.0f);
}
