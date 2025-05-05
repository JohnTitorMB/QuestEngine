#ifndef  _ASSETS_H_
#define _ASSETS_H_
#include <string>
class Assets
{
protected :
	Assets() {}
	Assets(std::string name) { m_name = name; }
	virtual ~Assets() {}
	std::string m_name;

	friend class AssetsManager;

public:
	std::string GetAssetName() const { return m_name; };
};

#endif // ! _ASSETS_H_
