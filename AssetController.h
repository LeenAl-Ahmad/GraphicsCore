
#ifndef  ASSET_CONTROLLER_H
#define ASSET_CONTROLLER_H

#include "StandardIncludes.h"
#include "StackAllocator.h"
#include "Asset.h"

class AssetController : public Singleton<AssetController>
{
public:
	AssetController();
	virtual ~AssetController();

	void Initialize(int _sS);
	void Clear();
	Asset* GetAsset(string _g);

	static StackAllocator* Stack;

private:
	map<string, Asset*> m_assets;
};

#endif // ! ASSET_CONTROLLER_H


