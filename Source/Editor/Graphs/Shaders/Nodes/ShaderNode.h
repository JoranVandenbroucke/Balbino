//
// Created by joran on 25/02/2023.
//

#pragma once
#include <array>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

enum shader_stage : uint16_t
{
    shader_stage_vertex                  = 1 << 0,
    shader_stage_tessellation_control    = 1 << 1,
    shader_stage_tessellation_evaluation = 1 << 2,
    shader_stage_geometry                = 1 << 3,
    shader_stage_fragment                = 1 << 4,
    shader_stage_compute                 = 1 << 5,
    shader_stage_raygen                  = 1 << 6,
    shader_stage_any_hit                 = 1 << 7,
    shader_stage_closest_hit             = 1 << 8,
    shader_stage_miss                    = 1 << 9,
    shader_stage_intersection            = 1 << 10,
    shader_stage_callable                = 1 << 11,
    shader_stage_task                    = 1 << 12,
    shader_stage_mesh                    = 1 << 13,
    shader_stage_max                     = 1 << 14,
    shader_stage_all                     = shader_stage_max - 1
};

const char* ToStringUi( shader_stage type );
const char* ToString( shader_stage type );
const char* ToDefine( shader_stage type );

struct SSocketType final {
    enum class var_type : uint8_t
    {
        none,
        float_type,
        int_type,
        color,
        vector2,
        vector3,
        vector4,
        shader
    };
    enum compiler_define
    {
        // Vertex flags
        compiler_define_uses_model_view      = ( 1 << 0 ),
        compiler_define_uses_world_position  = ( 1 << 1 ),
        compiler_define_uses_normal          = ( 1 << 2 ),
        compiler_define_uses_tangent         = ( 1 << 3 ),
        compiler_define_uses_colour          = ( 1 << 4 ),
        compiler_define_uses_uv              = ( 1 << 5 ),
        compiler_define_uses_position_offset = ( 1 << 6 ),

        // BRDF flags
        compiler_define_diffuse_lambert     = ( 1 << 7 ),
        compiler_define_diffuse_burley      = ( 1 << 8 ),
        compiler_define_diffuse_oren_nayar  = ( 1 << 9 ),
        compiler_define_specular_blin_phong = ( 1 << 10 ),
        compiler_define_specular_beckmann   = ( 1 << 11 ),
        compiler_define_specular_ggx        = ( 1 << 12 ),

        // Visibility flags
        compiler_define_specular_implicit            = ( 1 << 13 ),
        compiler_define_specular_neumann             = ( 1 << 14 ),
        compiler_define_specular_cook_torrance       = ( 1 << 15 ),
        compiler_define_specular_kelemen             = ( 1 << 16 ),
        compiler_define_specular_ggx_correlated      = ( 1 << 17 ),
        compiler_define_specular_ggx_correlated_fast = ( 1 << 18 ),
        compiler_define_specular_kelemen_two         = ( 1 << 19 ),
        compiler_define_specular_neubelt             = ( 1 << 20 ),

        // Fresnel functions
        compiler_define_fresnel_cook_torrance = ( 1 << 21 ),
        compiler_define_fresnel_schlick       = ( 1 << 22 )
    };
    bool hasEditorValues { true };
    var_type type;
    compiler_define flags {};
    std::string name {};
    std::string uiName {};
};
const char* ToDefine( SSocketType::compiler_define type );

struct SDefine final {
    SDefine() = default;
    SDefine( char n[ 64 ], char v[ 64 ], int32_t t )
        : shaderTypes { t }
    {
        strcpy_s( name, n );
        strcpy_s( value, v );
    };
    SDefine( const char* n, const char* v, int32_t t )
        : shaderTypes { t }
    {
        strcpy_s( name, n );
        strcpy_s( value, v );
    };

    char name[ 64 ] {};
    char value[ 64 ] {};
    int32_t shaderTypes { 0 };
    int32_t priority { 0 };
};
struct SInclude final {
    SInclude() = default;
    SInclude( const char* f, int32_t s )
        : shaderTypes { s }
    {
        strcpy_s( file, f );
    };

