#include "visualizer.h"

std::string serialize_wstring(std::wstring& wstr) {
	char* begin = reinterpret_cast<char*>(wstr.data());
	std::string str;
	int size = wstr.size() * 2;
	str.resize(size);
	for (int i = 0; i < size; ++i) {
		str[i] = begin[i];
	}
	return std::move(str);
}

std::wstring deserialize_wstring(const std::string& str) {
	std::wstring wstr;
	int size = str.size();
	wstr.resize(size / 2);
	char* begin = reinterpret_cast<char*>(wstr.data());
	for (int i = 0; i < size; ++i) {
		begin[i] = str[i];
	}
	return std::move(wstr);
}


CCharacterSystem::CCharacterSystem() noexcept
{
}

CCharacterSystem::~CCharacterSystem()
{
}

void CCharacterSystem::serialize(CCharacterType& type, void* buffer_ptr, int& size)
{
	visualizer_serialize::CCharacterType character_type;

	visualizer_serialize::CCharacterData character_data;
	
	character_type.set_m_strtypename(serialize_wstring(type.m_strTypeName));

	character_type.set_m_strcharactername(serialize_wstring(type.m_strCharacterName));

	const auto& data = type.m_data[0];

	character_data.set_m_nanimationmode(data.m_nAnimationMode);
	character_data.set_m_ncurrentanimation(data.m_nCurrentAnimation);
	character_data.set_m_nsavedanimation(data.m_nSavedAnimation);
	character_data.set_m_ncountframe(data.m_nCountFrame);
	character_data.set_m_nsavedframe(data.m_nSavedFrame);

	const auto& matrix = data.m_ModelData.m_mtxModelToWorld;
	const auto& vector = data.m_ModelData.m_vtTranslation;


	for (int i = 0; i < 4; ++i) {
		character_data.add_m_mtxmodeltoworld(matrix.m1[i]);
		character_data.add_m_mtxmodeltoworld(matrix.m2[i]);
		character_data.add_m_mtxmodeltoworld(matrix.m3[i]);
		character_data.add_m_mtxmodeltoworld(matrix.m4[i]);
	}

	for (int i = 0; i < 3; ++i) {
		character_data.add_m_vttranslation(vector.m[i]);
	}

	character_data.add_m_ptcamerapositionandlerp(data.m_ModelData.m_ptCameraPositionAndLerp.m[3]);
				
	*character_type.add_m_data() = character_data;

	size = character_type.ByteSizeLong();

	character_type.SerializeToArray(buffer_ptr, size);
}

void CCharacterSystem::deserialize(CCharacterType& type, void* buffer_ptr, int size)
{
	visualizer_serialize::CCharacterType character_type;


	if (!character_type.ParseFromArray(buffer_ptr, size))
	{
		return;
	}

	type.m_strTypeName = deserialize_wstring(character_type.m_strtypename());

	type.m_strCharacterName = deserialize_wstring(character_type.m_strcharactername());

	auto& character_data = character_type.m_data()[0];

	if (type.m_data.empty()) {
		type.m_data.resize(1);
	}

	auto& data = type.m_data[0];

	data.m_nAnimationMode = character_data.m_nanimationmode();
	data.m_nCountFrame = character_data.m_ncountframe();
	data.m_nCurrentAnimation = character_data.m_ncurrentanimation();
	data.m_nSavedAnimation = character_data.m_nsavedanimation();
	data.m_nSavedFrame = character_data.m_nsavedframe();

	auto& matrix = data.m_ModelData.m_mtxModelToWorld;
	const auto& character_matrix = character_data.m_mtxmodeltoworld();

	for (int i = 0; i < 4; ++i) {
		matrix.m1[i] = character_matrix[i];
		matrix.m2[i] = character_matrix[i + 1];
		matrix.m3[i] = character_matrix[i + 2];
		matrix.m4[i] = character_matrix[i + 3];
	}

	auto& vector = data.m_ModelData.m_vtTranslation;
	const auto& character_vector = character_data.m_vttranslation();

	for (int i = 0; i < 3; ++i) {
		vector.m[i] = character_vector[i];
	}

	data.m_ModelData.m_ptCameraPositionAndLerp.m[3] = character_data.m_ptcamerapositionandlerp()[0];
}
