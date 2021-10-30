#ifdef _RAY_TRACING
RaytracingAccelerationStructure sceneBVH;
ByteAddressBuffer bindlessBuffers[] : register(t0, space1);
Texture2D bindlessTextures[] : register(t0, space2);
#endif