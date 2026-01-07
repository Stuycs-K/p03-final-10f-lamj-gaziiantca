#include "vector2.h"

#include <math.h>
#include <stdio.h>

static inline Vec2 Vec2_new(double x, double y) {
  return (Vec2) {x, y};
}

Vec2 Vec2_add(Vec2 v1, Vec2 v2) {
  return (Vec2) {v1.x + v2.x, v1.y + v2.y};
}
Vec2 Vec2_sub(Vec2 v1, Vec2 v2) {
  return (Vec2) {v1.x - v2.x, v1.y - v2.y};
}
Vec2 Vec2_scale(Vec2 v, double scalar) {
  return (Vec2) {v.x * scalar, v.y * scalar};
}

double Vec2_dot(Vec2 v1, Vec2 v2) { return v1.x * v2.x + v1.y * v2.y; }
double Vec2_cross(Vec2 v1, Vec2 v2) { return v1.x * v2.y - v1.y * v2.x; }

double Vec2_magSq(Vec2 v) { return v.x * v.x + v.y * v.y; }
double Vec2_mag(Vec2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

Vec2 Vec2_normalize(Vec2 v) {
  double mag = Vec2_mag(v);
  if (mag > 0) {
    return Vec2_scale(v, 1.0f / mag);
  }
  return (Vec2) {0, 0};
}

void Vec2_print(Vec2 v) { printf("(%.2f, %.2f)\n", v.x, v.y); }

double Vec2_dist(Vec2 v1, Vec2 v2) {
  return Vec2_mag(Vec2_sub(v1, v2));
}
