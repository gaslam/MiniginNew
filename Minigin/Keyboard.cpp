#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Keyboard.h"

#include <algorithm>
#include <vector>
#include <SDL.h>
#include <vector>
#include <functional>

#include "Logger.h"


class Keyboard::KeyboardImpl
{
public:
	explicit KeyboardImpl()
	{
		m_CurrentKeys.resize(SDL_NUM_SCANCODES);
		m_OldKeys.resize(SDL_NUM_SCANCODES);
	}

	void UpdateOldKeys()
	{
		m_OldKeys.assign(m_CurrentKeys.begin(), m_CurrentKeys.end());
	}
	void SetCurrentKey(unsigned int index, bool value)
	{
		if(index >= SDL_NUM_SCANCODES)
		{
			MG_ASSERT_WARNING(index < 0 || index >= SDL_NUM_SCANCODES,"Cannot press key!! key must be valid SDL_SCANCODE!!")
				return;
		}
		m_CurrentKeys[index] = value;
	}

	bool IsButtonDown(unsigned int button) const { return m_CurrentKeys[button] && !m_OldKeys[button]; }
	bool IsUpThisFrame(unsigned int button) const { return !m_CurrentKeys[button] && m_OldKeys[button]; }
	bool IsPressed(unsigned int button) const { return m_CurrentKeys[button]; }
private:

	std::vector<bool> m_CurrentKeys{};
	std::vector<bool> m_OldKeys{};
};

Keyboard::Keyboard() : m_pImpl(new KeyboardImpl())
{
}

Keyboard::~Keyboard()
{
	delete m_pImpl;
}

void Keyboard::UpdateOldKeys() const
{
	m_pImpl->UpdateOldKeys();
}

void Keyboard::SetKey(unsigned int index, bool value) const
{
	m_pImpl->SetCurrentKey(index, value);
}

bool Keyboard::IsButtonDown(unsigned int button) const
{
	return m_pImpl->IsButtonDown(button);
}

bool Keyboard::IsUpThisFrame(unsigned int button) const
{
	return m_pImpl->IsUpThisFrame(button);
}

bool Keyboard::IsPressed(unsigned int button) const
{
	return m_pImpl->IsPressed(button);
}
