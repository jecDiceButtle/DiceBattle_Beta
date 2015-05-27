#include "board.h"
#include "../../lib/gplib.h"

//tuika
#include "movableObject.h"

//3D�J����
extern LPDIRECT3DDEVICE9 pD3DDevice;
//���b�V���S�̊Ǘ��N���X
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;
//tuika


namespace game
{
	//tuika
	using namespace ci_ext;
	using namespace gplib;
	//tuika


	//**************************************************************************************//
	//�쐬����v���O�����ŕK�v�ƂȂ�ϐ��A�萔��`
	//**************************************************************************************//

	const ci_ext::Vec3i Board::MAPSIZE = ci_ext::Vec3i(4,4,0);

	//**************************************************************************************//
	//�֐��L�q
	//**************************************************************************************//

	bool Board::isMovablePosition(const ci_ext::Vec3i &pos)
	{
		return (0 <= pos.x() && pos.x() <= MAPSIZE.x()
			&& (0 <= pos.y() && pos.y() <= MAPSIZE.y()));
	}



	Board::Board(const std::string& objectName)
		:
		MovableObject(
		DrawObjf(objectName)
		)
	{

	}

	void Board::render()
	{
		meshManage->drawMesh(DxVec3(0.f, 0.f, 0.f), "board", DxVec3(0.f, 0.f, 0.f), ARGB(255, 255, 255, 255), DxVec3(10.f, 10.f, 10.f));
	}

	void Board::update()
	{

	}

}