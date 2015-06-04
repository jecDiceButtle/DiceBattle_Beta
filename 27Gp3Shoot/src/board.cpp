#include "board.h"
#include "../../lib/gplib.h"

//tuika
#include "movableObject.h"

//3Dカメラ
extern LPDIRECT3DDEVICE9 pD3DDevice;
//メッシュ全体管理クラス
extern std::shared_ptr<Dx_Camera> dxCamera;
extern std::shared_ptr<DX3DMESHMANAGER> meshManage;


/*
*	H27/6/4 丸山
*	panel_をboolからintへ変更。　false(穴)だったものを-1へ変更。true(通常床）を0へ変更。
*/

namespace game
{
	//tuika
	using namespace ci_ext;
	using namespace gplib;

	//**************************************************************************************//
	//作成するプログラムで必要となる変数、定数定義
	//**************************************************************************************//

	const ci_ext::Vec3i Board::MAPSIZE = ci_ext::Vec3i(4, 4, 0);

	//**************************************************************************************//
	//関数記述
	//**************************************************************************************//

	/*bool Board::isMovablePosition(const ci_ext::Vec3i &pos)
	{
	return (0 <= pos.x() && pos.x() <= MAPSIZE.x()
	&& (0 <= pos.y() && pos.y() <= MAPSIZE.y()));
	}*/

	bool Board::HolePos(const ci_ext::Vec3i &pos)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (panel_[j][i] == -1 &&
					j*panelLength <= pos.x() && pos.x() <= j*panelLength + 10.f&&
					i*panelLength <= pos.y() && pos.y() <= i*panelLength + 10.f)
				{
					return false;
				}

			}
		}
	}

	void Board::colorChange(int i, int j)
	{
	}


	//**************************************************************************************//
	//デフォルト関数
	//**************************************************************************************//
	Board::Board(const std::string& objectName)
		:
		MovableObject(
		DrawObjf(objectName)
		),
		scale(10.f, 10.f, 10.f),
		panelLength(10.f),
		panel_({
			{ 0, 0, 0, 0 },
			{ 0, 0, -1, 0 },
			{ 0, 0, 0, 0 },
			{ -1, 0, 0, 0 } })
			{
	}
	Board::Board(const std::string& objectName, const std::vector<std::vector<int>>& mapData_)
		:
		MovableObject(
		DrawObjf(objectName)
		),
		scale(10.f, 10.f, 10.f),
		panelLength(10.f)
	{
		panel_ = mapData_;
	}


	void Board::render()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{

				if (panel_[i][j] == 0)
				{
					if ((i + j) % 2 == 0)
					{
						meshManage->drawMesh(Vec3f(j*panelLength, 0.f, -i*panelLength), "board1", Vec3f(0.f, 0.f, 0.f), ARGB(255, 255, 255, 255), scale);

					}
					else if ((i + j) % 2 == 1)
					{
						meshManage->drawMesh(Vec3f(j*panelLength, 0.f, -i*panelLength), "board2", Vec3f(0.f, 0.f, 0.f), ARGB(255, 255, 255, 255), scale);
					}



				}

			}
		}


	}

	void Board::update()
	{
	}

}