#include "pch.h"
#include "FragmentOutput.h"

#include <string>

#include "Attribute.h"
#include "imgui.h"
#include "imnodes.h"

CFragmentOutputNode::CFragmentOutputNode( int id, int& attributeStartId )
	: INode{ id, attributeStartId }
	, m_connections{}
{
	attributeStartId += m_connectionSize;
}

void CFragmentOutputNode::Draw()
{
	ImGui::PushItemWidth( 200 );
	ImNodes::BeginNode( m_id );

	ImNodes::BeginNodeTitleBar();
	ImGui::TextUnformatted( "Fragment Output" );
	ImNodes::EndNodeTitleBar();

	/*
	 *  1.  color baseColor .82 .67 .16
	 *  2.  float metallic 0 1 0
	 *  3.  float subsurface 0 1 0
	 *  4.  float specular 0 1 .5
	 *  5.  float roughness 0 1 .5
	 *  6.  float specularTint 0 1 0
	 *  7.  float anisotropic 0 1 0
	 *  8.  float sheen 0 1 0
	 *  9.  float sheenTint 0 1 .5
	 *  10. float clearcoat 0 1 0
	 *  11. float clearcoatGloss 0 1 1
	 *  12. float alpha 0 1 1
	 *  13. vector normal 0 1 0
	 */

	DrawInputColorAttribute( m_color, m_attributeStartId, m_connections[0], "Diffuse Color" );
	DrawInputFloatAttribute( m_metallic, m_attributeStartId + 1, m_connections[1], "Metallic", 0, 1 );
	DrawInputFloatAttribute( m_subsurface, m_attributeStartId + 2, m_connections[2], "Subsurface", 0, 1 );
	DrawInputFloatAttribute( m_specular, m_attributeStartId + 3, m_connections[3], "Specular", 0, 1 );
	DrawInputFloatAttribute( m_roughness, m_attributeStartId + 4, m_connections[4], "Roughness", 0, 1 );
	DrawInputFloatAttribute( m_specularTint, m_attributeStartId + 5, m_connections[5], "Specular tint", 0, 1 );
	DrawInputFloatAttribute( m_anisotropic, m_attributeStartId + 6, m_connections[6], "Anisotropic", 0, 1 );
	DrawInputFloatAttribute( m_sheen, m_attributeStartId + 7, m_connections[7], "Sheen", 0, 1 );
	DrawInputFloatAttribute( m_sheenTint, m_attributeStartId + 8, m_connections[8], "Sheen tint", 0, 1 );
	DrawInputFloatAttribute( m_clearcoat, m_attributeStartId + 9, m_connections[9], "Clearcoat", 0, 1 );
	DrawInputFloatAttribute( m_clearcoatGloss, m_attributeStartId + 10, m_connections[10], "Clearcoat tint", 0, 1 );
	DrawInputFloatAttribute( m_alpha, m_attributeStartId + 11, m_connections[11], "Alpha", 0, 1 );
	DrawInputVectorAttribute( m_normal, m_attributeStartId + 12, true, "Normal" );

	ImNodes::EndNode();
}

void CFragmentOutputNode::Attach( int endAttr )
{
	m_connections[endAttr - m_attributeStartId] = true;
}

void CFragmentOutputNode::Detach( int endAttr )
{
	m_connections[endAttr - m_attributeStartId] = false;
}

