#pragma once

#include <filesystem>

#include <string>
#include <vector>

#include "../Graphs/Shaders/ShaderGraph.h"

#include "../Graphs/Shaders/Nodes/Colour/BrightContrastNode.h"
#include "../Graphs/Shaders/Nodes/Colour/ColorInvertNode.h"
#include "../Graphs/Shaders/Nodes/Colour/ColorMixNode.h"
#include "../Graphs/Shaders/Nodes/Colour/GammaNode.h"
#include "../Graphs/Shaders/Nodes/Colour/HueSaturationValueNode.h"
#include "../Graphs/Shaders/Nodes/Converter/ClampNode.h"
#include "../Graphs/Shaders/Nodes/Converter/CombineRGB.h"
#include "../Graphs/Shaders/Nodes/Converter/CombineXYZ.h"
#include "../Graphs/Shaders/Nodes/Converter/MathNode.h"
#include "../Graphs/Shaders/Nodes/Converter/RGBtoBWNode.h"
#include "../Graphs/Shaders/Nodes/Converter/SeparateRGB.h"
#include "../Graphs/Shaders/Nodes/Converter/SeparateXYZ.h"
#include "../Graphs/Shaders/Nodes/Converter/VectorMathNode.h"
#include "../Graphs/Shaders/Nodes/Input/CameraData.h"
#include "../Graphs/Shaders/Nodes/Shaders/FragmentLitNode.h"
#include "../Graphs/Shaders/Nodes/Shaders/FragmentUnlitNode.h"
#include "../Graphs/Shaders/Nodes/Shaders/OutputNode.h"
#include "../Graphs/Shaders/Nodes/Shaders/VertexOutput.h"
#include "../Graphs/Shaders/Nodes/Textures/ImageTextureNode.h"
#include "../Graphs/Shaders/Nodes/Vector/BumpNode.h"
#include "../Graphs/Shaders/Nodes/Vector/Displacement.h"
#include "../Graphs/Shaders/Nodes/Vector/MappingNode.h"
#include "../Graphs/Shaders/Nodes/Vector/NormalMapNode.h"

namespace FawnForge
{
    class CShaderEditor final
    {
    public:
        CShaderEditor();
        ~CShaderEditor();
        CShaderEditor( const CShaderEditor& )            = delete;
        CShaderEditor( CShaderEditor&& )                 = delete;
        CShaderEditor& operator=( const CShaderEditor& ) = delete;
        CShaderEditor& operator=( CShaderEditor&& )      = delete;

        void Draw();
        void ShowWindow( const SFile& shader );

    private:
        enum class ui_node_type
        {
            //Shader
            shader_node,
            vertex_output,
            //todo:geometry_output,
            //todo:tesselation_control,
            //todo:tesselation_evaluation,
            fragment_output,
            fragment_unlit_output,

            //input
            camera_data,
            //todo:bevel,
            //todo:fresnel,
            //todo:rgb,
            //todo:mix_mode_value,
            //todo:normal,
            //todo:tangent,
            //todo:uv_map,
            //todo:vertex_color,
            //todo:wireframe,

            //Texture
            //todo:BrickTexture,
            //todo:CheckerTexture,
            //todo:GradientTexture,
            image_texture,
            //todo:MagicTexture,
            //todo:MusgraveTexture,
            //todo:NoiseTexture,
            //todo:PointDensity,
            //todo:SkyTexture,
            //todo:VoronoiTexture,
            //todo:WaveTexture,
            //todo:WhiteNoiseTexture,

            //vertex_component_flag_color
            bright_contrast,
            gamma,
            hue_saturation_value,
            invert,
            mix,
            //todo:replace,
            //todo:white_balance,
            //todo:color_mask,
            //todo:colorspace_conversion,

            //Vector,
            bump,
            displacement,
            mapping,
            normal_map,
            //todo:normal_blend,
            //todo:vector_curves,
            //todo:vector_displacement,
            //todo:vector_rotate,
            //todo:vector_transform,

            //Converter
            //todo:blackbody,
            clamp,
            combine_xyz,
            combine_rgb,
            separate_xyz,
            separate_rgb,
            //todo:float_curve,
            //todo:map_range,
            math,
            rgb_to_bw,
            vector_math,
            //todo:wavelength,
            //todo:dither,

            max
        };

        struct SLink {
            int id;
            int endNodeId;
            int startNodeId;
            int endAttr;
            int startAttr;
        };

        std::vector<SLink> m_links;
        std::vector<std::string> m_allNodeNames;

        bool m_isWindowVisible {};
        bool m_wantsToSave {};
        int m_cullMode {};
        int m_linkId {};
        SFile m_currentShaderFile;
        CShaderGraph m_shaderGraph;

        std::vector<SLink> GetNeighbors( int currentNode );

        std::unordered_map<ui_node_type, CShaderNode* (*)( CShaderGraph& )> m_nodeCreationMap { { ui_node_type::shader_node,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<COutputNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::vertex_output,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CVertexOutputNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::fragment_output,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CFragmentLitNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::math,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CMathNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::bright_contrast,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CBrightContrastNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::gamma,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CGammaNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::hue_saturation_value,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CHueSaturationValueNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::invert,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CColorInvertNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::mix,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CColorMixNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::image_texture,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CImageTextureNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::bump,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CBumpNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::displacement,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CDisplacementNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::mapping,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CMappingNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::normal_map,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CNormalMapNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::clamp,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CClampNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::rgb_to_bw,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CRGBtoBWNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::vector_math,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CVectorMathNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::fragment_unlit_output,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CFragmentUnlitNode>() );
                                                                                                  } },
                                                                                                { ui_node_type::camera_data,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CCameraData>() );
                                                                                                  } },
                                                                                                { ui_node_type::combine_xyz,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CCombineXYZ>() );
                                                                                                  } },
                                                                                                { ui_node_type::combine_rgb,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CCombineRGB>() );
                                                                                                  } },
                                                                                                { ui_node_type::separate_xyz,
                                                                                                  []( CShaderGraph& graph ) {
                                                                                                      return dynamic_cast<CShaderNode*>( graph.CreateNode<CSeparateXyz>() );
                                                                                                  } },
                                                                                                { ui_node_type::separate_rgb, []( CShaderGraph& graph ) {
                                                                                                     return dynamic_cast<CShaderNode*>( graph.CreateNode<CSeparateRgb>() );
                                                                                                 } } };

        static std::unordered_map<ui_node_type, const char*> m_nodeNameMap;

        void EnterFileName() noexcept;
        void Evaluate() noexcept;
        void AddNode( ui_node_type type, const BambiMath::Vector2& position );
        void AddLink( SLink& link ) noexcept;

        bool DrawBinding( SShaderBinding& binding ) noexcept;
        static const char* ToString( ui_node_type type );
    };
}// namespace FawnForge
