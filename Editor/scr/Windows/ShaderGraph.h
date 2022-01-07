#pragma once
#include <glm.hpp>
#include <vector>

struct INode;

namespace ImNodes
{
	struct EditorContext;
}

namespace BalEditor
{
	class CShaderGraph
	{
	public:
		CShaderGraph();
		~CShaderGraph();
		CShaderGraph( const CShaderGraph& ) = delete;
		CShaderGraph( CShaderGraph&& ) = delete;
		CShaderGraph& operator=( const CShaderGraph& ) = delete;
		CShaderGraph& operator=( CShaderGraph&& ) = delete;
		void Draw();
		void ShowWindow();
	private:
		enum class EUiNodeType
		{
			//Shader
			VertexOutput,
			FragmentOutput,

			//Input
			//todo AmbientOcclusion,
			//todo Attribute,
			//todo Bevel,
			//todo CameraData,
			//todo Fresnel,
			//todo Geometry,
			//todo HairInfo,
			//todo LayerWeight,
			//todo LightPath,
			//todo ObjectInfo,
			//todo ParticleInfo,
			//RGB,
			//todo Tangent,
			//todo TextureCoordinate,
			//todo UVMap,
			//Value,
			//todo VertexColor,
			//todo VolumeInfo,
			//todo Wireframe,

			//Texture
			//BrickTexture,
			//CheckerTexture,
			//todo EnvironmentTexture,
			//GradientTexture,
			//todo IESTexture,
			//ImageTexture,
			//todo MagicTexture,
			//todo MusgraveTexture,
			//todo NoiseTexture,
			//todo PointDensity,
			//todo SkyTexture,
			//todo VoronoiTexture,
			//todo WaveTexture,
			//todo WhiteNoiseTexture,

			//Color
			BrightContrast,
			Gamma,
			HueSaturationValue,
			Invert,
			Mix,

			//Vector,
			Bump,
			Displacement,
			Mapping,
			//todo Normal,
			NormalMap,
			//todo VectorCurves,
			//todo VectorDisplacement,
			//todo VectorRotate,
			//todo VectorTransform,

			//Converter
			//todo Blackbody,
			Clamp,
			//todo ColorRamp,
			//todo CombineSeparate,
			//todo FloatCurve,
			//todo MapRange,
			Math,
			RGBtoBW,
			VectorMath,
			//todo Wavelength,
			MaxIndex
			//todo ScreenDoorTransparency
			//Atmospheric Fog Color
		};
		struct SLink
		{
			int id;
			int startNodeId, endNodeId;
			int startAttr, endAttr;
		};
		std::vector<INode*> m_nodes;
		std::vector<SLink> m_links;
		int m_currentId = 0;
		int m_currentAttributeId = 0;
		bool m_isVisible;

		std::vector<SLink>GetNeighbors( const int currentNode );
		void Evaluate();
		void AddNode( EUiNodeType type, const glm::vec2& position );
		static const char* ToString( EUiNodeType type );
	};
}
