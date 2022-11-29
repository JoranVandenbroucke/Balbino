#pragma once

namespace BalVulkan
{
    class CSampler;
    
    class CImageView;
    
    class CCommandPool;
    
    class CBuffer;
    
    struct EMemoryPropertyFlagBits
    {
        enum Enum
        {
            DeviceLocalBit       = 0x00000001,
            HostVisibleBit       = 0x00000002,
            HostCoherentBit      = 0x00000004,
            HostCachedBit        = 0x00000008,
            LazilyAllocatedBit   = 0x00000010,
            ProtectedBit         = 0x00000020,
            DeviceCoherentBitAmd = 0x00000040,
            DeviceUncachedBitAmd = 0x00000080,
            RdmaCapableBitNv     = 0x00000100,
            FlagBitsMaxEnum      = 0x7FFFFFFF
        };
    };
    struct EBufferUsageFlagBits
    {
        enum Enum
        {
            TransferSrcBit         = 0x00000001,
            TransferDstBit         = 0x00000002,
            UniformTexelBufferBit  = 0x00000004,
            StorageTexelBufferBit  = 0x00000008,
            UniformBufferBit       = 0x00000010,
            StorageBufferBit       = 0x00000020,
            IndexBufferBit         = 0x00000040,
            VertexBufferBit        = 0x00000080,
            IndirectBufferBit      = 0x00000100,
            ShaderDeviceAddressBit = 0x00020000,
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeSrcBitKhr = 0x00002000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeDstBitKhr = 0x00004000,
#endif
            TransformFeedbackBufferBitExt                 = 0x00000800,
            TransformFeedbackCounterBufferBitExt          = 0x00001000,
            ConditionalRenderingBitExt                    = 0x00000200,
            AccelerationStructureBuildInputReadOnlyBitKhr = 0x00080000,
            AccelerationStructureStorageBitKhr            = 0x00100000,
            ShaderBindingTableBitKhr                      = 0x00000400,
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeDstBitKhr = 0x00008000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeSrcBitKhr = 0x00010000,
#endif
            RayTracingBitNv           = ShaderBindingTableBitKhr,
            ShaderDeviceAddressBitExt = ShaderDeviceAddressBit,
            ShaderDeviceAddressBitKhr = ShaderDeviceAddressBit,
            FlagBitsMaxEnum           = 0x7FFFFFFF
        };
    };
    
    struct ECullMode
    {
        enum Enum : uint8_t
        {
            Back,
            Front,
            None
        };
    };
    
    struct EShaderStage
    {
        enum Enum : uint16_t
        {
            VertexShader   = 1,
            GeometryShader = 2,
            FragmentShader = 4,
            
