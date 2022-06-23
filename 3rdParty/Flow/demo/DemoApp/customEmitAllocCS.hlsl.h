#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
//
// Buffer Definitions: 
//
// cbuffer params
// {
//
//   uint4 minMaskIdx;                  // Offset:    0 Size:    16
//   uint4 maxMaskIdx;                  // Offset:   16 Size:    16
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim      HLSL Bind  Count
// ------------------------------ ---------- ------- ----------- -------------- ------
// maskUAV                               UAV    uint          3d             u0      1 
// params                            cbuffer      NA          NA            cb0      1 
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Input
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// no Output
cs_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer CB0[2], immediateIndexed
dcl_uav_typed_texture3d (uint,uint,uint,uint) u0
dcl_input vThreadID.xyz
dcl_temps 2
dcl_thread_group 8, 8, 8
iadd r0.xyzw, vThreadID.xyzz, cb0[0].xyzz
ige r1.xyz, r0.xywx, cb0[0].xyzx
and r1.x, r1.y, r1.x
and r1.x, r1.z, r1.x
ilt r1.yzw, r0.xxyw, cb0[1].xxyz
and r1.y, r1.z, r1.y
and r1.y, r1.w, r1.y
and r1.x, r1.y, r1.x
if_nz r1.x
  store_uav_typed u0.xyzw, r0.xyzw, l(1,1,1,1)
endif 
ret 
// Approximately 12 instruction slots used
#endif

const BYTE g_customEmitAllocCS[] =
{
     68,  88,  66,  67,  40,  49, 
    206, 141, 171,  96, 209, 139, 
    236,  95, 146,  56,  48,  52, 
     47,  50,   1,   0,   0,   0, 
    212,   3,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
    160,   1,   0,   0, 176,   1, 
      0,   0, 192,   1,   0,   0, 
     56,   3,   0,   0,  82,  68, 
     69,  70, 100,   1,   0,   0, 
      1,   0,   0,   0, 140,   0, 
      0,   0,   2,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
     83,  67,   0,   1,   0,   0, 
     59,   1,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
    124,   0,   0,   0,   4,   0, 
      0,   0,   4,   0,   0,   0, 
      8,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0, 132,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0, 109,  97, 
    115, 107,  85,  65,  86,   0, 
    112,  97, 114,  97, 109, 115, 
      0, 171, 132,   0,   0,   0, 
      2,   0,   0,   0, 164,   0, 
      0,   0,  32,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 244,   0,   0,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,   2,   0,   0,   0, 
     12,   1,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
     48,   1,   0,   0,  16,   0, 
      0,   0,  16,   0,   0,   0, 
      2,   0,   0,   0,  12,   1, 
      0,   0,   0,   0,   0,   0, 
    255, 255, 255, 255,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 109, 105, 
    110,  77,  97, 115, 107,  73, 
    100, 120,   0,  78, 118,  70, 
    108, 111, 119,  85, 105, 110, 
    116,  52,   0, 171,   1,   0, 
     19,   0,   1,   0,   4,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
    255,   0,   0,   0, 109,  97, 
    120,  77,  97, 115, 107,  73, 
    100, 120,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  49,  48,  46,  49, 
      0, 171,  73,  83,  71,  78, 
      8,   0,   0,   0,   0,   0, 
      0,   0,   8,   0,   0,   0, 
     79,  83,  71,  78,   8,   0, 
      0,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,  83,  72, 
     69,  88, 112,   1,   0,   0, 
     80,   0,   5,   0,  92,   0, 
      0,   0, 106,   8,   0,   1, 
     89,   0,   0,   4,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      2,   0,   0,   0, 156,  40, 
      0,   4,   0, 224,  17,   0, 
      0,   0,   0,   0,  68,  68, 
      0,   0,  95,   0,   0,   2, 
    114,   0,   2,   0, 104,   0, 
      0,   2,   2,   0,   0,   0, 
    155,   0,   0,   4,   8,   0, 
      0,   0,   8,   0,   0,   0, 
      8,   0,   0,   0,  30,   0, 
      0,   7, 242,   0,  16,   0, 
      0,   0,   0,   0,  70,  10, 
      2,   0,  70, 138,  32,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,  33,   0,   0,   8, 
    114,   0,  16,   0,   1,   0, 
      0,   0,  70,   3,  16,   0, 
      0,   0,   0,   0,  70, 130, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,   1,   0,   0,   7, 
     18,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     34,   0,   0,   8, 226,   0, 
     16,   0,   1,   0,   0,   0, 
      6,  13,  16,   0,   0,   0, 
      0,   0,   6, 137,  32,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
      1,   0,   0,   7,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   7,  18,   0,  16,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   1,   0, 
      0,   0,  31,   0,   4,   3, 
     10,   0,  16,   0,   1,   0, 
      0,   0, 164,   0,   0,  10, 
    242, 224,  17,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
     21,   0,   0,   1,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  12,   0, 
      0,   0,   2,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   5,   0, 
      0,   0,   1,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0
};