    char file[ 64 ] {};
    int32_t shaderTypes { 0 };
    int32_t priority { 0 };
};
struct SShaderBinding final {
    enum class layout
    {
        input,
        subpass_input,
        output,
        subpass_output,
        image,
        image_sampler,
        image_storage,
        sampler,
        uniform,
        storage,
        max
    };
    enum class value_type
    {
        bool_type,
        int_type,
        u_int,
        float_type,
        double_type,
        vec2,
        vec3,
        vec4,
        dvec2,
        dvec3,
        dvec4,
        bvec2,
        bvec3,
        bvec4,
        ivec2,
        ivec3,
        ivec4,
        uvec2,
        uvec3,
        uvec4,
        mat2,
        mat3,
        mat4,
        mat2x3,
        mat2x4,
        mat3x2,
        mat3x4,
        mat4x2,
        mat4x3,
        dmat2,
        dmat3,
        dmat4,
        dmat2x3,
        dmat2x4,
        dmat3x2,
        dmat3x4,
        dmat4x2,
        dmat4x3,
        sampler_1d,
        sampler_2d,
        sampler_3d,
        sampler_cube,
        sampler_2d_rect,
        sampler_1d_array,
        sampler_2d_array,
        sampler_cube_array,
        sampler_1d_shadow,
        sampler_2d_shadow,
        sampler_cube_shadow,
        sampler_2d_rect_shadow,
        sampler_1d_array_shadow,
        sampler_2d_array_shadow,
        sampler_cube_array_shadow,
        struct_type,
        max
    };

    SShaderBinding() = default;
    SShaderBinding( layout _layout, value_type _value_type, int _set, int _binding, int _arraySize, const char* _name, const char* _instanceName = "" )
        : layoutType { _layout }
        , valueType { _value_type }
        , set { _set }
        , binding { _binding }
        , arraySize { _arraySize }
        , name {}
    {
        strcpy_s( name, _name );
        strcpy_s( instanceName, _instanceName );
    }
    SShaderBinding( layout _layout, value_type _value_type, int _set, int _binding, const char* _arraySize, const char* _name, const char* _instanceName = "" )
        : layoutType { _layout }
        , valueType { _value_type }
        , set { _set }
        , binding { _binding }
        , name {}
    {
        strcpy_s( name, _name );
        strcpy_s( instanceName, _instanceName );
        strcpy_s( arraySizeString, _arraySize );
    }
    layout layoutType { layout::input };
    value_type valueType { value_type::float_type };
    int32_t set { autoConfig };
    int32_t binding { autoConfig };
    int32_t arraySize { 1 };
    int32_t shaderTypes { 0 };
    int32_t priority { 0 };

    char name[ 64 ] { "" };
    char instanceName[ 64 ] { "" };
    char arraySizeString[ 64 ] { "" };

    std::vector<SShaderBinding> members;

    static constexpr int autoConfig { -1 };

