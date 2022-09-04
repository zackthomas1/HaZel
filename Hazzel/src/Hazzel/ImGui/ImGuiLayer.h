#pragma once

#include "Hazzel/Layer.h"

namespace Hazzel {

	class HAZZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(); 
		~ImGuiLayer(); 

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;

	private: 
		float m_Time = 0.f;
	};
}