#pragma OPENCL EXTENSION cl_khr_fp64 : enable

typedef struct __attribute__ ((packed)) Color
{
    uchar r;
    uchar g;
    uchar b;
} Color;
typedef struct __attribute__ ((packed)) RGB
{
    float r;
    float g;
    float b;
} RGB;
typedef struct __attribute__ ((packed)) MandelbrotParams
{
    double escape_r;
    double frac_min_x;
    double frac_min_y;
    double x_scale;
    double y_scale;
    uint x_offset;
    uint y_offset;
    uint min_y;
    int power;
    ushort max_iteration;
    ushort color_offset;
    RGB color_scale;
} MandelbrotParams;

typedef double2 cpx;

inline double cpx_re(const cpx* z) { return z->x; }
inline double cpx_im(const cpx* z) { return z->y; }
inline double cpx_abs(const cpx* z) { return sqrt(z->x * z->x + z->y * z->y); }
inline cpx cpx_add(const cpx* a, const cpx* b) { return (cpx)(a->x + b->x, a->y + b->y); }
inline cpx cpx_sub(const cpx* a, const cpx* b) { return (cpx)(a->x - b->x, a->y - b->y); }
inline cpx cpx_mul(const cpx* a, const cpx* b) { return (cpx)(a->x * b->x - a->y * b->y, a->x * b->y + a->y * b->x); }
inline cpx cpx_div(const cpx* a, const cpx* b)
{
	__private const double dividend = (b->x * b->x + b->y * b->y);
	return (cpx)((a->x * b->x + a->y * b->y) / dividend, (a->y * b->x - a->x * b->y) / dividend);
}
inline double cpx_theta(const cpx* z)
{
	if(z->x > 0.0)
        return atan(z->y / z->x);
    if(z->x < 0.0 && z->y >= 0.0)
        return atan(z->y / z->x) + M_PI;
    if(z->x < 0.0 && z->y < 0.0)
        return atan(z->y / z->x) - M_PI;
    if(z->x == 0.0 && z->y > 0.0)
        return M_PI_2;
    if(z->x == 0.0 && z->y < 0.0)
        return -M_PI_2;
    return 0.0;
}
inline cpx cpx_exp(const cpx* z)
{
	__private const double e = exp(z->x);
	return (cpx)(e * cos(z->y), e * sin(z->y));
}
inline cpx cpx_log(const cpx* z) { return (cpx)(log(cpx_abs(z)), cpx_theta(z)); }
inline cpx cpx_pow(const cpx* z, uint n)
{
    __private const double r_n = pow(cpx_abs(z), n);
    __private const double theta_n = n * cpx_theta(z);
    return (cpx)(r_n * cos(theta_n), r_n * sin(theta_n));
}
inline cpx cpx_sqrt(const cpx* z)
{
	__private const double r_root = sqrt(cpx_abs(z));
	__private const double theta_2 = cpx_theta(z) / 2.0;
	return (cpx)(r_root * cos(theta_2), r_root * sin(theta_2));
}

__kernel void mandelbrot(__global Color* data, __constant MandelbrotParams* params) 
{
    __private const uint y = get_global_id(1);
    __private const uint x = get_global_id(0);
    __private const cpx c = (cpx)(params->x_scale * (x + params->x_offset) + params->frac_min_x,
        params->y_scale * (y + params->min_y - params->y_offset) + params->frac_min_y);
    __private cpx z = (cpx)(0.0, 0.0);
    __private bool outside = false;
    __private ushort iteration = 0;
    while (iteration < params->max_iteration)
    {
        z = cpx_pow(&z, params->power);
        z = cpx_add(&z, &c);
        if (cpx_abs(&z) <= params->escape_r)
            ++iteration;
        else
        {
            outside = true;
            break;
        }
    }
    __private const uint offset = y * get_global_size(0) + x;
    if (outside)
    {
        z = cpx_pow(&z, params->power);
        z = cpx_add(&z, &c);
        z = cpx_pow(&z, params->power);
        z = cpx_add(&z, &c);
        __private float t = ((float)(iteration + params->color_offset + 2) - log(log(cpx_abs(&z))) / log(2.0f)) / (params->max_iteration * 1.01f);
        if (t >= FLT_EPSILON)
        {
            __private const float g = t * t;
            __private const float r = g * t;
            __private const float b = -t + g * 3.0f + r * -3.0f;
            t *= r;
            data[offset].r = (uchar)((params->color_scale.r * (-t + r)) * 255);
            data[offset].g = (uchar)((params->color_scale.g * (g - 2.0f * r + t)) * 255);
            data[offset].b = (uchar)((-params->color_scale.b * (b + t)) * 255);
        }
    }
}