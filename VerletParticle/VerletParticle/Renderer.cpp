#include <SFML/Graphics.hpp>
#include <iostream>
#include "Renderer.h"
#include "Physics.h"
void Renderer::render(const Physics& p) const
{
	// Bounds
	const sf::Vector3f constraint = p.getConstraint();
	sf::CircleShape constraintBackground{ constraint.z };
	constraintBackground.setOrigin(constraint.z, constraint.z);
	constraintBackground.setFillColor(sf::Color::Black);
	constraintBackground.setPosition(constraint.x, constraint.y);
	constraintBackground.setPointCount(64);
	m_target.draw(constraintBackground);

	sf::CircleShape circle{1.0f};
	circle.setPointCount(32);
	circle.setOrigin(1.0f, 1.0f);
	const auto& particlesUnits = p.getParticles();
	for (const auto&p: particlesUnits)
	{
		//std::cout << "particle";
		circle.setPosition(p.position);
		circle.setScale(p.radius, p.radius);
		circle.setFillColor(p.colour);
		m_target.draw(circle);
	}
}
