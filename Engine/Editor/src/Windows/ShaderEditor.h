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
        void ShowWindow( const SFile& shader );

    private:
        enum class ui_node_type
        {
            //Shader
            shader_node,
            vertex_output,
            //todo geometry_output,
            //todo tesselation_control,
            //todo tesselation_evaluation,
            fragment_output,
            fragment_unlit_output,
            
            //Input
            camera_data,
            //todo bevel,
            //todo fresnel,
            //todo rgb,
            //todo value,
            //todo normal,
            //todo tangent,
            //todo uv_map,
            //todo vertex_color,
            //todo wireframe,
            
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
            //todo replace,
            //todo white_balance,
            //todo color_mask,
            //todo colorspace_conversion,
            
            //Vector,
            bump,
            displacement,
            mapping,
            normal_map,
            //todo normal_blend,
            //todo vector_curves,
            //todo vector_displacement,
            //todo vector_rotate,
            //todo vector_transform,
            
            //Converter
            //todo blackbody,
            clamp,
            combine_xyz,
            combine_rgb,
            separate_xyz,
            separate_rgb,
            //todo float_curve,
            //todo map_range,
            math,
            rgb_to_bw,
            vector_math,
            //todo wavelength,
            //todo dither,
            
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
        
        std::unordered_map<ui_node_type, CShaderNode*(*)(CShaderGraph&)> m_nodeCreationMap;
        static std::unordered_map<ui_node_type, const char*> m_nodeNameMap;
        
        void EnterFileName();
        void Evaluate();
        void AddNode( ui_node_type type, const glm::vec2& position );
        void AddLink( SLink& link );
    
        bool DrawBinding( SShaderBinding& binding );
        static const char* ToString( ui_node_type type );
    };
}
