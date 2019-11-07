#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int tmode;

void main()
{
    {
        if(tmode==1)
            FragColor=texture(texture1, TexCoord);
        if(tmode==2)
            FragColor=texture(texture2, TexCoord);
    }
}