    static const char* ToString( layout type );
    static const char* ToString( value_type type );
    static bool IsBindingSimularEnough( const SShaderBinding& b1, const SShaderBinding& b2 );

private:
    static std::unordered_map<SShaderBinding::value_type, std::string_view> valueTypeStrings;
};
struct SShaderInfo {
    constexpr void AddDefine( const std::string_view define, const std::string_view value, int32_t priority = 0, bool global = false )
    {
        for ( auto& d : defines )
        {
            if ( std::strcmp( d.name, define.data() ) == 0 && std::strcmp( d.value, value.data() ) == 0 )
            {
                d.shaderTypes |= shaderTypes;
                if ( global )
                {
                    d.shaderTypes = shader_stage_all;
                }
                return;
            }
        }
        defines.emplace_back( define.data(), value.data(), shaderTypes );
        defines.back().priority = priority;
        if ( global )
        {
            defines.back().shaderTypes = shader_stage_all;
        }
    }
    constexpr void AddInclude( const std::string_view& file, int32_t priority = 0, bool global = false )
    {
        for ( auto& p : includes )
        {
            if ( std::strcmp( p.file, file.data() ) == 0 )
            {
                p.shaderTypes |= shaderTypes;
                if ( global )
                {
                    p.shaderTypes = shader_stage_all;
                }
                return;
            }
        }
        includes.emplace_back( file.data(), shaderTypes );
        includes.back().priority = priority;
        if ( global )
        {
            includes.back().shaderTypes = shader_stage_all;
        }
    }
    constexpr void AddBinding( const SShaderBinding& binding, int32_t priority = 0, bool global = false )
    {
        for ( auto& b : bindings )
        {
            if ( std::strcmp( b.name, binding.name ) == 0 && SShaderBinding::IsBindingSimularEnough( b, binding ) )
            {
                b.shaderTypes |= shaderTypes;
                if ( global )
                {
                    b.shaderTypes = shader_stage_all;
                }
                return;
            }
        }
        bindings.push_back( binding );
        bindings.back().shaderTypes = shaderTypes;
        bindings.back().priority    = priority;
        if ( global )
        {
            bindings.back().shaderTypes = shader_stage_all;
        }
    }
    constexpr void Clear()
    {
        defines.clear();
        includes.clear();
        bindings.clear();
    }
    int32_t shaderTypes { 0 };
    std::vector<SDefine> defines;
    std::vector<SInclude> includes;
    std::vector<SShaderBinding> bindings;
};

namespace std
{
    template<>
    struct hash<SShaderBinding> {
        size_t operator()( const SShaderBinding& binding ) const noexcept
        {
            size_t seed = 0;

            // Hash the members of the struct
            HashCombine( seed, binding.layoutType );
            HashCombine( seed, binding.valueType );
            HashCombine( seed, binding.set );
            HashCombine( seed, binding.binding );
            HashCombine( seed, binding.arraySize );
            for ( int i { 0 }; i < 64; ++i )
            {
                HashCombine( seed, binding.name[ i ] );
                HashCombine( seed, binding.instanceName[ i ] );
                HashCombine( seed, binding.arraySizeString[ i ] );
            }

            for ( const auto& member : binding.members )
            {
                HashCombine( seed, operator()( member ) );
            }

            return seed;
        }

    private:
        // Combine the hash of a member variable into a seed mix_mode_value
        template<typename T>
        static void HashCombine( size_t& seed, const T& value ) noexcept
        {
            seed ^= std::hash<T> {}( value ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
        }
    };
    template<>
    struct hash<SDefine> {
        size_t operator()( const SDefine& define ) const noexcept
        {
            size_t seed = 0;
            // constexpr Hash the members of the struct
            for ( int i { 0 }; i < 64; ++i )
            {
                HashCombine( seed, define.name[ i ] );
            }
            for ( int i { 0 }; i < 64; ++i )
            {
                HashCombine( seed, define.value[ i ] );
            }

            return seed;
        }

    private:
        // Combine the hash of a member variable into a seed mix_mode_value
        template<typename T>
        static void HashCombine( size_t& seed, const T& value ) noexcept
        {
            seed ^= std::hash<T> {}( value ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
        }
    };
    template<>
    struct hash<SInclude> {
        size_t operator()( const SInclude& include ) const noexcept
        {
            size_t seed = 0;
            // constexpr Hash the members of the struct
            for ( int i { 0 }; i < 64; ++i )
            {
                HashCombine( seed, include.file[ i ] );
            }

            return seed;
        }

    private:
        // Combine the hash of a member variable into a seed mix_mode_value
        template<typename T>
        static void HashCombine( size_t& seed, const T& value ) noexcept
        {
            seed ^= std::hash<T> {}( value ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
        }
    };
}// namespace std

class CShaderInput;

class CShaderOutput;

class CShaderNode;

class CShaderInput final
{
public:
    constexpr explicit CShaderInput( SSocketType type, CShaderNode* pParent )
        : m_socketType { std::move( type ) }
        , m_pParent { pParent }
    {
    }
    ~CShaderInput() = default;

