#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 v_TexCoord;

out vec2 fragTexCoord;

void main()
{
   gl_Position = position;
   fragTexCoord = v_TexCoord;

};



#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 fragTexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, fragTexCoord);
	color = texColor;
	//color = vec4(fragTexCoord.xy,0,1);
};

