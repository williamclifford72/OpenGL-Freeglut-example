#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 a_normal; 

out vec2 v_texCoord;
out vec3 v_normal; 
out vec3 v_fragPos;

uniform mat4 u_MVP;
uniform mat4 model;



void main()
{
	v_texCoord = texCoord;
	v_normal = mat3(transpose(inverse(model))) * a_normal;
	v_fragPos = vec3(model * vec4(position, 1.0));
	gl_Position = u_MVP * vec4(position,1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;
in vec3 v_normal;
in vec3 v_fragPos; 

uniform vec3 light_pos; 
uniform sampler2D u_Texture;

void main()
{

    vec4 texColor = texture(u_Texture, v_texCoord);
	
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
	vec3 norm = normalize(v_normal);
	vec3 lightDir = normalize(light_pos - v_fragPos);

	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;
	vec3 result = (ambient + diffuse) * texColor.xyz;

    color = vec4(result, 1.0);
};