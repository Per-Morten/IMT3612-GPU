#ifndef PMS_LIB_TYPES
#define PMS_LIB_TYPES

///////////////////////////////////////////////////////////
/// \brief Signed integer types.
///////////////////////////////////////////////////////////
typedef char        int8_t;
typedef short       nt16_t;
typedef int         int32_t;
typedef long        int64_t;

///////////////////////////////////////////////////////////
/// \brief Unsigned integer types.
///////////////////////////////////////////////////////////
typedef uchar       uint8_t;
typedef ushort      uint16_t;
typedef uint        uint32_t;
typedef ulong       uint64_t;

///////////////////////////////////////////////////////////
/// \brief Floating point types.
///////////////////////////////////////////////////////////
typedef float       float32_t;
typedef half        float16_t;

///////////////////////////////////////////////////////////
/// \brief Vector signed integer types.
///////////////////////////////////////////////////////////
typedef char2       v2_int8_t;
typedef char3       v3_int8_t;
typedef char4       v4_int8_t;
typedef char8       v8_int8_t;
typedef char16      v16_int8_t;

typedef short2      v2_int16_t;
typedef short3      v3_int16_t;
typedef short4      v4_int16_t;
typedef short8      v8_int16_t;
typedef short16     v16_int16_t;

typedef int2        v2_int32_t;
typedef int3        v3_int32_t;
typedef int4        v4_int32_t;
typedef int8        v8_int32_t;
typedef int16       v16_int32_t;

typedef long2       v2_int64_t;
typedef long3       v3_int64_t;
typedef long4       v4_int64_t;
typedef long8       v8_int64_t;
typedef long16      v16_int64_t;

///////////////////////////////////////////////////////////
/// \brief Vector unsigned integer types.
///////////////////////////////////////////////////////////
typedef uchar2      v2_uint8_t;
typedef uchar3      v3_uint8_t;
typedef uchar4      v4_uint8_t;
typedef uchar8      v8_uint8_t;
typedef uchar16     v16_uint8_t;

typedef ushort2     v2_uint16_t;
typedef ushort3     v3_uint16_t;
typedef ushort4     v4_uint16_t;
typedef ushort8     v8_uint16_t;
typedef ushort16    v16_uint16_t;

typedef uint2       v2_uint32_t;
typedef uint3       v3_uint32_t;
typedef uint4       v4_uint32_t;
typedef uint8       v8_uint32_t;
typedef uint16      v16_uint32_t;

typedef ulong2      v2_uint64_t;
typedef ulong3      v3_uint64_t;
typedef ulong4      v4_uint64_t;
typedef ulong8      v8_uint64_t;
typedef ulong16     v16_uint64_t;

///////////////////////////////////////////////////////////
/// \brief Vector floating point types.
///////////////////////////////////////////////////////////
typedef float2      v2_float32_t;
typedef float3      v3_float32_t;
typedef float4      v4_float32_t;
typedef float8      v8_float32_t;
typedef float16     v16_float32_t;

typedef half2       v2_float16_t;
typedef half3       v3_float16_t;
typedef half4       v4_float16_t;
typedef half8       v8_float16_t;
typedef half16      v16_float16_t;

#endif
