out vec4 FragColor;
in vec2 TexCoodrs;
uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoodrs);
}