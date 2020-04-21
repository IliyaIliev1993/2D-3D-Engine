#version 330 core
out vec4 FragColor;

in vec2 oTexCoord;

uniform float fTime;
uniform sampler2D iTexture;
uniform vec4 vColor;

void main()
{
    vec2 p = -1.0 + 2.0 * oTexCoord;
    float len = length(p / 1);
    vec2 uv = oTexCoord + (p/len)*cos((len * 12.0 - fTime * 4.0))*0.03;
    vec3 col = texture2D(iTexture,uv).xyz;
    gl_FragColor = vec4(col,1.0);
}
