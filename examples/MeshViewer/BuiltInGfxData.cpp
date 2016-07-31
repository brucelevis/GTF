
#include "BuiltInGfxData.h"

const char * RENDER_MESH_VERTEX_SHADER = R"(
#version 330
precision highp float;
layout(location = 0) in  vec3 in_Position;
layout(location = 1) in  vec4 in_Color;
layout(location = 2) in  vec3 in_Normal;
layout(location = 3) in  vec3 in_Tangent;
layout(location = 4) in  vec3 in_Binormal;
layout(location = 5) in  vec2 in_TexCoord;

uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

out vec2 ex_TexCoord;
out vec4 ex_Color;
out vec3 ex_Normal;
out vec3 ex_Tangent;
out vec3 ex_Binormal;
out vec4 ex_EyeSpacePosition;
out mat4 ex_ModelViewMatrix;

void main(void)
{
    ex_TexCoord = in_TexCoord;
    ex_ModelViewMatrix = uViewMatrix * uModelMatrix;
    ex_Normal = normalize(mat3(ex_ModelViewMatrix) * in_Normal);
    ex_Tangent =  normalize(mat3(ex_ModelViewMatrix) * in_Tangent);
    ex_Binormal =  normalize(mat3(ex_ModelViewMatrix) * in_Binormal);
    ex_EyeSpacePosition = ex_ModelViewMatrix * vec4(in_Position.xyz, 1.0);
    gl_Position = uProjectionMatrix * ex_ModelViewMatrix * vec4(in_Position.xyz, 1.0);
    
}

)";

const char * RENDER_MESH_FRAGMENT_SHADER = R"(
#version 330
precision highp float;

in vec2 ex_TexCoord;
in vec4 ex_Color;
in vec3 ex_Normal;
in vec3 ex_Tangent;
in vec3 ex_Binormal;
in vec4 ex_EyeSpacePosition;
in mat4 ex_ModelViewMatrix;

out vec4 out_Color;

const vec3 lightSource = normalize(vec3(0.0, 0.5, 1.0));
const vec3 ambient = vec3(0.4);
const vec3 specular = vec3(0.05);
const vec3 diffuse = vec3(0.45);
const float shininess = 0.5;

uniform sampler2D uNormalMap;
uniform uint uWithNormalMap;
uniform sampler2D uColorMap;
uniform uint uWithColorMap;


void main (void) 
{  
	mat3 tangentToWorld = mat3(ex_Tangent.x, ex_Binormal.x, ex_Normal.x,
                               ex_Tangent.y, ex_Binormal.y, ex_Normal.y,
                               ex_Tangent.z, ex_Binormal.z, ex_Normal.z);
	
	vec3 v = ex_EyeSpacePosition.xyz;
	vec3 N;
	if(uWithNormalMap == 1u)
	{
		 vec3 normalSampled = texture(uNormalMap, ex_TexCoord.xy).rgb;
		 N = normalize(((normalSampled * 2.0) - 1.0) * tangentToWorld);
	}
	else
	{
		N = normalize(((vec3(0.5, 0.5, 1.0) * 2.0) - 1.0) * tangentToWorld);
	}
	
	vec3 baseColor = vec3(1.0, 1.0, 1.0);
	if(uWithColorMap == 1u)
	{
		 baseColor = texture(uColorMap, ex_TexCoord.xy).rgb;
	}

		vec3 L = normalize(lightSource - v);   
	vec3 E = normalize(-v);
	vec3 R = normalize(-reflect(L,N));  
 
	//calculate Diffuse Term:  
	vec3 Idiff = baseColor * diffuse * max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);     
   
	// calculate Specular Term:
	vec3 Ispec = specular * pow(max(dot(R,E),0.0),0.3*shininess);
	Ispec = clamp(Ispec, 0.0, 1.0); 
	
	// write Total Color:  
	vec3 colorAmbient = baseColor * ambient;
	out_Color = vec4(colorAmbient + Idiff, 1.0);     
}

)";

void BuiltInGfxData::init()
{
	renderMeshProgram.reset(gtf::GRHI->createProgram());
	std::unique_ptr<gtf::RHIShaderSource>vertexSource(gtf::GRHI->createSource());
	std::unique_ptr<gtf::RHIShaderSource>fragmentSource(gtf::GRHI->createSource());

	vertexSource->compileSource(RENDER_MESH_VERTEX_SHADER, gtf::ERHIShaderType::RHI_VERTEX_SHADER);
	fragmentSource->compileSource(RENDER_MESH_FRAGMENT_SHADER, gtf::ERHIShaderType::RHI_FRAGMENT_SHADER);

	gtf::RHIShaderSource* sources[] = { vertexSource.get() , fragmentSource.get() };
	renderMeshProgram->link(sources, 2);
}