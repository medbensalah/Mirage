#include "InputField.h"

#include "Mirage/Core/Log.h"

namespace Mirage::VisualComponents
{
	int InputField::ID = 0;
	
	// ----------------------------------------------------------------------------- //
	Float::Float(float* value, float min, float max, float step)
		: m_Value(value), m_Min(min), m_Max(max), m_Step(step)
	{
		if (m_Value == nullptr)
			m_Value = new float(0.0f);
		
	}
	void Float::Draw(float scale)
	{
		ImGui::DragFloat(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max,"%.5g");
	}
	
	Float2::Float2(float value[2], float min, float max, float step)
		: m_Min(min), m_Max(max), m_Step(step)
	{
		
		if (value == nullptr)
			m_Value[0] = m_Value[1] = 0.0f;
		else
		{
			m_Value[0] = value[0];
			m_Value[1] = value[1];
		}
	}
	void Float2::Draw(float scale)
	{
		ImGui::DragFloat2(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max,"%.5g");
	}

	Float3::Float3(float value[3], float min, float max, float step)
		: m_Min(min), m_Max(max), m_Step(step)
	{
		if (value == nullptr)
			m_Value[0] = m_Value[1] = m_Value[2] = 0.0f;
		else
		{
			m_Value[0] = value[0];
			m_Value[1] = value[1];
			m_Value[2] = value[2];
		}
	}
	void Float3::Draw(float scale)
	{
		ImGui::DragFloat3(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max,"%.5g");
	}

	Float4::Float4(float value[4], float min, float max, float step)
		: m_Min(min), m_Max(max), m_Step(step)
	{
		if (value == nullptr)
			m_Value[0] = m_Value[1] = m_Value[2] = m_Value[3] = 0.0f;
		else
		{
			m_Value[0] = value[0];
			m_Value[1] = value[1];
			m_Value[2] = value[2];
			m_Value[3] = value[3];
		}
	}
	void Float4::Draw(float scale)
	{
		ImGui::DragFloat4(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max,"%.5g");
	}
	
	// ----------------------------------------------------------------------------- //
	Int::Int(int* value, int min, int max, int step)
		: m_Value(value), m_Min(min), m_Max(max), m_Step(step)
	{
		if (m_Value == nullptr)
			m_Value = new int(0);
	}
	void Int::Draw(float scale)
	{
		ImGui::DragInt(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max);
	}

	Int2::Int2(int value[2], int min, int max, int step)
		: m_Min(min), m_Max(max), m_Step(step)
	{
		if (value == nullptr)
			m_Value[0] = m_Value[1] = 0;
		else
		{
			m_Value[0] = value[0];
			m_Value[1] = value[1];
		}
	}
	void Int2::Draw(float scale)
	{
		ImGui::DragInt2(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max);
	}

	Int3::Int3(int value[3], int min, int max, int step)
		: m_Min(min), m_Max(max), m_Step(step)
	{
		if (value == nullptr)
			m_Value[0] = m_Value[1] = m_Value[2] = 0;
		else
		{
			m_Value[0] = value[0];
			m_Value[1] = value[1];
			m_Value[2] = value[2];
		}
	}
	void Int3::Draw(float scale)
	{
		ImGui::DragInt3(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max);
	}

	Int4::Int4(int value[4], int min, int max, int step)
		: m_Min(min), m_Max(max), m_Step(step)
	{
		if (value == nullptr)
			m_Value[0] = m_Value[1] = m_Value[2] = m_Value[3] = 0;
		else
		{
			m_Value[0] = value[0];
			m_Value[1] = value[1];
			m_Value[2] = value[2];
			m_Value[3] = value[3];
		}
	}
	void Int4::Draw(float scale)
	{
		ImGui::DragInt4(m_Label.c_str(), m_Value, m_Step, m_Min, m_Max);
	}
	
	// ----------------------------------------------------------------------------- //
	Boolean::Boolean(bool* value)
		: m_Value(value)
	{
		if (m_Value == nullptr)
			m_Value = new bool(false);
	}
	void Boolean::Draw(float scale)
	{
		ImGui::Checkbox(m_Label.c_str(), m_Value);
	}
	
	// ----------------------------------------------------------------------------- //
	ImGuiInputTextFlags TextInput::m_Flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackResize;
	TextInput::TextInput(const char* value, int size)
	{
		if (value == nullptr)
		{
			m_Size = 128;
			m_Value = new char[size];
			memset(m_Value, 0, size);
		}
		else
		{
			m_Value = new char[strlen(value) + 1];
			m_Size = strlen(value) + 1;
			strcpy_s(m_Value, m_Size, value);
		}
	}
	void TextInput::Draw(float scale)
	{
		ImGui::InputText(m_Label.c_str(), m_Value, m_Size, m_Flags, [](ImGuiInputTextCallbackData* data) -> int
			{
				TextInput* input = (TextInput*)data->UserData;
				if (data->EventFlag == ImGuiInputTextFlags_CallbackResize && std::strlen(input->m_Value) >= input->m_Size)
				{
					char* newBuffer = new char[data->BufTextLen + 128];
					strcpy_s(newBuffer, data->BufTextLen + 128, input->m_Value);
					delete[] input->m_Value;
					input->m_Size = data->BufTextLen + 128;
					input->m_Value = newBuffer;
					MRG_CORE_WARN("Text input buffer resized to {0}", input->m_Size);
				}
				return 0;
			},
			this);
	}
	
	ImGuiInputTextFlags TextArea::m_Flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackResize;
	TextArea::TextArea(const char* value, int size)
	{
		if (value == nullptr)
		{
			m_Size = 128;
			m_Value = new char[size];
			memset(m_Value, 0, size);
		}
		else
		{
			m_Value = new char[strlen(value) + 1];
			m_Size = strlen(value) + 1;
			strcpy_s(m_Value, m_Size, value);
		}
	}
	void TextArea::Draw(float scale)
	{
		ImGui::InputTextMultiline(m_Label.c_str(), m_Value, m_Size, ImVec2(0, 60), m_Flags, [](ImGuiInputTextCallbackData* data) -> int
			{
				TextArea* input = (TextArea*)data->UserData;
				if (data->EventFlag == ImGuiInputTextFlags_CallbackResize && std::strlen(input->m_Value) >= input->m_Size)
				{
					char* newBuffer = new char[data->BufTextLen + 128];
					strcpy_s(newBuffer, data->BufTextLen + 128, input->m_Value);
					delete[] input->m_Value;
					input->m_Size = data->BufTextLen + 128;
					input->m_Value = newBuffer;
					MRG_CORE_WARN("Text input buffer resized to {0}", input->m_Size);
				}
				return 0;
			},
			this);
	}
}
