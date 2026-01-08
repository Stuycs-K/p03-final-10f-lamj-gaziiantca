#include "vector2.h"

#include <math.h>
#include <stdio.h>

static inline Vector2 Vector2_new(double x, double y) {
  return (Vector2) {x, y};
}

Vector2 Vector2_add(Vector2 v1, Vector2 v2) {
  return (Vector2) {v1.x + v2.x, v1.y + v2.y};
}
Vector2 Vector2_sub(Vector2 v1, Vector2 v2) {
  return (Vector2) {v1.x - v2.x, v1.y - v2.y};
}
Vector2 Vector2_scale(Vector2 v, double scalar) {
  return (Vector2) {v.x * scalar, v.y * scalar};
}

double Vector2_dot(Vector2 v1, Vector2 v2) { return v1.x * v2.x + v1.y * v2.y; }
double Vector2_cross(Vector2 v1, Vector2 v2) { return v1.x * v2.y - v1.y * v2.x; }

double Vector2_magSq(Vector2 v) { return v.x * v.x + v.y * v.y; }
double Vector2_mag(Vector2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

Vector2 Vector2_normalize(Vector2 v) {
  double mag = Vector2_mag(v);
  if (mag > 0) {
    return Vector2_scale(v, 1.0f / mag);
  }
  return (Vector2) {0, 0};
}

void Vector2_print(Vector2 v) { printf("(%.2f, %.2f)\n", v.x, v.y); }

double Vector2_dist(Vector2 v1, Vector2 v2) {
  return Vector2_mag(Vector2_sub(v1, v2));
}