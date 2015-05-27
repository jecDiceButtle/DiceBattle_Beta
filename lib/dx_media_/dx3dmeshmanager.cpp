

#include "dx3dmeshmanager.h"

extern LPDIRECT3DDEVICE9		pD3DDevice;

Dx_Graphics3D* DX3DMESHMANAGER::getG(){	
	return g; 
}

DX3DMESHMANAGER::DX3DMESHMANAGER()
{
	//�`��Ǘ��N���X�̐���
	g = new Dx_Graphics3D(pD3DDevice);
	//���f�B�A�Ǘ��N���X�̐���
	m_manager = new MediaManager(pD3DDevice);

}

DX3DMESHMANAGER::~DX3DMESHMANAGER()
{
	//�e���b�V���̏���
/*	for(auto& obj : DxMeshTable)
	{
		obj.second->del();
	}
-*/
	DxMeshTable.clear();
/*
	for(auto& obj : DxAnimMeshTable)
	{
		obj.second->del();
	}
*/
	DxAnimMeshTable.clear();
	//���f�B�A�Ǘ��N���X�̔j��
	SAFE_DELETE(m_manager);
	//�`��Ǘ��N���X�̔j��
	SAFE_DELETE(g);
}

void DX3DMESHMANAGER::loadMesh(const string& handleName,const string& fileName,const string& path,MESHTYPE type)
{
	//�A�j�������Ƃ���ŕʂ̓ǂݍ��݂��s��
	if(type == MESHTYPE::MESH){
		//�������̂��`�F�b�N����B
		DxMeshTable.insert(std::unordered_map<string, DX3DMESHMANAGER::pMesh>::value_type(
			handleName,
			pMesh(new Mesh(m_manager, fileName, path))
			)
			);
	}else if(type == MESHTYPE::ANIMMESH){
		//�������̂��`�F�b�N����B

		DxAnimMeshTable.insert(std::unordered_map<string, DX3DMESHMANAGER::pAnimMesh>::value_type(
			handleName,
			pAnimMesh(new AnimMesh(m_manager, fileName, path))
			)
			);
	}
}

void DX3DMESHMANAGER::drawMesh(DxVec3 pos, const string& handleName, DxVec3 angle, D3DCOLOR color, DxVec3 scale)
{
	if(DxMeshTable.find(handleName) != DxMeshTable.end())
		g->DrawModel(DxMeshTable.at(handleName)->mesh, &pos, &scale, &angle, &color);
}
//tuika
void DX3DMESHMANAGER::drawMeshQuaternion(DxVec3 pos, const string& handleName, DxVec3 angle, D3DCOLOR color, DxVec3 scale, D3DXMATRIX matrot)
{
	if (DxMeshTable.find(handleName) != DxMeshTable.end())
		g->DrawModel(DxMeshTable.at(handleName)->mesh, &pos, &scale, NULL, &color, &matrot);
}
void DX3DMESHMANAGER::drawAnimMesh(DxVec3 pos, const string& handleName, DxVec3 angle, D3DCOLOR color, DxVec3 scale)
{
	if(DxAnimMeshTable.find(handleName) != DxAnimMeshTable.end())
		g->DrawModel(DxAnimMeshTable.at(handleName)->mesh,&pos,&scale,&angle,&color);
}
void DX3DMESHMANAGER::ChangeAnim(const string& handleName,int nowMotion)
{
	if(DxAnimMeshTable.find(handleName) != DxAnimMeshTable.end())
		DxAnimMeshTable.at(handleName)->anime->ChangeAnim(nowMotion);
}
void DX3DMESHMANAGER::step(const string& handleName,float spd)
{
	//�A�j���[�V�����Đ�
	if(DxAnimMeshTable.find(handleName) != DxAnimMeshTable.end()){
		DxAnimMeshTable.at(handleName)->anime->PlayAnimation(spd);
	}
}
float DX3DMESHMANAGER::nowTrack(const string& handleName,int nowMotion)
{
	D3DXTRACK_DESC desc;
	if(DxAnimMeshTable.find(handleName) != DxAnimMeshTable.end()){
		DxAnimMeshTable.at(handleName)->anime->GetTrackDesc(nowMotion,&desc);
		return (float)desc.Position;
	}else{
		return -1;
	}
	return -1;
}

void DX3DMESHMANAGER::createCloneAnimeController(const string& handleName, Dx_AnimeAuxiliary *anime)
{
  LPD3DXANIMATIONCONTROLLER anim;
  DxAnimMeshTable.at(handleName)->mesh->pAnimController->CloneAnimationController(
    DxAnimMeshTable.at(handleName)->mesh->pAnimController->GetMaxNumAnimationOutputs(),
    DxAnimMeshTable.at(handleName)->mesh->pAnimController->GetMaxNumAnimationSets(),
    DxAnimMeshTable.at(handleName)->mesh->pAnimController->GetMaxNumTracks(),
    DxAnimMeshTable.at(handleName)->mesh->pAnimController->GetMaxNumEvents(),
    &anim);
  anime->SetAnimController(anim);
}


