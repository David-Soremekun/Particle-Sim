// VerletParticle.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>
#include "Physics.h"
#include "Renderer.h"


int main()
{
    int width{ 1080 };
    int height{ 720 };
    const uint32_t frameRate = 60;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1;

    sf::RenderWindow win(sf::VideoMode(width, height), "SFML Particle Sim C++", sf::Style::Default, settings);
    win.setFramerateLimit(frameRate);

    std::cout << "Window Created!\n";

    Physics physicsSolver;
    Renderer renderer{win};

    physicsSolver.setConstraint({ static_cast<float>(width) * 0.5f,static_cast<float>(height) * 0.5f }, 300.0f);
    physicsSolver.setSubstepCount(8);
    physicsSolver.setSimUpdateRate(frameRate);

    const float spawnDelay = 0.50f;
    const float spawnSpeed = 20.0f;
    const sf::Vector2f spawnPosition = { 500.0f,200.0f };
    const int minRad = 5;
    const int maxRad = 15;
    const uint32_t particleCount = 400;
    const float maxAng = 1.0f;



    sf::Clock clock;
    while (win.isOpen()) {
        sf::Event events {};
        while (win.pollEvent(events)) {
            switch (events.type) {
            case sf::Event::Closed:
                win.close();
            default:
                break;
            }
        }

        if (physicsSolver.getParticleCount() < particleCount && clock.getElapsedTime().asSeconds() >= spawnDelay) {
            clock.restart();
            auto& particle = physicsSolver.addObject(spawnPosition, rand() % maxRad + minRad);
            const float t = physicsSolver.getTime();
            const float angle = maxAng * sin(t) + PI * 0.5f;
            physicsSolver.setAngle(particle, angle);
            physicsSolver.setParticleVelocity(particle, spawnSpeed * sf::Vector2f{cos(angle), sin(angle)});
            particle.colour = /*getRainbow(t)*/sf::Color(rand() % 100 + 155, rand() % 100 + 155, rand() % 100 + 155, 255);
        }

        physicsSolver.Update();
        win.clear(sf::Color::Black);
        renderer.render(physicsSolver);
            win.display();

    }
    return 0;
}
