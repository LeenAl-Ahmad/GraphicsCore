#pragma once

#ifndef ASSET_H
#define ASSET_H


#include "objPool.h"

class Asset
{
public:
	Asset();
	virtual ~Asset() {}

	string GetGUID() { return m_GUID; }
	void SetGUID(string _g) { m_GUID = _g; }
	int GetDataSize() { return m_dataSize; }
	void SetDataSize(int _s) { m_dataSize = _s; }
	byte* GetData() { return m_data; }
	void SetData(byte* _d) { m_data = _d; }

	void ToString();
	static ObjectPool<Asset>* Pool;

private:
	string m_GUID;
	int m_dataSize;
	byte* m_data;
};

#endif // !ASSET_h

