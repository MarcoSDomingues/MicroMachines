#version 330

out vec4 colorOut;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

uniform Materials mat;

in Data { //esta info vem das normais interpoladas
	vec3 normal;
	vec3 eye;
	vec3 lightDir;
} DataIn;

void main() {

	vec4 spec = vec4(0.0);

	//estes vectores tem de ser normalizados pois foram interpolados
	vec3 n = normalize(DataIn.normal);
	vec3 l = normalize(DataIn.lightDir);
	vec3 e = normalize(DataIn.eye);
	//half-vector = l + e

	float intensity = max(dot(n,l), 0.0);


	if (intensity > 0.0) {

		vec3 h = normalize(l + e);
		float intSpec = max(dot(h,n), 0.0);
		spec = mat.specular * pow(intSpec, mat.shininess);
	}

	colorOut = max(intensity * mat.diffuse + spec, mat.ambient);
}