            RaygenShader       = 8,
            AnyHitShader       = 16,
            ClosestHitShader   = 32,
            MissShader         = 64,
            IntersectionShader = 128,
            CallableShader     = 512,
            TaskShader         = 1024,
            MeshShader         = 2048
        };
    };
    struct EImageLayout
    {
        enum Enum
        {
            Undefined                             = 0,
            General                               = 1,
            ColorAttachmentOptimal                = 2,
            DepthStencilAttachmentOptimal         = 3,
            DepthStencilReadOnlyOptimal           = 4,
            ShaderReadOnlyOptimal                 = 5,
            TransferSrcOptimal                    = 6,
            TransferDstOptimal                    = 7,
            Preinitialized                        = 8,
            DepthReadOnlyStencilAttachmentOptimal = 1000117000,
            DepthAttachmentStencilReadOnlyOptimal = 1000117001,
            DepthAttachmentOptimal                = 1000241000,
            DepthReadOnlyOptimal                  = 1000241001,
            StencilAttachmentOptimal              = 1000241002,
            StencilReadOnlyOptimal                = 1000241003,
            PresentSrcKhr                         = 1000001002,
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeDstKhr = 1000024000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeSrcKhr = 1000024001,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeDpbKhr = 1000024002,
#endif
            SharedPresentKhr                        = 1000111000,
            FragmentDensityMapOptimalExt            = 1000218000,
            FragmentShadingRateAttachmentOptimalKhr = 1000164003,
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeDstKhr = 1000299000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeSrcKhr = 1000299001,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeDpbKhr = 1000299002,
#endif
            ReadOnlyOptimalKhr                       = 1000314000,
            AttachmentOptimalKhr                     = 1000314001,
            DepthReadOnlyStencilAttachmentOptimalKhr = DepthReadOnlyStencilAttachmentOptimal,
            DepthAttachmentStencilReadOnlyOptimalKhr = DepthAttachmentStencilReadOnlyOptimal,
            ShadingRateOptimalNv                     = FragmentShadingRateAttachmentOptimalKhr,
            DepthAttachmentOptimalKhr                = DepthAttachmentOptimal,
            DepthReadOnlyOptimalKhr                  = DepthReadOnlyOptimal,
            StencilAttachmentOptimalKhr              = StencilAttachmentOptimal,
            StencilReadOnlyOptimalKhr                = StencilReadOnlyOptimal,
            MaxEnum                                  = 0x7FFFFFFF
        };
    };
    //Coppy of Vulkan VkImageUsageFlagBits
    struct EImageUsageFlagBits
    {
        enum Enum
        {
            TransferSrcBit            = 0x00000001,
            TransferDstBit            = 0x00000002,
            SampledBit                = 0x00000004,
            StorageBit                = 0x00000008,
            ColorAttachmentBit        = 0x00000010,
            DepthStencilAttachmentBit = 0x00000020,
            TransientAttachmentBit    = 0x00000040,
            InputAttachmentBit        = 0x00000080,
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeDstBitKHR = 0x00000400,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeSrcBitKHR = 0x00000800,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoDecodeDpbBit_KHR = 0x00001000,
#endif
            FragmentDensityMapBitExt            = 0x00000200,
            FragmentShadingRateAttachmentBitKhr = 0x00000100,
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeDstBitKHR = 0x00002000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeSrcBitKHR = 0x00004000,
#endif
#ifdef VK_ENABLE_BETA_EXTENSIONS
            VideoEncodeDpbBitKHR = 0x00008000,
#endif
            InvocationMaskBitHuawei = 0x00040000,
            ShadingRateImageBitNv   = FragmentShadingRateAttachmentBitKhr,
            FlagBitsMaxEnum         = 0x7FFFFFFF
        };
    };
    struct EImageViewType
    {
        enum Enum : uint8_t
        {
            View1D,
            View1DArray,
            View2D,
            View2DArray,
            View3D,
            Rect,
            RectArray,
            Cube,
            CubeArray,
        };
    };
    
    struct EPipelineStageFlagBits
    {
        enum Enum
        {
            TopOfPipeBit                        = 0x00000001,
            DrawIndirectBit                     = 0x00000002,
            VertexInputBit                      = 0x00000004,
            VertexShaderBit                     = 0x00000008,
            TessellationControlShaderBit        = 0x00000010,
            TessellationEvaluationShaderBit     = 0x00000020,
            GeometryShaderBit                   = 0x00000040,
            FragmentShaderBit                   = 0x00000080,
            EarlyFragmentTestsBit               = 0x00000100,
            LateFragmentTestsBit                = 0x00000200,
            ColorAttachmentOutputBit            = 0x00000400,
            ComputeShaderBit                    = 0x00000800,
            TransferBit                         = 0x00001000,
            BottomOfPipeBit                     = 0x00002000,
            HostBit                             = 0x00004000,
            AllGraphicsBit                      = 0x00008000,
            AllCommandsBit                      = 0x00010000,
            TransformFeedbackBitExt             = 0x01000000,
            ConditionalRenderingBitExt          = 0x00040000,
            AccelerationStructureBuildBitKHR    = 0x02000000,
            RayTracingShaderBitKHR              = 0x00200000,
            ShadingRateImageBitNv               = 0x00400000,
            TaskShaderBitNv                     = 0x00080000,
            MeshShaderBitNv                     = 0x00100000,
            FragmentDensityProcessBitExt        = 0x00800000,
            CommandPreprocessBitNv              = 0x00020000,
            NoneKHR                             = 0,
            RayTracingShaderBitNv               = RayTracingShaderBitKHR,
            AccelerationStructureBuildBitNv     = AccelerationStructureBuildBitKHR,
            FragmentShadingRateAttachmentBitKHR = ShadingRateImageBitNv,
            FlagBitsMaxEnum                     = 0x7FFFFFFF
        };
    };
    
