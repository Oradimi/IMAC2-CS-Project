#ifndef DC6B995E_1F37_4D4A_B718_3BB0C9E76360
#define DC6B995E_1F37_4D4A_B718_3BB0C9E76360

#include <p6/p6.h>
#include <vector>

class Boid {
public:
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec2 acceleration;
  float radius = 50.0f;
  static float max_speed;
  static float max_force;
  static float separation_weight;
  static float alignment_weight;
  static float cohesion_weight;

  Boid(glm::vec2 init_position, glm::vec2 init_velocity);

  void update_position(float frame_duration);
  void apply_bounds(const glm::vec2 &top_left, const glm::vec2 &bottom_right);
  glm::vec2 separation(const std::vector<Boid> &boids);
  glm::vec2 alignment(const std::vector<Boid> &boids);
  glm::vec2 cohesion(const std::vector<Boid> &boids);
  void update_acceleration(const std::vector<Boid> &boids);
};

class BoidsSystem {
public:
  std::vector<Boid> boids;
  glm::vec2 top_left_bound;
  glm::vec2 bottom_right_bound;

  BoidsSystem(int num_boids, glm::vec2 top_left, glm::vec2 bottom_right);

  void update(float frame_duration);
  void draw(p6::Context &ctx) const;
  void resize(int new_size, glm::vec2 top_left, glm::vec2 bottom_right);
};

#endif /* DC6B995E_1F37_4D4A_B718_3BB0C9E76360 */
