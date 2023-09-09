#include "mathUtil.glsl"
//Functions
void math_clamp(float a, float b, float c, out float d)
{
    d = clamp(a, b, c);
}
//The sum of the two values.
void math_add(float a, float b, out float c)
{
    c = a + b;
}
//The difference between the two values.
void math_subtract(float a, float b, out float c)
{
    c = a - b;
}
//The product of the two values.
void math_multiply(float a, float b, out float c)
{
    c = a * b;
}
//The division of the first value by the second value.
void math_divide(float a, float b, out float c)
{
    c = safe_divide(a, b);
}
//The sum of the product of the two values with Addend.
void math_multiply_add(float a, float b, float c)
{
    c = a * b + c;
}
//The Base raised to the power of Exponent.
void math_power(float a, float b, out float c)
{
    if (a >= 0.0)
    {
        c = compatible_pow(a, b);
    }
    else
    {
        float fraction = mod(abs(b), 1.0);
        if (fraction > 0.999 || fraction < 0.001)
        {
            c = compatible_pow(a, floor(b + 0.5));
        }
        else
        {
            c = 0.0;
        }
    }
}
//The log of the value with a Base as its base.
void math_logarithm(float a, float b, out float c)
{
    c = (a > 0.0 && b > 0.0) ? log2(a) / log2(b) : 0.0;
}
//The square root of the value.
void math_square_root(float a, out float b)
{
    b = (a > 0.0) ? sqrt(a) : 0.0;
}
//One divided by the square root of the value.
void math_inverse_square_root(float a, out float b)
{
    b = inversesqrt(a);
}
//The input value is read without regard to its sign.This turns negative values into positive values.
void math_absolute(float a, out float b)
{
    b = abs(a);
}
//Raises Euler`s number to the power of the value.
void math_exponent(float a, out float b)
{
    b = exp(a);
}

//Comparison
//Outputs the smallest of the input values.
void math_minimum(float a, float b, out float c)
{
    c = min(a, b);
}
//Outputs the largest of two input values.
void math_maximum(float a, float b, out float c)
{
    c = max(a, b);
}
//Outputs 1.0 if the first value is smaller than the second value.Otherwise the output is 0.0.
void math_less_than(float a, float b, out float c)
{
    c = (a < b) ? 1.0 : 0.0;
}
//Outputs 1.0 if the first value is larger than the second value.Otherwise the output is 0.0.
void math_greater_than(float a, float b, out float c)
{
    c = (a > b) ? 1.0 : 0.0;
}
//Extracts the sign of the input value. All positive numbers will output 1.0. All negative numbers will output - 1.0. And 0.0 will output 0.0.
void math_sign(float a, out float b)
{
    b = sign(a);
}
//Outputs 1.0 if the difference between the two input values is less than or equal to Epsilon.
void math_compare(float a, float b, float c, out float d)
{
    d = (abs(a - b) <= max(c, 1e-5)) ? 1.0 : 0.0;
}
//Smooth Minimum.
void math_smooth_minimum(float a, float b, float c, out float d)
{
    if (c != 0.0) {
        float h = max(c - abs(a - b), 0.0) / c;
        d = min(a, b) - h * h * h * c * (1.0 / 6.0);
    }
    else
    {
        d = min(a, b);
    }
}
void math_smooth_maximum(float a, float b, float c, out float d)
{
    float maximum = max(a, b);
    float minimum = min(a, b);
    d = maximum + log(1.0 + exp(minimum - maximum)) * c;
}
//Smooth Maximum.

//Rounding
//Rounds the input value to the nearest integer.
void math_round(float a, out float b)
{
    b = floor(a + 0.5f);
}
//Rounds the input value down to the nearest integer.
void math_floor(float a, out float b)
{
    b = floor(a + 0.5f);
}
//Rounds the input value up to the nearest integer.
void math_ceil(float a, out float b)
{
    b = ceil(a + 0.5f);
}
//Outputs the integer part of the value.
void math_truncate(float a, out float b)
{
    b = trunc(a);
}
//Fraction.
void math_fraction(float a, out float b)
{
    b = a - floor(a);
}
//Outputs the remainder once the first value is divided by the second value.
void math_modulo(float a, float b, out float c)
{
    c = compatible_fmod(a, b);
}
//Outputs a value between Min and Max based on the absolute difference between the input value and the nearest integer multiple of Max less than the value.
void math_wrap(float a, float b, float c, out float d)
{
    d = wrap(a, b, c);
}
void math_snap(float a, float b, out float c)
{
    c = floor(safe_divide(a, b)) * b;
}
//Rounds the input value down to the nearest integer multiple of Increment.
//The output value is moved between 0.0 and the Scale based on the input value.
void math_ping_pong(float a, float b, out float c)
{
    c = (b != 0.0) ? abs(fract((a - b) / (b * 2.0)) * b * 2.0 - b) : 0.0;
}

//Trigonometric
//The Sine of the input value.
void math_sine(float a, out float b)
{
    b = sin(a);
}
//The Cosine of the input value.
void math_cosine(float a, out float b)
{
    b = cos(a);
}
//The Tangent of the input value.
void math_tangent(float a, out float b)
{
    b = tan(a);
}
//The Arcsine of the input value.
void math_arcsine(float a, out float b)
{
    b = asin(a);
}
//The Arccosine of the input value.
void math_arccosine(float a, out float b)
{
    b = acos(a);
}
//The Arctangent of the input value.
void math_arctangent(float a, out float b)
{
    b = atan(a);
}
//Outputs the Inverse Tangent of the first value divided by the second value measured in radians.
void math_arctan2(float a, float b, out float c)
{
    c = atan(a, b);
}
//The Hyperbolic Sine of the input value.
void math_hyperbolic_sine(float a, out float b)
{
    b = sinh(a);
}
//The Hyperbolic Cosine of the input value.
void math_hyperbolic_cosine(float a, out float b)
{
    b = cosh(a);
}
//The Hyperbolic Tangent of the input value.
void math_hyperbolic_tangent(float a, out float b)
{
    b = tanh(a);
}

//Conversion
//Converts the input from degrees to radians.
void math_to_radians(float a, out float b)
{
    b = radians(a);
}
//Converts the input from radians to degrees.
void math_to_degrees(float a, out float b)
{
    b = degrees(a);
}