    struct EFilter
    {
        enum Enum
        {
            Nearest  = 0,
            Linear   = 1,
            CubicImg = 1000015000,
            CubicExt = CubicImg,
            MaxEnum  = 0x7FFFFFFF
        };
    };
    
    struct ESamplerAddressMode
    {
        enum Enum
        {
            Repeat               = 0,
            MirroredRepeat       = 1,
            ClampToEdge          = 2,
            ClampToBorder        = 3,
            MirrorClampToEdge    = 4,
            MirrorClampToEdgeKHR = MirrorClampToEdge,
            MaxEnum              = 0x7FFFFFFF
        };
    };
    
    //Coppy of Vulkan VkFormat
    struct EFormat
    {
        enum Enum
        {
            Undefined                               = 0,
            R4G4UnormPack8                          = 1,
            R4G4B4A4UnormPack16                     = 2,
            B4G4R4A4UnormPack16                     = 3,
            R5G6B5UnormPack16                       = 4,
            B5G6R5UnormPack16                       = 5,
            R5G5B5A1UnormPack16                     = 6,
            B5G5R5A1UnormPack16                     = 7,
            A1R5G5B5UnormPack16                     = 8,
            R8Unorm                                 = 9,
            R8Snorm                                 = 10,
            R8Uscaled                               = 11,
            R8Sscaled                               = 12,
            R8Uint                                  = 13,
            R8Sint                                  = 14,
            R8Srgb                                  = 15,
            R8G8Unorm                               = 16,
            R8G8Snorm                               = 17,
            R8G8Uscaled                             = 18,
            R8G8Sscaled                             = 19,
            R8G8Uint                                = 20,
            R8G8Sint                                = 21,
            R8G8Srgb                                = 22,
            R8G8B8Unorm                             = 23,
            R8G8B8Snorm                             = 24,
            R8G8B8Uscaled                           = 25,
            R8G8B8Sscaled                           = 26,
            R8G8B8Uint                              = 27,
            R8G8B8Sint                              = 28,
            R8G8B8Srgb                              = 29,
            B8G8R8Unorm                             = 30,
            B8G8R8Snorm                             = 31,
            B8G8R8Uscaled                           = 32,
            B8G8R8Sscaled                           = 33,
            B8G8R8Uint                              = 34,
            B8G8R8Sint                              = 35,
            B8G8R8Srgb                              = 36,
            R8G8B8A8Unorm                           = 37,
            R8G8B8A8Snorm                           = 38,
            R8G8B8A8Uscaled                         = 39,
            R8G8B8A8Sscaled                         = 40,
            R8G8B8A8Uint                            = 41,
            R8G8B8A8Sint                            = 42,
            R8G8B8A8Srgb                            = 43,
            B8G8R8A8Unorm                           = 44,
            B8G8R8A8Snorm                           = 45,
            B8G8R8A8Uscaled                         = 46,
            B8G8R8A8Sscaled                         = 47,
            B8G8R8A8Uint                            = 48,
            B8G8R8A8Sint                            = 49,
            B8G8R8A8Srgb                            = 50,
            A8B8G8R8UnormPack32                     = 51,
            A8B8G8R8SnormPack32                     = 52,
            A8B8G8R8UscaledPack32                   = 53,
            A8B8G8R8SscaledPack32                   = 54,
            A8B8G8R8UintPack32                      = 55,
            A8B8G8R8SintPack32                      = 56,
            A8B8G8R8SrgbPack32                      = 57,
            A2R10G10B10UnormPack32                  = 58,
            A2R10G10B10SnormPack32                  = 59,
            A2R10G10B10UscaledPack32                = 60,
            A2R10G10B10SscaledPack32                = 61,
            A2R10G10B10UintPack32                   = 62,
            A2R10G10B10SintPack32                   = 63,
            A2B10G10R10UnormPack32                  = 64,
            A2B10G10R10SnormPack32                  = 65,
            A2B10G10R10UscaledPack32                = 66,
            A2B10G10R10SscaledPack32                = 67,
            A2B10G10R10UintPack32                   = 68,
            A2B10G10R10SintPack32                   = 69,
            R16Unorm                                = 70,
            R16Snorm                                = 71,
            R16Uscaled                              = 72,
            R16Sscaled                              = 73,
            R16Uint                                 = 74,
            R16Sint                                 = 75,
            R16Sfloat                               = 76,
            R16G16Unorm                             = 77,
            R16G16Snorm                             = 78,
            R16G16Uscaled                           = 79,
            R16G16Sscaled                           = 80,
            R16G16Uint                              = 81,
            R16G16Sint                              = 82,
            R16G16Sfloat                            = 83,
            R16G16B16Unorm                          = 84,
            R16G16B16Snorm                          = 85,
            R16G16B16Uscaled                        = 86,
            R16G16B16Sscaled                        = 87,
            R16G16B16Uint                           = 88,
            R16G16B16Sint                           = 89,
            R16G16B16Sfloat                         = 90,
            R16G16B16A16Unorm                       = 91,
            R16G16B16A16Snorm                       = 92,
            R16G16B16A16Uscaled                     = 93,
            R16G16B16A16Sscaled                     = 94,
            R16G16B16A16Uint                        = 95,
            R16G16B16A16Sint                        = 96,
            R16G16B16A16Sfloat                      = 97,
            R32Uint                                 = 98,
            R32Sint                                 = 99,
            R32Sfloat                               = 100,
            R32G32Uint                              = 101,
            R32G32Sint                              = 102,
            R32G32Sfloat                            = 103,
            R32G32B32Uint                           = 104,
            R32G32B32Sint                           = 105,
            R32G32B32Sfloat                         = 106,
            R32G32B32A32Uint                        = 107,
            R32G32B32A32Sint                        = 108,
            R32G32B32A32Sfloat                      = 109,
            R64Uint                                 = 110,
            R64Sint                                 = 111,
            R64Sfloat                               = 112,
            R64G64Uint                              = 113,
            R64G64Sint                              = 114,
            R64G64Sfloat                            = 115,
            R64G64B64Uint                           = 116,
            R64G64B64Sint                           = 117,
            R64G64B64Sfloat                         = 118,
            R64G64B64A64Uint                        = 119,
            R64G64B64A64Sint                        = 120,
            R64G64B64A64Sfloat                      = 121,
            B10G11R11UfloatPack32                   = 122,
            E5B9G9R9UfloatPack32                    = 123,
            D16Unorm                                = 124,
            X8D24UnormPack32                        = 125,
            D32Sfloat                               = 126,
            S8Uint                                  = 127,
            D16Unorms8Uint                          = 128,
            D24Unorms8Uint                          = 129,
            D32Sfloats8Uint                         = 130,
            Bc1RgbUnormBlock                        = 131,
            Bc1RgbSrgbBlock                         = 132,
            Bc1RgbaUnormBlock                       = 133,
            Bc1RgbaSrgbBlock                        = 134,
            Bc2UnormBlock                           = 135,
            Bc2SrgbBlock                            = 136,
            Bc3UnormBlock                           = 137,
            Bc3SrgbBlock                            = 138,
            Bc4UnormBlock                           = 139,
            Bc4SnormBlock                           = 140,
            Bc5UnormBlock                           = 141,
            Bc5SnormBlock                           = 142,
            Bc6HufloatBlock                         = 143,
            Bc6HsfloatBlock                         = 144,
            Bc7UnormBlock                           = 145,
            Bc7SrgbBlock                            = 146,
            Etc2R8G8B8UnormBlock                    = 147,
            Etc2R8G8B8SrgbBlock                     = 148,
            Etc2R8G8B8A1UnormBlock                  = 149,
            Etc2R8G8B8A1SrgbBlock                   = 150,
            Etc2R8G8B8A8UnormBlock                  = 151,
            Etc2R8G8B8A8SrgbBlock                   = 152,
            Eacr11UnormBlock                        = 153,
            Eacr11SnormBlock                        = 154,
            Eacr11G11UnormBlock                     = 155,
            Eacr11G11SnormBlock                     = 156,
            Astc4X4UnormBlock                       = 157,
            Astc4X4SrgbBlock                        = 158,
            Astc5X4UnormBlock                       = 159,
            Astc5X4SrgbBlock                        = 160,
            Astc5X5UnormBlock                       = 161,
            Astc5X5SrgbBlock                        = 162,
            Astc6X5UnormBlock                       = 163,
            Astc6X5SrgbBlock                        = 164,
            Astc6X6UnormBlock                       = 165,
            Astc6X6SrgbBlock                        = 166,
            Astc8X5UnormBlock                       = 167,
            Astc8X5SrgbBlock                        = 168,
            Astc8X6UnormBlock                       = 169,
            Astc8X6SrgbBlock                        = 170,
            Astc8X8UnormBlock                       = 171,
            Astc8X8SrgbBlock                        = 172,
            Astc10X5UnormBlock                      = 173,
            Astc10X5SrgbBlock                       = 174,
            Astc10X6UnormBlock                      = 175,
            Astc10X6SrgbBlock                       = 176,
            Astc10X8UnormBlock                      = 177,
            Astc10X8SrgbBlock                       = 178,
            Astc10X10UnormBlock                     = 179,
            Astc10X10SrgbBlock                      = 180,
            Astc12X10UnormBlock                     = 181,
            Astc12X10SrgbBlock                      = 182,
            Astc12X12UnormBlock                     = 183,
            Astc12X12SrgbBlock                      = 184,
            G8B8G8R8422Unorm                        = 1000156000,
            B8G8R8G8422Unorm                        = 1000156001,
            G8B8R83Plane420Unorm                    = 1000156002,
            G8B8R82Plane420Unorm                    = 1000156003,
            G8B8R83Plane422Unorm                    = 1000156004,
            G8B8R82Plane422Unorm                    = 1000156005,
            G8B8R83Plane444Unorm                    = 1000156006,
            R10X6UnormPack16                        = 1000156007,
            R10X6G10X6Unorm2Pack16                  = 1000156008,
            R10X6G10X6B10X6A10X6Unorm4Pack16        = 1000156009,
            G10X6B10X6g10X6R10X6422Unorm4Pack16     = 1000156010,
            B10X6G10X6R10X6G10X6422Unorm4Pack16     = 1000156011,
            G10X6B10X6R10X63Plane420Unorm3Pack16    = 1000156012,
            G10X6B10X6R10X62Plane420Unorm3Pack16    = 1000156013,
            G10X6B10X6R10X63Plane422Unorm3Pack16    = 1000156014,
            G10X6B10X6R10X62Plane422Unorm3Pack16    = 1000156015,
            G10X6B10X6R10X63Plane444Unorm3Pack16    = 1000156016,
            R12X4UnormPack16                        = 1000156017,
            R12X4G12X4Unorm2Pack16                  = 1000156018,
            R12X4G12X4B12X4A12X4Unorm4Pack16        = 1000156019,
            G12X4B12X4G12X4R12X4422Unorm4Pack16     = 1000156020,
            B12X4G12X4R12X4G12X4422Unorm4Pack16     = 1000156021,
            G12X4B12X4R12X43Plane420Unorm3Pack16    = 1000156022,
            G12X4B12X4R12X42Plane420Unorm3Pack16    = 1000156023,
            G12X4B12X4R12X43Plane422Unorm3Pack16    = 1000156024,
            G12X4B12X4R12X42Plane422Unorm3Pack16    = 1000156025,
            G12X4B12X4R12X43Plane444Unorm3Pack16    = 1000156026,
            G16B16G16R16422Unorm                    = 1000156027,
            B16G16R16G16422Unorm                    = 1000156028,
            G16B16R163Plane420Unorm                 = 1000156029,
            G16B16R162Plane420Unorm                 = 1000156030,
            G16B16R163Plane422Unorm                 = 1000156031,
            G16B16R162Plane422Unorm                 = 1000156032,
            G16B16R163Plane444Unorm                 = 1000156033,
            Pvrtc12BppUnormBlockimg                 = 1000054000,
            Pvrtc14BppUnormBlockimg                 = 1000054001,
            Pvrtc22BppUnormBlockimg                 = 1000054002,
            Pvrtc24BppUnormBlockimg                 = 1000054003,
            Pvrtc12BppSrgbBlockimg                  = 1000054004,
            Pvrtc14BppSrgbBlockimg                  = 1000054005,
            Pvrtc22BppSrgbBlockimg                  = 1000054006,
            Pvrtc24BppSrgbBlockimg                  = 1000054007,
            Astc4X4SfloatBlockext                   = 1000066000,
            Astc5X4SfloatBlockext                   = 1000066001,
            Astc5X5SfloatBlockext                   = 1000066002,
            Astc6X5SfloatBlockext                   = 1000066003,
            Astc6X6SfloatBlockext                   = 1000066004,
            Astc8X5SfloatBlockext                   = 1000066005,
            Astc8X6SfloatBlockext                   = 1000066006,
            Astc8X8SfloatBlockext                   = 1000066007,
            Astc10X5SfloatBlockext                  = 1000066008,
            Astc10X6SfloatBlockext                  = 1000066009,
            Astc10X8SfloatBlockext                  = 1000066010,
            Astc10X10SfloatBlockext                 = 1000066011,
            Astc12X10SfloatBlockext                 = 1000066012,
            Astc12X12SfloatBlockext                 = 1000066013,
            G8B8R82Plane444Unormext                 = 1000330000,
            G10X6B10X6R10X62Plane444Unorm3Pack16EXT = 1000330001,
            G12X4B12X4R12X42Plane444Unorm3Pack16EXT = 1000330002,
            G16B16R162Plane444Unormext              = 1000330003,
            A4R4G4B4UnormPack16Ext                  = 1000340000,
            A4B4G4R4UnormPack16Ext                  = 1000340001,
            G8B8G8R8422UnormKhr                     = G8B8G8R8422Unorm,
            B8G8R8G8422UnormKhr                     = B8G8R8G8422Unorm,
            G8B8R83Plane420UnormKHR                 = G8B8R83Plane420Unorm,
            G8B8R82Plane420UnormKHR                 = G8B8R82Plane420Unorm,
            G8B8R83Plane422UnormKHR                 = G8B8R83Plane422Unorm,
            G8B8R82Plane422UnormKHR                 = G8B8R82Plane422Unorm,
            G8B8R83Plane444UnormKHR                 = G8B8R83Plane444Unorm,
            R10X6UnormPack16KHR                     = R10X6UnormPack16,
            R10X6G10X6Unorm2Pack16KHR               = R10X6G10X6Unorm2Pack16,
            R10X6G10X6B10X6A10X6Unorm4Pack16KHR     = R10X6G10X6B10X6A10X6Unorm4Pack16,
            G10X6B10X6G10X6R10X6422Unorm4Pack16KHR  = G10X6B10X6g10X6R10X6422Unorm4Pack16,
            B10X6G10X6R10X6G10X6422Unorm4Pack16KHR  = B10X6G10X6R10X6G10X6422Unorm4Pack16,
            G10X6B10X6R10X63Plane420Unorm3Pack16KHR = G10X6B10X6R10X63Plane420Unorm3Pack16,
            G10X6B10X6R10X62Plane420Unorm3Pack16KHR = G10X6B10X6R10X62Plane420Unorm3Pack16,
            G10X6B10X6R10X63Plane422Unorm3Pack16KHR = G10X6B10X6R10X63Plane422Unorm3Pack16,
            G10X6B10X6R10X62Plane422Unorm3Pack16KHR = G10X6B10X6R10X62Plane422Unorm3Pack16,
            G10X6B10X6R10X63Plane444Unorm3Pack16KHR = G10X6B10X6R10X63Plane444Unorm3Pack16,
            R12X4UnormPack16KHR                     = R12X4UnormPack16,
            R12X4G12X4Unorm2Pack16KHR               = R12X4G12X4Unorm2Pack16,
            R12X4G12X4B12X4A12X4Unorm4Pack16KHR     = R12X4G12X4B12X4A12X4Unorm4Pack16,
            G12X4B12X4G12X4R12X4422Unorm4Pack16KHR  = G12X4B12X4G12X4R12X4422Unorm4Pack16,
            B12X4G12X4R12X4G12X4422Unorm4Pack16KHR  = B12X4G12X4R12X4G12X4422Unorm4Pack16,
            G12X4B12X4R12X43Plane420Unorm3Pack16KHR = G12X4B12X4R12X43Plane420Unorm3Pack16,
            G12X4B12X4R12X42Plane420Unorm3Pack16KHR = G12X4B12X4R12X42Plane420Unorm3Pack16,
            G12X4B12X4R12X43Plane422Unorm3Pack16KHR = G12X4B12X4R12X43Plane422Unorm3Pack16,
            G12X4B12X4R12X42Plane422Unorm3Pack16KHR = G12X4B12X4R12X42Plane422Unorm3Pack16,
            G12X4B12X4R12X43Plane444Unorm3Pack16KHR = G12X4B12X4R12X43Plane444Unorm3Pack16,
            G16B16G16R16422UnormKhr                 = G16B16G16R16422Unorm,
            B16G16R16G16422UnormKhr                 = B16G16R16G16422Unorm,
            G16B16R163Plane420UnormKHR              = G16B16R163Plane420Unorm,
            G16B16R162Plane420UnormKHR              = G16B16R162Plane420Unorm,
            G16B16R163Plane422UnormKHR              = G16B16R163Plane422Unorm,
            G16B16R162Plane422UnormKHR              = G16B16R162Plane422Unorm,
            G16B16R163Plane444UnormKHR              = G16B16R163Plane444Unorm,
            MaxEnum                                 = 0x7fffffff
        };
    };
    
    struct SDescriptorImageInfo
    {
        CImageView* view;
        CSampler  * sampler;
    };
    
    struct SDescriptorBufferInfo
    {
        CBuffer* buffer;
    };
    
    struct SDescriptorSet
    {
        struct EType
        {
            enum Enum
            {
                Image,
                Buffer,
                DynamicBuffer,
                BufferStorage,
                Input
            };
        };
        union DescriptorInfo
        {
            SDescriptorImageInfo  image;
            SDescriptorBufferInfo buffer;
        }        description;
        uint32_t set, binding;
        EType::Enum descriptorType;
    
        SDescriptorSet() = default;
        
        SDescriptorSet( EType::Enum buffer, CBuffer* pModelBuffer, uint32_t set, uint32_t binding );
        SDescriptorSet( EType::Enum buffer, CImageView* pModelBuffer, CSampler* pSampler, uint32_t set, uint32_t binding );
    };
    
    void DrawMesh( const CCommandPool* command, uint32_t indexCount, uint32_t firstIndex = 0, uint32_t firstInstance = 0, uint32_t instanceCount = 1 );
}
