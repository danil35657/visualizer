#pragma once

#include <string>
#include <vector>

#include <character_type.pb.h>


class CMatrix
{
public:
	float m1[4];
	float m2[4];
	float m3[4];
	float m4[4];
};

class CVector4
{
public:
	float m[4];
};

class CCharacterSystem
{
public:
	struct CModelBuffer
	{
		CMatrix  m_mtxModelToWorld;
		CVector4 m_vtTranslation;
		CVector4 m_ptCameraPositionAndLerp;
	};

	struct CCharacterData
	{
		CModelBuffer m_ModelData; 

		unsigned m_nAnimationMode = 0;

		unsigned m_nCurrentAnimation = 1;
		unsigned m_nSavedAnimation = 0;

		int m_nCountFrame = 0;
		int m_nSavedFrame = 0;
	};

	struct CCharacterType
	{
		std::wstring                m_strTypeName;
		std::wstring                m_strCharacterName;
		std::vector<CCharacterData> m_data;
	};

	CCharacterSystem() noexcept;

	virtual ~CCharacterSystem();

	void serialize(CCharacterType& type, std::string& output);

	void deserialize(CCharacterType& type, const std::string& input);


public:
	std::vector<CCharacterType> m_pCharacters;
	bool m_bRender = true;
};