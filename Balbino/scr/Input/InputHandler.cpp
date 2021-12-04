#include "pch.h"
#include "InputHandler.h"

Balbino::CInputHandler::CInputHandler()
	: m_inputsAxis{}
	, m_inputsAxisBinding{}
	, m_mousePosition{}
	, m_mouseRelativePosition{}
{
}

bool Balbino::CInputHandler::Initialize()
{
	ToggleCursor();
	return true;
}

bool Balbino::CInputHandler::Cleanup()
{
	for (auto var : m_inputsAxis)
	{
		delete var.second;
	}
	for (auto var : m_inputsAxisBinding)
	{
		delete var.second;
	}
	m_inputsAxis.clear();
	m_inputsAxisBinding.clear();
	return true;
}

void Balbino::CInputHandler::ProcessEvents(SDL_Event e)
{
	switch (e.type)
	{
	case SDL_KEYDOWN:
		for (auto axis : m_inputsAxis)
		{
			if (axis.second->code == e.key.keysym.sym && e.key.repeat == 0)
			{
				auto it{m_inputsAxisBinding.equal_range(axis.first)};
				for (auto iter{it.first}; iter != it.second; ++iter)
				{
					iter->second->value += axis.second->weight;
				}
			}
		}
		if (e.key.keysym.sym == SDLK_ESCAPE)
			ToggleCursor();
		break;
	case SDL_KEYUP:
		for (auto axis : m_inputsAxis)
		{
			if (axis.second->code == e.key.keysym.sym)
			{
				auto it{m_inputsAxisBinding.equal_range(axis.first)};
				for (auto iter{it.first}; iter != it.second; ++iter)
				{
					iter->second->value -= axis.second->weight;
				}
			}
		}
		break;
	case SDL_MOUSEMOTION:
		m_mousePosition.x = static_cast<float>(e.motion.x);
		m_mousePosition.y = static_cast<float>(e.motion.y);
		m_mouseRelativePosition.x = static_cast<float>(e.motion.xrel);
		m_mouseRelativePosition.y = static_cast<float>(e.motion.yrel);
		break;
	case SDL_MOUSEBUTTONDOWN:
		break;
	case SDL_MOUSEBUTTONUP:
		break;
	case SDL_MOUSEWHEEL:
		break;
	case SDL_MOUSEWHEEL_FLIPPED:
		break;
	case SDL_MOUSEWHEEL_NORMAL:
		break;
	case SDL_JOYAXISMOTION:
		break;
	case SDL_JOYBALLMOTION:
		break;
	case SDL_JOYHATMOTION:
		break;
	case SDL_JOYBUTTONDOWN:
		break;
	case SDL_JOYBUTTONUP:
		break;
	case SDL_JOYDEVICEADDED:
		break;
	case SDL_JOYDEVICEREMOVED:
		break;
	case SDL_CONTROLLERAXISMOTION:
		break;
	case SDL_CONTROLLERBUTTONDOWN:
		break;
	case SDL_CONTROLLERBUTTONUP:
		break;
	case SDL_CONTROLLERDEVICEADDED:
		break;
	case SDL_CONTROLLERDEVICEREMAPPED:
		break;
	case SDL_CONTROLLERDEVICEREMOVED:
		break;
	case SDL_CONTROLLERTOUCHPADDOWN:
		break;
	case SDL_CONTROLLERTOUCHPADMOTION:
		break;
	case SDL_CONTROLLERTOUCHPADUP:
		break;
	case SDL_CONTROLLERSENSORUPDATE:
		break;
	case SDL_FINGERDOWN:
		break;
	case SDL_FINGERUP:
		break;
	case SDL_FINGERMOTION:
		break;
	case SDL_MULTIGESTURE:
		break;
	default: ;
	}
}

void Balbino::CInputHandler::Update()
{
	for (auto axis : m_inputsAxis)
	{
		auto it{m_inputsAxisBinding.equal_range(axis.first)};
		for (auto iter{it.first}; iter != it.second; ++iter)
		{
			iter->second->function(iter->second->value);
		}
	}
	m_mouseRelativePosition.x = m_mouseRelativePosition.y = 0;
}

void Balbino::CInputHandler::AddAxis(const char* pName, SDL_KeyCode code, float weight)
{
	m_inputsAxis.emplace(pName, DBG_NEW CInputAxis{
		                     .name = SDL_GetKeyName(code), .code = static_cast<uint16_t>(code), .weight = weight,
	                     });
}

void Balbino::CInputHandler::GetMousePosition(float& x, float& y) const
{
	x = m_mousePosition.x;
	y = m_mousePosition.y;
}

void Balbino::CInputHandler::GetRelativeMousePosition(float& x, float& y) const
{
	x = m_mouseRelativePosition.x;
	y = m_mouseRelativePosition.y;
}

void Balbino::CInputHandler::ToggleCursor()
{
	if (SDL_SetRelativeMouseMode(m_visible ? SDL_TRUE : SDL_FALSE) != 0)
	{
		throw std::runtime_error(std::string("could not enable Relative Mouse Mode: ") + SDL_GetError());
	}
	m_visible = !m_visible;
}
