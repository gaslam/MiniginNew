#pragma once
#include <SDL_scancode.h>

class Keyboard
{

public:
	explicit Keyboard();
	~Keyboard();
	Keyboard(const Keyboard& other) = delete;
	Keyboard(Keyboard&& other)noexcept = delete;
	Keyboard& operator=(const Keyboard& other) = delete;
	Keyboard& operator=(Keyboard&& other) noexcept = delete;
	using Key = SDL_Scancode;

	void UpdateOldKeys() const;
	void SetKey(unsigned int index, bool value) const;

	bool IsButtonDown(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	class KeyboardImpl;
	KeyboardImpl* m_pImpl;
};

