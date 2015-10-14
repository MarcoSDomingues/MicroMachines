#version 330

uniform mat4 m_pvm; //clip coordinates
uniform mat4 m_viewModel; //view coordinates
uniform mat3 m_normal; //transposta da inversa da matriz viewModel

uniform vec4 l_pos;

in vec4 position;
in vec4 normal;    //por causa do gerador de geometria

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataOut;

void main () {

	vec4 pos = m_viewModel * position;

	DataOut.normal = normalize(m_normal * normal.xyz); //tem de ser normalizado para as normais interpoladas terem a direccao correcta
	//mas as normais interpoladas podem nao ter tamanho 1, por isso voltam a ser normalizadas no fragment shader
	DataOut.lightDir = vec3(l_pos - pos);
	DataOut.eye = vec3(-pos);

	gl_Position = m_pvm * position; //posicao do vertice em clip coordinates
}
