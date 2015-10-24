#version 330

const int MaxLights = 1;

struct Materials {
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	vec4 emissive;
	float shininess;
	int texCount;
};

struct LightProperties {
	bool isEnabled;
	bool isLocal;           //true for a pointlight or a spotlight
	bool isSpot;
	//spotlight attributes
	vec3 coneDirection;
	float spotCutOff;
	float spotExponent;
};

uniform LightProperties Lights[MaxLights];

uniform Materials mat;

uniform vec4 l_pos;

//esta info vem das normais interpoladas
in Data { 
	vec3 normal;
	vec3 eye;
	vec4 pos;
} DataIn;

out vec4 colorOut;

void main() {

	bool enabled = true;
	vec4 spec = vec4(0.0);
	//estes vectores tem de ser normalizados pois foram interpolados
	vec3 n = normalize(DataIn.normal);
	vec3 e = normalize(DataIn.eye);

	//loop over all the lights
	for (int light = 0; light < MaxLights; light++) {
		if (!Lights[light].isEnabled) 
			enabled = false;

		vec3 h = vec3(0.0);
		vec3 light_Dir = vec3(l_pos);

		if (Lights[light].isLocal) {
			light_Dir = light_Dir - vec3(DataIn.pos);
			light_Dir = normalize(light_Dir);
		
			if (Lights[light].isSpot) {
				//TODO
			}

			h = normalize(light_Dir + e);
		}
		else {
			//Directional Light
			light_Dir = vec3(l_pos);
		}

		float intensity;
		if (!enabled)
			intensity = 0.0f;
		else
			intensity = max(dot(n, light_Dir), 0.0);

		if (intensity > 0.0) {

			vec3 h = normalize(light_Dir + e);
			float intSpec = max(dot(h,n), 0.0);
			spec = mat.specular * pow(intSpec, mat.shininess);
		}

		colorOut = max(intensity * mat.diffuse + spec, mat.ambient);
	}

}
