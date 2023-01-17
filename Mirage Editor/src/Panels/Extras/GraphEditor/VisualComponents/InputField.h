#pragma once
#include <string>

#include "VisualComponent.h"
#include "Mirage/Math/glmTypes.h"

namespace Mirage::VisualComponents
{
	class InputField : public VisualComponent
	{
	public:
		InputField() {
			m_ID = ID++;
			m_Label = "##" + std::to_string(m_ID);
		}
		
		virtual ~InputField() override {}
		
		void Draw(float scale) override {};

		void SetVisible(bool visible) { m_Visible = visible; }
		bool IsVisible() const { return m_Visible; }

		static int ID;
	protected:
		int m_ID;
		bool m_Visible;
		std::string m_Label;
	};

	class Float : public InputField
	{
	public:
		Float(float* value = nullptr, float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float() override {};

		void Draw(float scale) override;

	private:
		float* m_Value;
		float m_Min;
		float m_Max;
		float m_Step;
	};
	class Float2 : public InputField
	{
	public:
		Float2(float value[2] = nullptr, float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float2() override {};

		void Draw(float scale) override;

	private:
		float m_Value[2];
		float m_Min;
		float m_Max;
		float m_Step;
	};
	class Float3 : public InputField
	{
	public:
		Float3(float value[3] = nullptr, float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float3() override {};

		void Draw(float scale) override;

	private:
		float m_Value[3];
		float m_Min;
		float m_Max;
		float m_Step;
	};
	class Float4 : public InputField
	{
	public:
		Float4(float value[4] = nullptr, float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float4() override {};

		void Draw(float scale) override;

	private:
		float m_Value[4];
		float m_Min;
		float m_Max;
		float m_Step;
	};

	class Int : public InputField
	{
	public:
		Int(int* value = nullptr, int min = 0, int max = 0, int step = 1);
		virtual ~Int() override {};

		void Draw(float scale) override;

	private:
		int* m_Value;
		int m_Min;
		int m_Max;
		int m_Step;
	};
	class Int2 : public InputField
	{
	public:
		Int2(int value[2] = nullptr, int min = 0, int max = 0, int step = 1);
		virtual ~Int2() override {};

		void Draw(float scale) override;

	private:
		int m_Value[2];
		int m_Min;
		int m_Max;
		int m_Step;
	};
	class Int3 : public InputField
	{
	public:
		Int3(int value[3] = nullptr, int min = 0, int max = 0, int step = 1);
		virtual ~Int3() override {};

		void Draw(float scale) override;

	private:
		int m_Value[3];
		int m_Min;
		int m_Max;
		int m_Step;
	};
	class Int4 : public InputField
	{
	public:
		Int4(int value[4] = nullptr, int min = 0, int max = 0, int step = 1);
		virtual ~Int4() override {};

		void Draw(float scale) override;

	private:
		int m_Value[4];
		int m_Min;
		int m_Max;
		int m_Step;
	};

	class Boolean : public InputField
	{
	public:
		Boolean(bool* value = nullptr);
		virtual ~Boolean() override {};

		void Draw(float scale) override;

	private:
		bool* m_Value;
	};
	
	class TextInput : public InputField
	{
	public:
		TextInput(const char* value = nullptr, int size = 512);
		virtual ~TextInput() override {};

		void Draw(float scale) override;
		
	private:
		char* m_Value;
		size_t m_Size;
		static ImGuiInputTextFlags m_Flags;
	};
	class TextArea : public InputField
	{
	public:
		TextArea(const char* value = nullptr, int size = 512);
		virtual ~TextArea() override {};

		void Draw(float scale) override;
		
	private:
		char* m_Value;
		size_t m_Size;
		static ImGuiInputTextFlags m_Flags;
	};
}
