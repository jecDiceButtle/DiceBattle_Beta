

#pragma once

#include "dx.h"

#include "media_manager.h"
#include "dx_font.h"
#include "dx_graphics3d.h"

#include	<memory>

#include	<unordered_map>


class DX3DMESHMANAGER
{

private:
	//描画管理クラス
	Dx_Graphics3D *g;
	//メディア管理クラス
	MediaManager *m_manager;
	//アニメ付メッシュ
	class AnimMesh{
		public:
			DxAnimeMesh	*mesh;
		Dx_AnimeAuxiliary *anime;
		string			filename_;
		AnimMesh(MediaManager *manager, const string& fileName, const string& path)
		{
			mesh = manager->LoadAnimeMeshFromX(fileName, path);
			anime = new Dx_AnimeAuxiliary(mesh->pAnimController);
			filename_ = fileName;
		}
		~AnimMesh()
		{
			SAFE_DELETE(anime);
		}
		void del()
		{
		}
	};
	class Mesh{
	public:
		DxMesh		*mesh;
		string		filename_;
		Mesh(MediaManager *manager,const string& fileName,const string& path)
		{
			mesh = manager->LoadMeshFromX(fileName,path);
			filename_ = fileName;
		}
		~Mesh()
		{
			
		}
		void del()
		{
			SAFE_DELETE(mesh);
    }
	};
	typedef std::shared_ptr<AnimMesh> pAnimMesh;
	typedef std::shared_ptr<Mesh> pMesh;
	unordered_map<string,pMesh>		DxMeshTable;
	unordered_map<string,pAnimMesh>	DxAnimMeshTable;
public:
	enum MESHTYPE{
		MESH = 0,
		ANIMMESH = 1,
	};
	Dx_Graphics3D* getG();

	DX3DMESHMANAGER();
	~DX3DMESHMANAGER();

	void loadMesh(const string& handleName,const string& fileName,const string& path,MESHTYPE type = MESHTYPE::MESH);
	void drawMesh(DxVec3 pos, const string& handleName, DxVec3 angle, D3DCOLOR color = ARGB(255, 255, 255, 255), DxVec3 scale = DxVec3(1.f, 1.f, 1.f));
	//tuika
	//コメントアウトの場所が謎でbaguがあるかもしれない
	void drawMeshQuaternion(DxVec3 pos, const string& handleName, DxVec3 angle, D3DCOLOR color/* = ARGB(255, 255, 255, 255)*/, DxVec3 scale/* = DxVec3(1.f, 1.f, 1.f)*/, D3DXMATRIX matrot);
	void drawAnimMesh(DxVec3 pos, const string& handleName, DxVec3 angle, D3DCOLOR color = ARGB(255, 255, 255, 255), DxVec3 scale = DxVec3(1.f, 1.f, 1.f));
	void ChangeAnim(const string& handleName, int nowMotion);
	void step(const string& handleName,float spd);
	float nowTrack(const string& handleName, int nowMotion);

  void createCloneAnimeController(const string& handleName, Dx_AnimeAuxiliary *anime);
 
};



