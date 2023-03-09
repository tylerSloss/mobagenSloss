#include "Pacticle.h"
#include "Polygon.h"

Particle::Particle(Engine* pEngine, float size, Color32 color)
    : GameObject(pEngine), velocity(Vector2::zero()), circleSize(size), color(color), acceleration(Vector2::zero()) {
  polygon.points = {{0, -2}, {1, 1}, {0, 0}, {-1, 1}};
  transform.scale = {2, 2};
}

// Public Methods
void Particle::applyForce(Vector2 force) { acceleration += force; }

void Particle::Update(const float deltaTime) {
  // Has a max acceleration per frame
  if (Vector2::getMagnitude(acceleration) > maxAcceleration) acceleration = Vector2::normalized(acceleration) * maxAcceleration;

  // Apply acceleration to velocity
  setVelocity(velocity + acceleration);
  resetAcceleration();

  // constant speed never change || else only ceil the speed if wanted
  if (hasConstantSpeed || Vector2::getMagnitude(velocity) > speed) setVelocity(Vector2::normalized(velocity) * speed);

  transform.position += velocity * deltaTime;
}

void Particle::OnDraw(SDL_Renderer* renderer) {
  // todo: draw a circle or the ship?
  polygon.Draw(renderer, transform, color);

  if (drawAcceleration) {
    auto pos = getPosition();
    Polygon::DrawLine(renderer, pos, pos + previousAcceleration * 2.f, Color::Purple);
  }
}

void Particle::resetAcceleration() {
  previousAcceleration = acceleration;
  acceleration = Vector2::zero();
}