void CFragmentOutputNode::Evaluate( std::vector<INode*>::iterator& begin, const std::vector<INode*>::iterator& end, std::ostream& output, EAttributeType attributeType )
{
	( void ) begin;
	( void ) end;
	( void ) output;
	( void ) attributeType;
	output << R"(#version 450
#include "default.glsl"

layout(set=1, binding=4) uniform samplerCube gRadEnvironment;
layout(set=1, binding=5) uniform samplerCube gIrradEnvironment;
layout(set=1, binding=6) uniform sampler2D gBRDFLUP;

layout( location = 0) in vec4 fragColor;
layout( location = 1 ) in vec2 fragTexCoord;
layout( location = 2 ) in vec3 fragNormal;
layout( location = 3 ) in vec3 fragTangent;
layout( location = 4 ) in vec3 fragBinormal;
layout( location = 5 ) in vec4 fragWorldPosition;

layout (location = 0) out vec4 outFragcolor;

struct Light {
	vec4 position;
	vec3 color;
	float radius;
};

#define LIGHT_COUNT 6
layout (set = 0, binding = 1) uniform UBO 
{
	Light lights[LIGHT_COUNT];
	vec4 viewPos;
	int displayDebugTarget;
} ubo;

float sqr(float x) { return x*x; }

float SchlickFresnel(float u)
{
    float m = clamp(1-u, 0, 1);
    float m2 = m * m;
    return m2 * m2 * m; // pow(m,5)
}
vec3 SchlickFresnelRough(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float GTR1(float NdotH, float a)
{
    if (a >= 1) return 1 / BALBINO_PI;
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return (a2 - 1) / (BALBINO_PI * log(a2) * t);
}

float GTR2(float NdotH, float a)
{
    float a2 = a * a;
    float t = 1 + (a2 - 1) * NdotH * NdotH;
    return a2 / (BALBINO_PI * t * t);
}

float GTR2_aniso(float NdotH, float HdotX, float HdotY, float ax, float ay)
{
    return 1 / (BALBINO_PI * ax * ay * sqr( sqr(HdotX / ax) + sqr(HdotY / ay) + NdotH * NdotH ));
}

float smithG_GGX(float NdotV, float alphaG)
{
    float a = alphaG * alphaG;
    float b = NdotV * NdotV;
    return 1 / (NdotV + sqrt(a + b - a * b));
}

float smithG_GGX_aniso(float NdotV, float VdotX, float VdotY, float ax, float ay)
{
    return 1 / (NdotV + sqrt( sqr(VdotX * ax) + sqr(VdotY * ay) + sqr(NdotV) ));
}

vec3 mon2lin(vec3 x)
{
    return vec3(pow(x[0], 2.2), pow(x[1], 2.2), pow(x[2], 2.2));
}

void main(void)
{
)";
	if ( m_connections[0] )
	{
		output << "\tvec3 baseColor = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Color );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tvec3 baseColor = vec3(";
		output << std::to_string(m_color[0]) << ", ";
		output << std::to_string(m_color[1]) << ", ";
		output << std::to_string(m_color[2]);
		output << " ); " << std::endl;
	}

	if ( m_connections[1] )
	{
		output << "\tfloat metallic = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat metallic = ";
		output << std::to_string( m_metallic );
		output << ";" << std::endl;
	}

	if ( m_connections[2] )
	{
		output << "\tfloat subsurface = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat subsurface = ";
		output << std::to_string( m_subsurface );
		output << ";" << std::endl;
	}

	if ( m_connections[3] )
	{
		output << "\tfloat specular = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat specular = ";
		output << std::to_string( m_specular );
		output << ";" << std::endl;
	}

	if ( m_connections[4] )
	{
		output << "\tfloat roughness = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat roughness = ";
		output << std::to_string( m_roughness );
		output << ";" << std::endl;
	}

	if ( m_connections[5] )
	{
		output << "\tfloat specularTint = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat specularTint = ";
		output << std::to_string( m_specularTint );
		output << ";" << std::endl;
	}

	if ( m_connections[6] )
	{
		output << "\tfloat anisotropic = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat anisotropic = ";
		output << std::to_string( m_anisotropic );
		output << ";" << std::endl;
	}

	if ( m_connections[7] )
	{
		output << "\tfloat sheen = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat sheen = ";
		output << std::to_string( m_sheen );
		output << ";" << std::endl;
	}

	if ( m_connections[8] )
	{
		output << "\tfloat sheenTint = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "float sheenTint = ";
		output << std::to_string( m_sheenTint );
		output << ";" << std::endl;
	}

	if ( m_connections[9] )
	{
		output << "\tfloat clearcoat = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat clearcoat = ";
		output << std::to_string( m_clearcoat );
		output << ";" << std::endl;
	}

	if ( m_connections[10] )
	{
		output << "\tfloat clearcoatGloss = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "\tfloat clearcoatGloss = " ;
		output << std::to_string( m_clearcoatGloss );
		output << ";" << std::endl;
	}

	if ( m_connections[11] )
	{
		output << "\tfloat alpha = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Float );
		output << ";" << std::endl;
	}
	else
	{
		output << "float alpha = ";
		output << std::to_string( m_alpha );
		output << ";" << std::endl;
	}

	if ( m_connections[12] )
	{
		output << "\tvec3 normal = ";
		( *( begin++ ) )->Evaluate( begin, end, output, EAttributeType::Vector );
		output << ";" << std::endl;
	}
	else
		output << "vec3 normal = normalize(fragNormal);" << std::endl << "\tvec3 tangent = normalize( fragTangent );" << std::endl << "\tvec3 bitangent = normalize( fragTangent );";

	output << R"(
	vec3 view = normalize(ubo.viewPos.xyz - fragWorldPosition.xyz);
	vec3 directLight = vec3(0, 0, 0);
	float NdotV = dot(normal,view);
	float FV = SchlickFresnel(NdotV);
	
	vec3 Cdlin = mon2lin(baseColor);
	float Cdlum = .3 * Cdlin[0] + .6 * Cdlin[1]  + .1 * Cdlin[2]; // luminance approx.
	vec3 Ctint = Cdlum > 0 ? Cdlin/Cdlum : vec3(1); // normalize lum. to isolate hue+sat
	vec3 Cspec0 = mix(specular * .08 * mix(vec3(1), Ctint, specularTint), Cdlin, metallic);
	vec3 Csheen = mix(vec3(1), Ctint, sheenTint);
	
	// specular
	float aspect = sqrt(1 - anisotropic * .9);
	float ax = max(.001, sqr(roughness )/ aspect);
	float ay = max(.001, sqr(roughness) * aspect);
	
	for(int i = 0; i < LIGHT_COUNT; ++i)
	{
		vec3 lightDir = -normalize(ubo.lights[i].position.xyz - fragWorldPosition.xyz);
		float NdotL = dot(normal,lightDir);
		if (NdotL < 0 || NdotV < 0) 
		{
			continue;
		}

		vec3 H = normalize(lightDir + view);
		float NdotH = dot(normal, H);
		float LdotH = dot(lightDir, H);

		// Diffuse fresnel - go from 1 at normal incidence to .5 at grazing
		// and mix in diffuse retro-reflection based on roughness
		float FL = SchlickFresnel(NdotL);
		float Fd90 = 0.5 + 2 * LdotH * LdotH * roughness;
		float Fd = mix(1.0, Fd90, FL) * mix(1.0, Fd90, FV);

		// Based on Hanrahan-Krueger brdf approximation of isotropic bssrdf
		// 1.25 scale is used to (roughly) preserve albedo
		// Fss90 used to "flatten" retroreflection based on roughness
		float Fss90 = LdotH * LdotH * roughness;
		float Fss = mix(1.0, Fss90, FL) * mix(1.0, Fss90, FV);
		float ss = 1.25 * (Fss * (1 / (NdotL + NdotV) - .5) + .5);

		// specular
		float Ds = GTR2_aniso(NdotH, dot(H, tangent), dot(H, bitangent), ax, ay);
		float FH = SchlickFresnel(LdotH);
		vec3 Fs = mix(Cspec0, vec3(1), FH);
		float Gs;
		Gs  = smithG_GGX_aniso(NdotL, dot(lightDir, tangent), dot(lightDir, bitangent), ax, ay);
		Gs *= smithG_GGX_aniso(NdotV, dot(view, tangent), dot(view, bitangent), ax, ay);

		// sheen
		vec3 Fsheen = FH * sheen * Csheen;

		// clearcoat (ior = 1.5 -> F0 = 0.04)
		float Dr = GTR1(NdotH, mix(.1,.001,clearcoatGloss));
		float Fr = mix(.04, 1.0, FH);
		float Gr = smithG_GGX(NdotL, .25) * smithG_GGX(NdotV, .25);
		float lightStrength = dot(ubo.lights[i].position.xyz - fragWorldPosition.xyz, ubo.lights[i].position.xyz - fragWorldPosition.xyz);
		directLight += (((1 / BALBINO_PI) * mix(Fd, ss, subsurface) * Cdlin + Fsheen)
		    * (1-metallic)
		    + Gs * Fs * Ds + .25 * clearcoat * Gr * Fr * Dr) * ubo.lights[i].color * lightStrength;
	}

	vec3 ambientLighting = vec3(0, 0, 0);
	{
		vec3 F0 = mix(vec3(0.04f), baseColor, metallic);

		//ambient lighting (we now use IBL as the ambient term)
		vec3 kS = SchlickFresnelRough(NdotV, F0, roughness); 
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;
	
		vec3 irradiance = GetTextureCUBE(gIrradEnvironment, normal).xyz;
		vec3 diffuse = irradiance * baseColor * kD;

		// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
		vec3 reflectVec = reflect(-view, normal);
		vec3 prefilteredColor = GetTextureCUBELevel(gRadEnvironment, reflectVec, roughness * 4).xyz;
		vec2 brdf = texture(gBRDFLUP, vec2(NdotV, roughness)).rg;
		vec3 specularColor = prefilteredColor * (kS * brdf.x + brdf.y);
		vec3 ambient = diffuse + specularColor;
	
		ambientLighting = ambient;
	}
    outFragcolor = vec4(directLight /*+ ambientLighting*/, alpha);
})";
}

bool CFragmentOutputNode::HasFreeAttachment( int endAttr ) const
{
	if ( endAttr >= m_attributeStartId && endAttr < m_attributeStartId + m_connectionSize && !m_connections[endAttr - m_attributeStartId] )
		return true;
	return false;
}

int CFragmentOutputNode::GetId() const
{
	return m_id;
}

std::vector<int> CFragmentOutputNode::GetInputs() const
{
	std::vector<int> inputs( m_connectionSize, m_attributeStartId );
	for ( int i = 0; i < m_connectionSize; ++i )
	{
		inputs[i] += i;
	}
	return inputs;
}
