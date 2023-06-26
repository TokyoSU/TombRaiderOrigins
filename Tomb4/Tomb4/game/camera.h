#pragma once

void InitialiseCamera();
void MoveCamera(GAME_VECTOR* ideal, long speed);
long mgLOS(GAME_VECTOR* start, GAME_VECTOR* target, long push);
long CameraCollisionBounds(GAME_VECTOR* ideal, long push, long yfirst);
void LaraTorch(PHD_VECTOR* Soffset, PHD_VECTOR* Eoffset, short yrot, long brightness);
void ChaseCamera(ItemInfo* item);
void CombatCamera(ItemInfo* item);
void LookCamera(ItemInfo* item);
void FixedCamera();
void BinocularCamera(ItemInfo* item);
void CalculateCamera();

extern CAMERA_INFO camera;
extern GAME_VECTOR ForcedFixedCamera;
extern char UseForcedFixedCamera;
extern PHD_VECTOR LaraTorchStart;
extern PHD_VECTOR LaraTorchEnd;
extern long bLaraTorch;
extern long LaraTorchIntensity;
extern long LaraTorchYRot;
extern camera_type BinocularOldCamera;
extern long BinocularOn;
extern long BinocularRange;
extern long ExittingBinos;
extern long LaserSight;
extern SHATTER_ITEM ShatterItem;
