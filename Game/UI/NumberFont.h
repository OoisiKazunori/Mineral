#pragma once
#include "../Game/DrawCallSet.h"
#include "../KazLibrary/Helper/ISinglton.h"

class NumberFont : public ISingleton<NumberFont> {

public:

	std::array<KazBufferHelper::BufferData, 10> m_font;

public:

	void Load();

};