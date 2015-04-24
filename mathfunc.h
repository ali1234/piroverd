#ifndef PIROVERD_MATHFUNC_H
#define PIROVERD_MATHFUNC_H

static inline float squared_length(float *v)
{
    return (v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2]);
}

static inline float length(float *v)
{
    return sqrtf(squared_length(v));
}

static inline float dot_product(float *u, float *v)
{
    return (u[0]*v[0]) + (u[1]*v[1]) + (u[2]*v[2]);
}

static inline void cross_product(float *u, float *v, float *r)
{
    r[0] = u[1]*v[2] - u[2]*v[1];
    r[1] = u[2]*v[0] - u[0]*v[2];
    r[2] = u[0]*v[1] - u[1]*v[0];
}

static inline void projection(float *u, float *v, float *r)
{
    float costheta = dot_product(u, v) / squared_length(v);
    r[0] = v[0]*costheta;
    r[1] = v[1]*costheta;
    r[2] = v[2]*costheta;
}

static inline void subtract(float *u, float *v, float *r)
{
    r[0] = u[0] - v[0];
    r[1] = u[1] - v[1];
    r[2] = u[2] - v[2];
}

static inline void normalise(float *v)
{
    float scale = 1.0 / length(v);
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

static inline float unsigned_angle_between(float *u, float *v)
{
    return acos(dot_product(u, v) / (length(u) * length(v)));
}

static inline float angle_between(float *u, float *v, float *n)
{
    float angle = unsigned_angle_between(u, v);
    float cross[3];
    cross_product(u, v, cross);
    if(dot_product(n, cross) > 0) angle *= -1;
    return angle;
}

#endif // PIROVERD_MATHFUNC_H
