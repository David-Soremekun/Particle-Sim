#pragma once
#include <SFML/Graphics.hpp>
class Physics;
class Renderer
{
private:
	sf::RenderTarget& m_target;
public:
	explicit
		Renderer(sf::RenderTarget& target) :
		m_target{ target }
	{

	}

	void render(const Physics& p) const;
};