    [[nodiscard]] inline const std::string& GetName() const
    {
        return m_socketType.uiName;
    }
    [[nodiscard]] inline const std::string& GetShaderName() const
    {
        return m_socketType.name;
    }
    [[nodiscard]] inline SSocketType::var_type GetType() const
    {
        return m_socketType.type;
    }
    [[nodiscard]] inline SSocketType::compiler_define GetFlags() const
    {
        return m_socketType.flags;
    }
    [[nodiscard]] inline bool HasEditorValues() const
    {
        return m_socketType.hasEditorValues;
    }
    [[nodiscard]] inline bool IsConnected() const
    {
        return m_pLink != nullptr;
    }
    void Disconnect() noexcept;
    void Disconnect( const CShaderOutput* pOut ) noexcept;

    void Connect( CShaderOutput* pOutput ) noexcept;
    [[nodiscard]] inline std::array<float, 3> GetValue() const
    {
        return m_value;
    }
    constexpr void SetValue( const std::array<float, 3>& val )
    {
        m_value = val;
    }
    void SetSocketType( const SSocketType& type );
    [[nodiscard]] inline const CShaderOutput* GetLink() const
    {
        return m_pLink;
    }
    [[nodiscard]] inline CShaderOutput* GetLink()
    {
        return m_pLink;
    }

private:
    SSocketType m_socketType;
    CShaderNode* m_pParent;
    CShaderOutput* m_pLink { nullptr };
    std::array<float, 3> m_value {};
};

class CShaderOutput final
{
public:
    constexpr explicit CShaderOutput( SSocketType type, CShaderNode* pParent )
        : m_socketType { std::move( type ) }
        , m_pParent { pParent }
    {
    }
    ~CShaderOutput() = default;

    [[nodiscard]] inline const std::string& GetName() const
    {
        return m_socketType.uiName;
    }
    [[nodiscard]] inline const std::string& GetShaderName() const
    {
        return m_socketType.name;
    }
    [[nodiscard]] inline SSocketType::var_type GetType() const
    {
        return m_socketType.type;
    }
    [[nodiscard]] inline const CShaderNode* GetParent() const
    {
        return m_pParent;
    }
    [[nodiscard]] inline CShaderNode* GetParent()
    {
        return m_pParent;
    }
    [[nodiscard]] inline bool HasConnection() const
    {
        return !m_links.empty();
    }

    void Disconnect() noexcept;
    void Disconnect( CShaderInput* pIn ) noexcept;

    void Connect( CShaderInput* pInput ) noexcept;

private:
    const SSocketType m_socketType;
    CShaderNode* m_pParent;
    std::vector<CShaderInput*> m_links;
};

class CShaderNode
{
public:
    explicit CShaderNode( int& id, const std::vector<SSocketType>& inputs, const std::vector<SSocketType>& outputs );
    virtual ~CShaderNode() = default;

    [[nodiscard]] CShaderInput* GetInput( int id ) const;
    [[nodiscard]] CShaderInput* GetInput( const char* name ) const;
    [[nodiscard]] CShaderOutput* GetOutput( int id ) const;
    [[nodiscard]] CShaderOutput* GetOutput( const char* name ) const;
    [[nodiscard]] CShaderInput* GetInput( std::string_view name ) const;
    [[nodiscard]] CShaderOutput* GetOutput( std::string_view name ) const;

    [[nodiscard]] inline int GetId() const
    {
        return m_id;
    }
    [[nodiscard]] inline const std::vector<CShaderInput*>& GetInputNodes() const
    {
        return m_inputs;
    }
    [[nodiscard]] inline const std::vector<CShaderOutput*>& GetOutputNodes() const
    {
        return m_outputs;
    }

    virtual void Draw() noexcept;
    [[nodiscard]] virtual std::string GetCode() const           = 0;
    virtual void GetShaderInfo( SShaderInfo& shaderInfo ) const = 0;
    virtual void SetBindings( const std::vector<SShaderBinding>& bindings );

protected:
    void AddInputNode( const SSocketType& type );
    void RemoveInputNode() noexcept;

private:
    int m_id;
    std::vector<CShaderInput*> m_inputs;
    std::vector<CShaderOutput*> m_outputs;
};
