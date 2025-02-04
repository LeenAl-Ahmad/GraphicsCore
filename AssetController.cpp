#include "AssetController.h"
#include "FileController.h"

StackAllocator* AssetController::Stack = nullptr;

AssetController::AssetController()
{
}

void AssetController::Initialize(int _sS)
{
	Stack = new StackAllocator();
	AssetController::Stack->AllocateStack(_sS);
	Asset::Pool = new ObjectPool<Asset>();
}

AssetController::~AssetController() {
	Clear();
}

void AssetController::Clear() {
	// Remove all asset objects from the object pool
	for (auto const& x : m_assets) {
		Asset::Pool->ReleaseResource(x.second);
	}

	if (Asset::Pool != nullptr) {
		delete Asset::Pool;
		Asset::Pool = nullptr;
	}

	Stack->ClearMemory();
	m_assets.clear();
}


Asset* AssetController::GetAsset(string _g)
{
	if (m_assets.count(_g) != 0)
	{
		return m_assets[_g];
	}

	Asset* asset = Asset::Pool->GetResource();
	asset->SetGUID(_g);
	asset->SetDataSize(FileController::Instance().GetFileSize(_g));
	asset->SetData(Stack->GetMemory(asset->GetDataSize()));
	FileController::Instance().ReadFile(_g, asset->GetData(), asset->GetDataSize());

	m_assets[_g] = asset;

	return asset;
}