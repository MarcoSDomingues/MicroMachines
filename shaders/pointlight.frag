#version 330

const int MaxLights = 9;

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
uniform bool lightsOff;
uniform int depthFog;

//esta info vem das normais interpoladas
in Data { 
	vec3 normal;
	vec3 eye;
	vec4 pos;
	vec2 tex_coord;
} DataIn;

out vec4 colorOut;

const vec3 fogColor = vec3(0.5, 0.5,0.5);
const float FogDensity = 0.025;

void main() {
	int disabled = 0;
	vec4 spec = vec4(0.0);
	vec4 scatteredLight = vec4(0.0);
	vec4 reflectedLight = vec4(0.0);
	//estes vectores tem de ser normalizados pois foram interpolados
	vec3 n = normalize(DataIn.normal);
	vec3 e = normalize(DataIn.eye);

	//distance
	float dist = 0;
	float fogFactor = 0;
 
	//compute distance used in fog equations
	if(depthFog == 0)//select plane based vs range based
	{
	  //plane based
	  dist = abs(DataIn.pos.z);
	  //dist = (gl_FragCoord.z / gl_FragCoord.w);
	}
	else
	{
	   //range based
	   dist = length(DataIn.pos);
	}
 

	//loop over all the lights
	for (int light = 0; light < MaxLights; light++) {
		if (!Lights[light].isEnabled) 
		{	
			disabled++;
			continue;
		}

		float attenuation = 1.0;
		vec3 h;
		vec3 light_Dir = vec3(Lights[light].l_pos);

		if (Lights[light].isLocal) {
			light_Dir = light_Dir - vec3(DataIn.pos);
			float light_Distance = length(light_Dir);
			light_Dir = light_Dir / light_Distance;
		
			attenuation = 1.0 / (light_Distance * light_Distance);
			
			if (Lights[light].isSpot) {
				float spotCos = dot(light_Dir, -Lights[light].coneDirection);
				if (spotCos < Lights[light].spotCutOff)
					attenuation = 0.0;
				else
					attenuation *= pow(spotCos, Lights[light].spotExponent);
			}

		}

		else {
			//Directional Light
			light_Dir = vec3(Lights[light].l_pos);
		}

		h = normalize(light_Dir + e);
		float intDiff = max(dot(n, light_Dir), 0.0);
		float intSpec = max(dot(h,n), 0.0);

		if (intDiff == 0.0) 
			intSpec = 0.0;
		else 
			spec = mat.specular * pow(intSpec, mat.shininess);
		
		//Acumulate all the lights effect
		scatteredLight += mat.diffuse * intDiff * attenuation;
		reflectedLight += spec * attenuation;
		
	}
	vec4 texel, texel1, texel2;
	if (texMode) {
			texel1 = texture(texmap1, DataIn.tex_coord);  // texel from stone.tga
			texel2 = texture(texmap2, DataIn.tex_coord);  // texel from checker.tga
			texel = texel1 * texel2;
			if(texel.a == 0.0) discard;
	}



	if (texMode && !lightsOff)
		colorOut = max(mat.ambient*texel, scatteredLight*texel + reflectedLight);
	else if (texMode && lightsOff)
		colorOut = texel;
	else if (lightsOff)
		colorOut = mat.ambient;
	else
		colorOut = max(mat.ambient, scatteredLight + reflectedLight);

	if (!lightsOff) {
		fogFactor = 1.0 /exp(dist * (dist/2) * FogDensity);
		fogFactor = clamp( fogFactor, 0.0, 1.0 );
 
		colorOut = vec4(mix(fogColor, vec3(colorOut), fogFactor), colorOut.w);
	}

	

}
