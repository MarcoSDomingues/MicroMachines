#version 330

const int MaxLights = 7;

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
	vec4 l_pos;
	//spotlight attributes
	vec3 coneDirection;
	float spotCutOff;
	float spotExponent;
};

uniform LightProperties Lights[MaxLights];
uniform Materials mat;

uniform bool texMode;		//true se usar textura, false caso contrario, 
uniform sampler2D texmap1;
uniform sampler2D texmap2;

//esta info vem das normais interpoladas
in Data { 
	vec3 normal;
	vec3 eye;
	vec4 pos;
	vec2 tex_coord;
} DataIn;

out vec4 colorOut;

void main() {

	vec4 texel, texel1, texel2;

	bool disabled;
	vec4 colorLight = vec4(0.0);
	vec4 spec = vec4(0.0);
	//estes vectores tem de ser normalizados pois foram interpolados
	vec3 n = normalize(DataIn.normal);
	vec3 e = normalize(DataIn.eye);

	//loop over all the lights
	for (int light = 0; light < MaxLights; light++) {
		disabled = false;
		if (!Lights[light].isEnabled) 
			disabled = true;

		vec3 h = vec3(0.0);
		vec3 light_Dir = vec3(Lights[light].l_pos);

		if (Lights[light].isLocal) {
			light_Dir = light_Dir - vec3(DataIn.pos);
			light_Dir = normalize(light_Dir);
		
			if (Lights[light].isSpot) {
				//TODO
			}
		}
		else {
			//Directional Light
			vec3 light_Dir = vec3(Lights[light].l_pos);
		}
		if (disabled) 
		{
			if (colorLight == vec4(0.0))
				colorLight = mat.ambient;
			else
				colorLight += vec4(0.0);
		}
		else
		{
			float intensity = max(dot(n, light_Dir), 0.0);

			if (intensity > 0.0) {

				vec3 h = normalize(light_Dir + e);
				float intSpec = max(dot(h,n), 0.0);
				spec = mat.specular * pow(intSpec, mat.shininess);
			}

			if (texMode) {
				texel1 = texture(texmap1, DataIn.tex_coord);  // texel from stone.tga
				texel2 = texture(texmap2, DataIn.tex_coord);  // texel from checker.tga
				texel = texel1 * texel2;
				colorOut = max(intensity*texel + spec, 0.1*texel);
			} else {
				colorOut = max(intensity * mat.diffuse + spec, mat.ambient);
			}
			colorLight += max(intensity * mat.diffuse + spec, mat.ambient);
		}

	}
	colorOut += colorLight;
}
