#pragma once
#include <string>

#include "VisualComponent.h"
#include "Mirage/Core/Core.h"
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

	class Color : public InputField
	{
	public:
		Color(ImColor color) : m_Color(color) {}
		virtual ~Color() override {}

		void Draw(float scale) override;
		
	private:
		ImColor m_Color;

		friend class Port;
	};
	
	class Float : public InputField
	{
	public:
		Float(float value, float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float() override {}

		void Draw(float scale) override;

	private:
		float m_Value_x;
		float m_Min;
		float m_Max;
		float m_Step;
		
		friend class Port;
	};
	class Float2 : public InputField
	{
	public:
		Float2(float value[2], float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float2() override {}

		void Draw(float scale) override;

	private:
		float m_Value_x;
		float m_Value_y;
		float m_Min;
		float m_Max;
		float m_Step;
		
		friend class Port;
	};
	class Float3 : public InputField
	{
	public:
		Float3(float value[3], float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float3() override {}

		void Draw(float scale) override;

	private:
		float m_Value_x;
		float m_Value_y;
		float m_Value_z;
		float m_Min;
		float m_Max;
		float m_Step;
		
		friend class Port;
	};
	class Float4 : public InputField
	{
	public:
		Float4(float value[4], float min = 0, float max = 0, float step = 0.1f);
		virtual ~Float4() override {}

		void Draw(float scale) override;

	private:
		float m_Value_x;
		float m_Value_y;
		float m_Value_z;
		float m_Value_w;
		float m_Min;
		float m_Max;
		float m_Step;
		
		friend class Port;
	};
	// class FloatArray : public InputField
	// {
	// public:
	// 	FloatArray(float* value, int size, float min = 0, float max = 0, float step = 0.1f);
	// 	virtual ~FloatArray() override {}
	//
	// 	void Draw(float scale) override;
	//
	// private:
	// 	float m_Value[];
	// 	int m_Size;
	// 	float m_Min;
	// 	float m_Max;
	// 	float m_Step;
	// 	
	// 	friend class Port;
	// };

	
	class Int : public InputField
	{
	public:
		Int(int value, int min = 0, int max = 0, int step = 1);
		virtual ~Int() override {}

		void Draw(float scale) override;

	private:
		int m_Value_x;
		int m_Min;
		int m_Max;
		int m_Step;
		
		friend class Port;
	};
	class Int2 : public InputField
	{
	public:
		Int2(int value[2], int min = 0, int max = 0, int step = 1);
		virtual ~Int2() override {}

		void Draw(float scale) override;

	private:
		int m_Value_x;
		int m_Value_y;
		int m_Min;
		int m_Max;
		int m_Step;
		
		friend class Port;
	};
	class Int3 : public InputField
	{
	public:
		Int3(int value[3], int min = 0, int max = 0, int step = 1);
		virtual ~Int3() override {}

		void Draw(float scale) override;

	private:
		int m_Value_x;
		int m_Value_y;
		int m_Value_z;
		int m_Min;
		int m_Max;
		int m_Step;
		
		friend class Port;
	};
	class Int4 : public InputField
	{
	public:
		Int4(int value[4], int min = 0, int max = 0, int step = 1);
		virtual ~Int4() override {}

		void Draw(float scale) override;

	private:
		int m_Value_x;
		int m_Value_y;
		int m_Value_z;
		int m_Value_w;
		int m_Min;
		int m_Max;
		int m_Step;
		
		friend class Port;
	};

	class Boolean : public InputField
	{
	public:
		Boolean(bool value);
		virtual ~Boolean() override {}

		void Draw(float scale) override;

	private:
		bool m_Value;
		
		friend class Port;
	};
	
	class TextInput : public InputField
	{
	public:
		TextInput(const char* value = nullptr, int size = 512);
		virtual ~TextInput() override { delete[] m_Value; }

		void Draw(float scale) override;
		
	private:
		char* m_Value;
		size_t m_Size;
		static ImGuiInputTextFlags m_Flags;
		
		friend class Port;
	};
	class TextArea : public InputField
	{
	public:
		TextArea(const char* value = nullptr, int size = 512);
		virtual ~TextArea() override { delete[] m_Value; }

		void Draw(float scale) override;
		
	private:
		char* m_Value;
		size_t m_Size;
		static ImGuiInputTextFlags m_Flags;
		
		friend class Port;
	};
}
