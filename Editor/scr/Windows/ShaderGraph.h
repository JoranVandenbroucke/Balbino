#pragma once
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <FileParcer.h>

struct INode;

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
        void ShowWindow( const SFile& shader = {} );
    private:
        enum class EUiNodeType
        {
            //Shader
            ShaderNode,
            VertexOutput,
            GeometryOutput,
            TesselationControl,
            TesselationEvaluation,
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
            NormalMap,
            //todo VectorCurves,
            //todo VectorDisplacement,
            //todo VectorRotate,
            //todo VectorTransform,
            
            //Converter
            //todo Blackbody,
            Clamp,
            //todo CombineSeparate,
            //todo FloatCurve,
            //todo MapRange,
            Math,
            RGBtoBW,
            VectorMath,
            //todo Wavelength,
            //Input
            ViewDirection,
            ViewZ,
            ViewDepth,
            
            MaxIndex
            //todo ScreenDoorTransparency
            
            
        };
        
        struct SLink
        {
            int id;
            int startNodeId, endNodeId;
            int startAttr, endAttr;
        };
        
        std::vector<std::pair<int, INode*>> m_nodes;
        std::vector<SLink>                  m_links;
        std::vector<std::string>            m_allNodeNames;
        
        int   m_currentId          = 0;
        int   m_currentAttributeId = 0;
        bool  m_isVisible;
        bool  m_wantsToSave;
        SFile m_currentShaderFile;
        
        std::vector<SLink> GetNeighbors( int currentNode );
        bool HasCycleStartingFrom( const INode* pStartNode);
        void Evaluate();
        void AddNode( EUiNodeType type, const glm::vec2& position, int id = -1 );
        void AddLink( SLink& link );
        void LoadEditorFromData( const SFile& shader );
        void LoadDefaultEditor();
        std::string SaveEditor();
        static const char* ToString( EUiNodeType type );
    };
}
