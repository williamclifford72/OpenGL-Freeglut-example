#shader vertex
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_texCoord;

uniform mat4 u_MVP;

void main()
{
	v_texCoord = texCoord;
	gl_Position = u_MVP * vec4(position,1.0);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform vec4 cube_color; // used to highlight the where the light is
uniform sampler2D u_Texture;

void main()
{
	if (cube_color.w == 1.0)
	{
		color = cube_color;
	}
	else
	{
		vec4 texColor = texture(u_Texture, v_texCoord);
		color = texColor;
	}
    
};