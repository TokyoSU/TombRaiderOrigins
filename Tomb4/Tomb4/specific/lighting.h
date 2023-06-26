#pragma once

void S_CalculateStaticMeshLight(long x, long y, long z, long shade, ROOM_INFO* r);
void InitItemDynamicLighting(ItemInfo* item);
void SetupDynamicLight(DYNAMIC* light, ItemInfo* item);
void SetupLight(PCLIGHT* light, ItemInfo* item, long* ambient);
void mApplyMatrix(MATRIX_FLT* matrix, FVECTOR* start, FVECTOR* dest);
void mApplyTransposeMatrix(MATRIX_FLT* matrix, FVECTOR* start, FVECTOR* dest);
void CreateLightList(ItemInfo* item);
void FadeLightList(PCLIGHT* lights, long nLights);
void InitObjectLighting(ItemInfo* item);
void CalcAmbientLight(ItemInfo* item);
void ResetLighting();

extern ItemInfo* current_item;
extern long StaticMeshShade;
extern long ambientR, ambientG, ambientB;

extern FVECTOR lGlobalMeshPos;
extern SUNLIGHT_STRUCT SunLights[64];
extern POINTLIGHT_STRUCT PointLights[64];
extern POINTLIGHT_STRUCT SpotLights[64];
extern long nSunLights, nPointLights, nSpotLights, nShadowLights, nTotalLights;
