#ifndef VECTOR2_H
  #define VECTOR2_H

  typedef struct {
    double x, y;
  } Vec2;

  static inline Vec2 Vec2_new(double x, double y);

  Vec2 Vec2_add(Vec2 v1, Vec2 v2);
  Vec2 Vec2_sub(Vec2 v1, Vec2 v2);
  Vec2 Vec2_scale(Vec2 v, double scalar);

  double Vec2_dot(Vec2 v1, Vec2 v2);
  double Vec2_cross(Vec2 v1, Vec2 v2);

  double Vec2_magSq(Vec2 v);
  double Vec2_mag(Vec2 v);

  Vec2 Vec2_normalize(Vec2 v);

  void Vec2_print(Vec2 v);

  double Vec2_dist(Vec2 v1, Vec2 v2);

#endif
