#include "pch.h"
	
namespace Dear
{
	class Editor final : public Balbino::Application
	{
	public:
		Editor() = default;
		~Editor() override = default;
		Editor(const Editor&) = delete;
		Editor(Editor&&) = delete;
		Editor& operator=(const Editor&) = delete;
		Editor& operator=(Editor&&) = delete;

		void LoadGame() override;
	};

	void Editor::LoadGame()
	{
		//Balbino::CManager::GetMeshManager()->AddMesh("../Resources/Models/cube.ply");
		//Balbino::CManager::GetMeshManager()->AddMesh("../Resources/Models/cube.obj");
		//Balbino::CManager::GetMeshManager()->AddMesh("../Resources/Models/sponza.obj");

		////Lion
		//Balbino::CMaterial* pMaterial{ Balbino::CManager::GetMaterialManager()->AddMaterial() };
		//Balbino::CTexture* pTexture{ Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\lion_diff.png)") };
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\lion_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\lion_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////roof
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_roof_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_roof_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_roof_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////leaf
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_thorn_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_thorn_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_thorn_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////flower
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_plant_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_plant_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_plant_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////vase_round
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_round_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_round_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_round_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////back_plate
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\background_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\background_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\background_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////brick
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\spnza_bricks_a_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\spnza_bricks_a_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\spnza_bricks_a_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////arch
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_arch_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_arch_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_arch_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////ceiling
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_ceiling_a_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_ceiling_a_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_ceiling_a_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////column_a
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_a_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_a_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_a_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////floor
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_floor_a_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_floor_a_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_floor_a_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////column_c
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_c_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_c_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_c_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////detail
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_details_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_details_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_details_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////column_b
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_b_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_b_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_column_b_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////Material__47
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_purple_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_purple_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_purple_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////flagpole
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_flagpole_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_flagpole_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_flagpole_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////fabric_e
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_green_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_green_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_green_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////fabric_d
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_blue_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_blue_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_blue_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////fabric_a
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_red_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_red_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_fabric_red_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////fabric_g
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_blue_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_blue_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_blue_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////fabric_c
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_red_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_red_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_red_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////fabric_f
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_green_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_green_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\sponza_curtain_green_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////chain
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\chain_texture_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\chain_texture_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\chain_texture_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////vase hanging
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_hanging_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_hanging_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_hanging_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);

		////vase
		//pMaterial = Balbino::CManager::GetMaterialManager()->AddMaterial();
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_diff.png)");
		//pMaterial->SetDiffuseTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_normal.png)");
		//pMaterial->SetNormalTexture(pTexture);
		//pTexture = Balbino::CManager::GetTextureManager()->AddTexture(R"(..\Resources\Textures\Sponza\vase_srm.png)");
		//pMaterial->SetSrmTexture(pTexture);
		this->Application::LoadGame();
	}
}


Balbino::Application* Balbino::CreateApplication()
{
	return new Dear::Editor{};
}
