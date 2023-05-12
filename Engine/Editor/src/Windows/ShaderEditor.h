#pragma once
#include <FileParcer.h>
#include <filesystem>
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "../Graphs/Shaders/ShaderGraph.h"

struct Node;

namespace FawnForge
{
    class CShaderEditor
    {
    public:
        CShaderEditor();
        ~CShaderEditor();
        CShaderEditor( const CShaderEditor& ) = delete;
        CShaderEditor( CShaderEditor&& ) = delete;
        CShaderEditor& operator=( const CShaderEditor& ) = delete;
        CShaderEditor& operator=( CShaderEditor&& ) = delete;

        void Draw();
        void ShowWindow( const SFile& shader = {} );

    private:
        enum class ui_node_type
        {
            //Shader
            shader_node,
            vertex_output,
//            GeometryOutput,
//            TesselationControl,
//            TesselationEvaluation,
            fragment_output,
//            UnlitFragmentOutput,
            
            ////////blender nodes////////
            //todo Input
            //todo AmbientOcclusion,
            //todo Attribute,
            //todo Bevel,
            //todo ViewDirection,
            //todo ViewZ,
            //todo ViewDepth,
            //todo Fresnel,
            //todo HairInfo,
            //todo ObjectInfo,
            //todo ParticleInfo,
            //todo RGB,
            //todo Tangent,
            //todo UVMap,
            //todo Value,
            //todo VertexColor,
            //todo Wireframe,
            
            //Texture
            //todo BrickTexture,
            //todo CheckerTexture,
            //todo GradientTexture,
            image_texture,
            //todo MagicTexture,
            //todo MusgraveTexture,
            //todo NoiseTexture,
            //todo PointDensity,
            //todo SkyTexture,
            //todo VoronoiTexture,
            //todo WaveTexture,
            //todo WhiteNoiseTexture,
            
            //Color
            bright_contrast,
            gamma,
            hue_saturation_value,
            invert,
            mix,
            //todo Replace,
            //todo WhiteBalance,
            //todo ColorMask,
            
            //Vector,
            bump,
            displacement,
            mapping,
            normal_map,
            //todo NormalBlend,
            //todo VectorCurves,
            //todo VectorDisplacement,
            //todo VectorRotate,
            //todo VectorTransform,
            
            //Converter
            //todo Blackbody,
            clamp,
            //todo CombineXYZ,
            //todo CombineRGB,
            //todo SeparateXYZ,
            //todo SeparateRGB,
            //todo FloatCurve,
            //todo MapRange,
            math,
            rgb_to_bw,
            vector_math,
            //todo Wavelength,
            //todo Dither,
            //todo ColorspaceConversion,
            
            max
        };
        
        struct SLink
        {
            int id;
            int endNodeId;
            int startNodeId;
            int endAttr;
            int startAttr;
        };
        
        std::vector<SLink>                 m_links;
        std::vector<std::string>           m_allNodeNames;
        
        bool m_isWindowVisible{};
        bool m_wantsToSave{};
        int   m_cullMode{};
        int   m_linkId{};
        SFile m_currentShaderFile;
        CShaderGraph m_shaderGraph;
        
        std::vector<SLink> GetNeighbors( int currentNode );
        void EnterFileName();
        void Evaluate();
        void AddNode( ui_node_type type, const glm::vec2& position );
        void AddLink( SLink& link );
    
        bool DrawBinding( SShaderBinding& binding );
        static const char* ToString( ui_node_type type );
    };
}
