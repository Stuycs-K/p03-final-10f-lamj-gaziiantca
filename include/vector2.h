#ifndef VECTOR2_H
  #define VECTOR2_H

  typedef struct {
    double x, y;
  } Vector2;

  static inline Vector2 Vector2_new(double x, double y);

  Vector2 Vector2_add(Vector2 v1, Vector2 v2);
  Vector2 Vector2_sub(Vector2 v1, Vector2 v2);
  Vector2 Vector2_scale(Vector2 v, double scalar);

  double Vector2_dot(Vector2 v1, Vector2 v2);
  double Vector2_cross(Vector2 v1, Vector2 v2);

  double Vector2_magSq(Vector2 v);
  double Vector2_mag(Vector2 v);

  Vector2 Vector2_normalize(Vector2 v);

  void Vector2_print(Vector2 v);

  double Vector2_dist(Vector2 v1, Vector2 v2);

#endif