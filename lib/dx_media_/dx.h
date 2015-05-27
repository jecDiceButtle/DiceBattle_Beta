
#pragma once

//--------------------------------------------------------------------------
// ÅúäÓñ{ìIÇ»inludeï∂
//--------------------------------------------------------------------------
#include "../GpLib.h"

typedef D3DXVECTOR2 DxVec2;
typedef D3DXVECTOR3 DxVec3;

#include <vector>
#include <list>
#include <algorithm>
#include <typeinfo.h>

using namespace std;

#define SAFE_DELETE(p)			{ if(p!=NULL) { delete p; (p)=NULL;} }
#define	SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASEDC(p,h)     { if (p) { (p)->ReleaseDC((h)); (h)=NULL;} }
#define SAFE_GETDC(p,h)         ((p)!=NULL && ((p)->GetDC(&(h))==D3D_OK) )
#define SAFE_RELEASE(p)         { if (p) { (p)->Release();  (p)=NULL; } }


