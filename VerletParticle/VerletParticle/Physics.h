#pragma once
#include <SFML/Graphics.hpp>
#include "ExternalForces.h"
#include <vector>
#include <cmath>
#include <iostream>


struct Particle {
	
	float radius = 10.0f;
	float m_angle = 0.0f;
	float speed;
	sf::Vector2f position;
	sf::Vector2f prev_position;
	sf::Vector2f acceleration;

	sf::Color colour = sf::Color::White;
	
	Particle() = default;
	Particle(sf::Vector2f pos, float rad) :
		position{ pos },
		prev_position{ pos },
		radius{ rad },
		acceleration{ 0.0f,0.0f }
	{}

	void Update(float dt) {
		const sf::Vector2f displacement = position - prev_position;

		prev_position = position;
		position = position + displacement + acceleration * (dt * dt);

		acceleration = { 0.0f,0.0f };
	}
	void setAngle(float angle)
	{
		m_angle = angle;
	}

	void setVelocity(sf::Vector2f v, float dt)
	{
		speed = sqrtf((v.x * v.x) + (v.y * v.y));
		prev_position = position - (v * dt);
	}
	sf::Vector2f getVelocity(float dt) const
	{
		return (position - prev_position) / dt;
	}

	void addVelocity(sf::Vector2f v, float dt)
	{
		prev_position -= v * dt;
	}
	void Accelerate(sf::Vector2f a) {
		acceleration += a;
	}
};


class Physics
{
private:
	uint32_t p_sim_steps = 1;

	sf::Vector2f m_bounding_center;
	sf::Vector2f m_gravity = { 0.0f,9.81f };
	sf::Vector2f m_drag = { 0.0f,0.0f };
	sf::Vector2f m_wind = { 0.0f,0.0f };
	std::vector<Particle> m_units;
	
	float f_drag;
	float m_time = 0.0f;
	float m_frame_dt = 0.0f;
	float m_constraint_radius=50.0f;
	float m_speed;

	void applyWind() {
		for (auto& particles : m_units)
		{
			m_wind.x = 0.5f * _AIRDENSITY * _WINDSPEED * _WINDSPEED * (PI * particles.radius * particles.radius) * _DRAGCOEFFICIENT;

			particles.Accelerate(m_wind);
		}
	}
	void applyDrag() {
		for (auto& particles : m_units)
		{
			m_drag -= particles.getVelocity(1.0f);
			Normalize(m_drag);

			m_speed = Magnitude(particles.getVelocity(1.0f));
			f_drag = 0.5f * _AIRDENSITY * m_speed * m_speed * (PI * particles.radius * particles.radius) * _DRAGCOEFFICIENT;

			m_drag *= f_drag;
			particles.Accelerate(m_drag);

		}
	}
	void applyGravity() {
		for (auto& particles: m_units)
		{
			particles.Accelerate(m_gravity);

		}
	}
	void updateParticles(float dt) {
		for (auto&particle : m_units)
		{
			particle.Update(dt);
		}
	}
	void boundingConstraints() {
		for (auto&p :m_units) {
			const sf::Vector2f v;
		}
	}
	
	void applyConstraint() {
		for (auto& p: m_units) {
			const sf::Vector2f v = m_bounding_center - p.position;
			const float dist = Magnitude(v);
			if (dist>(m_constraint_radius-p.radius)){
				const sf::Vector2f n = v / dist;
				p.position = m_bounding_center - n * (m_constraint_radius - p.radius);
			}
		}
	}
	void checkCollision(float dt) {
		
		const uint64_t particleCount = m_units.size();
		//for (uint64_t i{ 0 }; i < particleCount;++i) 
		//{
		//	Particle& p1=m_units[i];
		//	for (uint64_t j{ i + 1 }; j < particleCount; ++j) {
		//		Particle& p2 =m_units[j];

		//		float dx = p1.position.x - p2.position.x;
		//		float dy = p1.position.y - p2.position.y;
		//		float dist = hypot(dx, dy);
		//		if (dist<=(p1.radius+p2.radius)) {
		//			//std::cout << "Collision Occured\n";
		//			float tangent = atan2(dx, dy);

		//			float angle = 0.5 * (PI + tangent);
		//			
		//			const float angle1 = 2 * tangent - p1.m_angle;
		//			const float angle2 = 2 * tangent - p2.m_angle;


		//			const float speed1 = Magnitude(p1.getVelocity(dt)) * BOUNCINESS;
		//			const float speed2 = Magnitude(p2.getVelocity(dt)) * BOUNCINESS;

		//			(p1.m_angle, p1.speed) = (angle1, speed1);
		//			(p2.m_angle, p2.speed) = (angle2, speed2);

		//			p1.position.x += sin(angle);
		//			p1.position.y -= cos(angle);
		//			p2.position.x -= sin(angle);
		//			p2.position.y += cos(angle);

		//		}
		//	}
		//}

		
			
		
		
	}

public:
	Physics()=default;

	Particle& addObject(sf::Vector2f position, float radius) 
	{
		m_units.emplace_back(position, radius);
		return m_units.back();
	}

	sf::Vector2f Normalize(sf::Vector2f vec)
	{
		float lengthOfVector = sqrt((vec.x * vec.x) + (vec.y * vec.y));
		vec.x /= lengthOfVector;
		vec.y /= lengthOfVector;

		return sf::Vector2f(vec);
	}


	void Update() {
		m_time += m_frame_dt;
		const float step_dt = getStepDt();
		for (uint32_t i{p_sim_steps}; i--;)
		{
			applyGravity();
			//applyDrag();
			checkCollision(step_dt);
			applyWind();
			applyConstraint();
			updateParticles(step_dt);
		}
	}

	float getTime() const
	{
		return m_time;
	}

	const std::vector<Particle>& getParticles() const 
	{
		return m_units;
	}

	void setParticleVelocity(Particle& particles, sf::Vector2f v) {
		particles.setVelocity(v, getStepDt());
	}
	void setConstraint(sf::Vector2f position, float radius) 
	{
		m_constraint_radius = radius;
		m_bounding_center = position;
	}

	sf::Vector3f getConstraint() const
	{
		return { m_bounding_center.x,m_bounding_center.y,m_constraint_radius };
	}

	uint32_t getParticleCount() const
	{
		return m_units.size();
	}
	void setAngle(Particle& particles, float angle)
	{
		particles.setAngle(angle);
	}
	float Magnitude(sf::Vector2f vec)
	{
		return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
	}
	float getStepDt() const
	{
		return m_frame_dt / static_cast<float>(p_sim_steps);
	}
	void setSimUpdateRate(uint32_t rate) {
		m_frame_dt = 1.0f / static_cast<float>(rate);
	}
	void setSubstepCount(uint32_t subSteps) 
	{
		p_sim_steps = subSteps;
	}

};

