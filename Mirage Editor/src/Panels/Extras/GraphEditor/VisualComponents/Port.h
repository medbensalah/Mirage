﻿#pragma once
#include "Container.h"
#include "InputField.h"
#include "VisualComponentsInclude.h"
#include "Mirage/Renderer/Texture.h"
#include "../Node.h"

namespace Mirage::VisualComponents
{
	enum class PortDataType
	{
		Bool,
		
		Int,
		Int2,
		Int3,
		Int4,
		
		Float,
		Float2,
		Float3,
		Float4,
		
		Color,
		
		String,
	};

	enum class PortType
	{
		Input,
		Output
	};

	class Port : public Container
	{
	public:
		Port(Graph::Node* owner, PortDataType dataType, PortType portType, Ref<Float4> data);
		virtual ~Port() override {
			m_Components.clear();
			m_Links.clear();
		}
		
		void DrawPort(float scale);
		void Draw(float scale) override;

		Ref<InputField> Add(InputField* component)
		{
			Ref<InputField> componentPtr(component);
			m_Components.push_back(componentPtr);
			return componentPtr;
		}

		Ref<InputField> Add(Ref<InputField> component)
		{
			m_Components.push_back(std::move(component));
			return component;
		}
		
		void Remove(Ref<VisualComponent> component);
		void Remove(Ref<InputField> component)
		{
			for (auto it = m_Components.begin(); it != m_Components.end(); it++)
			{
				if (*it == component)
				{
					m_Components.erase(it);
					break;
				}
			}
		}

		void OnBeginDrag(float scale);
		void OnDrag(float scale);
		void OnEndDrag(float scale);

		ImVec2 GetPosition() { return m_PortPos; }
		PortDataType GetDataType() { return m_DataType; }
		PortType GetPortType() { return m_PortType; }

		Ref<InputField> GetData() { return m_data; }
		void SetData(Ref<InputField> data)
		{
			Remove(m_data);
			m_data = data;
			Add(m_data);
		}
		
	private:
		Graph::Node* m_Owner;
		Ref<InputField> m_data;

		PortDataType m_DataType;
		PortType m_PortType;

		ImColor m_Color;
		ImVec2 m_PortPos = ImVec2(0, 0);
		
		std::vector<Ref<Link>> m_Links {};

		bool m_Dragging = false;
		
		static Ref<Texture2D> s_PortTexture;
		static Port* s_HoveredPort;
		static bool Initialized;
		static std::unordered_map<PortDataType, ImColor> s_Colors;
	};
}
