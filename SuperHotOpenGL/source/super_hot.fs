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
    vec3 totalLight = vec3(scene_ambient) * vec3(0.3, 0.3, 0.3);

    vec3 V = normalize(cam_position - vec3(position));
    vec3 L = vec3(vec3(light0.position) - vec3(position));
    float distance = length(L);

     L = normalize(L);

     vec3 L1 = vec3(vec3(light1.position) - vec3(position));
     L1 = normalize(L1);
     vec3 L2 = vec3(vec3(light2.position) - vec3(position));
     L2 = normalize(L2);
     vec3 L3 = vec3(vec3(light3.position) - vec3(position));
     L3 = normalize(L3);

    

   //  float attenuation = 1.0/(light0.constantAttenuation +
   //                        distance*light0.linearAttenuation+
    //                      distance*distance*light0.quadraticAttenuation);

    vec3 diffuse_reflection =  vec3(light0.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L)); //* attenuation
    vec3 diffuse_reflection1 =  vec3(light1.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L1)); //* attenuation
     vec3 diffuse_reflection2 =  vec3(light2.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L2)); //* attenuation
      vec3 diffuse_reflection3 =  vec3(light3.diffuse) * vec3(texture(texture_diffuse1, TexCoords)) * max(0.0, dot(N,L3)); //* attenuation
     
   
     
     

    totalLight = totalLight + diffuse_reflection1 +diffuse_reflection + diffuse_reflection2 +diffuse_reflection3;
    //totalLight = totalLight+diffuse_reflection;
    FragColor = vec4(totalLight, 1.0);
}