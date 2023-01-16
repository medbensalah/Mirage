#pragma once

#include <string>

#include "VisualComponent.h"

namespace Mirage::VisualComponents
{
	class Button : public VisualComponent
	{
	public:
		Button(std::string label) : m_Label(label) {};
		~Button() override {}
		
		void Draw(float scale) override;

	private:
		std::string m_Label;
	};
}
