#ifndef _ENG_VECTORMATH_H_
#define _ENG_VECTORMATH_H_

#include <genesis.h> // For s16, u16, fix16, fix32, maths.h types and functions

// Initialization
static inline Vect2D_f16 V2D_f16(fix16 x, fix16 y) {    
    Vect2D_f16 vec2d = {x, y};
    return vec2d;
}

// Initialization
static inline Vect2D_f16 V2D_f16_zero() {
    return (Vect2D_f16){FIX16(0), FIX16(0)};
}

// Basic Arithmetic
static inline Vect2D_f16 V2D_f16_add(Vect2D_f16 a, Vect2D_f16 b) {
    return V2D_f16(a.x + b.x, a.y + b.y);
}

static inline Vect2D_f16 V2D_f16_sub(Vect2D_f16 a, Vect2D_f16 b) {
    return V2D_f16(a.x - b.x, a.y - b.y);
}

static inline Vect2D_f16 V2D_f16_mul_scalar(Vect2D_f16 v, fix16 s) {
    return V2D_f16(F16_mul(v.x, s), F16_mul(v.y, s));
}

static inline Vect2D_f16 V2D_f16_div_scalar(Vect2D_f16 v, fix16 s) {
    // Consider adding a check for s == 0 if necessary, or ensure s is never zero
    return V2D_f16(F16_div(v.x, s), F16_div(v.y, s));
}

static inline Vect2D_f16 V2D_f16_negate(Vect2D_f16 v) {
    return V2D_f16(0 - v.x, 0 - v.y);
}

// Magnitude / Length
static inline fix16 V2D_f16_magnitude_sq(Vect2D_f16 v) {
    // (x*x) + (y*y)
    return F16_mul(v.x, v.x) + F16_mul(v.y, v.y);
}

static inline fix16 V2D_f16_magnitude(Vect2D_f16 v) {
    return F16_sqrt(V2D_f16_magnitude_sq(v));
}

// // Normalization
static inline Vect2D_f16 V2D_f16_normalize(Vect2D_f16 v) {
    fix16 mag = V2D_f16_magnitude(v);
    if (mag == FIX16(0)) { // Or very close to zero to avoid division by zero
        return V2D_f16(FIX16(0), FIX16(0));
    }
    return V2D_f16_div_scalar(v, mag);
}

// Dot Product
static inline fix16 V2D_f16_dot(Vect2D_f16 a, Vect2D_f16 b) {
    return F16_mul(a.x, b.x) + F16_mul(a.y, b.y);
}

// Distance
static inline fix16 V2D_f16_distance_sq(Vect2D_f16 a, Vect2D_f16 b) {
    Vect2D_f16 diff = V2D_f16_sub(a, b);
    return V2D_f16_magnitude_sq(diff);
}

static inline fix16 V2D_f16_distance(Vect2D_f16 a, Vect2D_f16 b) {
    return F16_sqrt(V2D_f16_distance_sq(a,b));
}

#endif // _ENG_VECTORMATH_H_
