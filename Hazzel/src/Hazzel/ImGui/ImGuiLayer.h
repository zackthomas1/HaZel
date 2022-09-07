#pragma once

#include "Hazzel/Layer.h"

#include "Hazzel/Events/ApplicationEvent.h"
#include "Hazzel/Events/KeyEvent.h"
#include "Hazzel/Events/MouseEvent.h"

namespace Hazzel {

	class HAZZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(); 
		~ImGuiLayer(); 

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin(); 
		void End();

	private: 
		float m_Time = 0.f;
	};
}