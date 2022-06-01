#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
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
		void OpenShader( const std::filesystem::path& path ) const;
	private:
		enum class EUiNodeType
		{
			//Shader
			VertexOutput,
			FragmentOutput,

			////////blender nodes////////
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
			//todo RGB,
			//todo Tangent,
			//todo TextureCoordinate,
			//todo UVMap,
			//todo Value,
			//todo VertexColor,
			//todo VolumeInfo,
			//todo Wireframe,

			//Texture
			//todo BrickTexture,
			//todo CheckerTexture,
			//todo EnvironmentTexture,
			//todo GradientTexture,
			//todo IESTexture,
			ImageTexture,
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

			////////UE4 nodes////////
			//todo Atmospheric Fog Color
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
		bool m_wantsToSave;
		std::string m_currentName;

		std::vector<SLink> GetNeighbors( int currentNode );
		void Evaluate();
		void AddNode( EUiNodeType type, const glm::vec2& position );
		static const char* ToString( EUiNodeType type );
	};
}
