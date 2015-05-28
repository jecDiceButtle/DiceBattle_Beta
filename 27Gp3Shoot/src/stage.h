#pragma once
#include "../../lib/ci_ext/object.hpp"
#include "../../lib/ci_ext/vec3.hpp"
#include "../../lib/gplib.h"

namespace game
{
	class CSceneStage : public ci_ext::Object
	{
	private:

		//*************************************************//
		//�@�萔
		//*************************************************//

		

		//*************************************************//
		//�@�ϐ�
		//*************************************************//

	private:
		

		//*************************************************//
		//�@�֐�
		//*************************************************//

		

	public:

		CSceneStage(const std::string& objectName);

		void init() override;
		void render() override;
		void update() override;
	};

}