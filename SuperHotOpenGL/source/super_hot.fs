#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 position;
in vec3 new_normal;

uniform sampler2D texture_diffuse1;
uniform vec4 mat_specular;
uniform float mat_s;
uniform vec3 cam_position;

//Estructura de la luz
struct LightSource{
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    float constantAttenuation, linearAttenuation, quadraticAttenuation;
};

LightSource light0 = LightSource(
     vec4(20.0, 20.0, 20.0, 1.0), //Posicion
     vec4(1.0, 1.0, 1.0, 1.0), //Difusividad
     vec4(1.0, 1.0, 1.0, 1.0), //Especularidad
     0.0, 0.0, 0.0
);

LightSource light1 = LightSource(
     vec4(-20.0, 20.0, -20.0, 1.0), //Posicion
     vec4(1.0, 1.0, 1.0, 1.0), //Difusividad
     vec4(1.0, 1.0, 1.0, 1.0), //Especularidad
     0.0, 0.0, 0.0
);
LightSource light2 = LightSource(
     vec4(20.0, 20.0, -20.0, 1.0), //Posicion
     vec4(1.0, 1.0, 1.0, 1.0), //Difusividad
     vec4(1.0, 1.0, 1.0, 1.0), //Especularidad
     0.0, 0.0, 0.0
);
LightSource light3 = LightSource(
     vec4(-0.247253, 0.033664, -0.012056, 1.0), //Posicion
     vec4(1.0, 1.0, 1.0, 1.0), //Difusividad
     vec4(1.0, 1.0, 1.0, 1.0), //Especularidad
     0.0, 0.0, 0.0
);

vec4 scene_ambient = vec4(0.3, 0.3, 0.3, 1.0);

void main()
{    
    vec3 N = new_normal;
	vec3 V = normalize(cam_position - vec3(position));	
	 
	vec3 L0 = vec3(vec3(light0.position) - vec3(position));
	float d0 = length(L0);
    L0 = normalize(L0);

	vec3 L1 = vec3(vec3(light1.position) - vec3(position));
	float d1 = length(L1);
    L1 = normalize(L1);

    vec3 L2 = vec3(vec3(light2.position) - vec3(position));
	float d2 = length(L2);
    L2 = normalize(L2);

    vec3 L3 = vec3(vec3(light3.position) - vec3(position));
	float d3 = length(L3);
    L3 = normalize(L3);
					

	float atenuacion0 = 1.0/(light0.constantAttenuation + d0*light0.linearAttenuation + d0*d0*light0.quadraticAttenuation);
	float atenuacion1 = 1.0/(light1.constantAttenuation + d1*light1.linearAttenuation + d1*d1*light1.quadraticAttenuation);
	float atenuacion2 = 1.0/(light2.constantAttenuation + d2*light2.linearAttenuation + d2*d2*light2.quadraticAttenuation);
	float atenuacion3 = 1.0/(light3.constantAttenuation + d3*light3.linearAttenuation + d3*d3*light3.quadraticAttenuation);      
         

	vec4 texture_color = texture(texture_diffuse1,TexCoords);	
	vec3 ambient_light = vec3(scene_ambient) * vec3(texture_color);

    vec3 diffuse_reflection0 =  vec3(light0.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L0)); //* attenuation
    vec3 diffuse_reflection1 =  vec3(light1.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L1)); //* attenuation
    vec3 diffuse_reflection2 =  vec3(light2.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L2)); //* attenuation
    vec3 diffuse_reflection3 =  vec3(light3.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L3)); //* attenuation
                  
				  
	//vec3 specular0 = atenuacion0 * vec3(light0.specular) * vec3(1.0,1.0,1.0) * pow(max(0.0,dot(reflect(-L0,N),V)),light0.s);
	//vec3 specular1 = atenuacion1 * vec3(light1.specular) * vec3(1.0,1.0,1.0) * pow(max(0.0,dot(reflect(-L1,N),V)),light1.s);
	//vec3 specular2 = atenuacion2 * vec3(light2.specular) * vec3(1.0,1.0,1.0) * pow(max(0.0,dot(reflect(-L2,N),V)),light2.s);
	//vec3 specular3 = atenuacion3 * vec3(light3.specular) * vec3(1.0,1.0,1.0) * pow(max(0.0,dot(reflect(-L3,N),V)),light3.s);

    vec3 totalLight = ambient_light + diffuse_reflection0 + diffuse_reflection1 + diffuse_reflection2 + diffuse_reflection3 ;
    FragColor = vec4(totalLight, 1.0);
}