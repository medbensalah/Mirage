#pragma once
#include <string>

#include "VisualComponent.h"

namespace Mirage::VisualComponents
{
	class Button : public VisualComponent
	{
	public:
		Button(const std::string& label, ImVec2& size);

		virtual ~Button() override {}

		virtual void Draw(float scale) override;

	private:
		std::string m_Label;
		ImVec2 m_Size;
	};
}
