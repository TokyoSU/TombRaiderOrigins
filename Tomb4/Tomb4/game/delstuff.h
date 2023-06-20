#pragma once

enum CalcMatrixFlag
{
	CM_Skin = 0,
	CM_Joint = 1,
	CM_Mirror = 2
};

void DrawLara(ITEM_INFO* item, bool isMirror);
void GetLaraJointPos(PHD_VECTOR* pos, long node);
void SetLaraUnderwaterNodes();
void Rich_CalcLaraMatrices_Normal(short* frame, long* bone, CalcMatrixFlag flag);
void Rich_CalcLaraMatrices_Interpolated(short* frame1, short* frame2, long frac, long rate, long* bone, CalcMatrixFlag flag);
void CalcLaraMatrices(CalcMatrixFlag flag);

extern short* GLaraShadowframe;
extern MATRIX_FLT lara_matrices[NUM_LARA_MESHES];
extern MATRIX_FLT lara_joint_matrices[NUM_LARA_MESHES];
extern long LaraNodeAmbient[2];
extern long bLaraUnderWater;
extern uchar LaraNodeUnderwater[15];
extern char SkinVertNums[40][12];
extern char ScratchVertNums[40][12];
extern char HairRotScratchVertNums[5][12];
extern char bLaraInWater;
