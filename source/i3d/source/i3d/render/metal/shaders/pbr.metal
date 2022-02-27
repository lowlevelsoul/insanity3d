//===============================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//===============================================================================================================================

#include <metal_stdlib>
using namespace metal;

#define POINT_DIST_EPSILON 0.000006
#define PI 3.14159265359

//===============================================================================================================================
float3 calcFresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

//===============================================================================================================================
float calcDistributionGGX(float3 N, float3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    
    return num / denom;
}

//===============================================================================================================================
float calcGeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}
//===============================================================================================================================
float calcGeometrySmith(float3 N, float3 V, float3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = calcGeometrySchlickGGX(NdotV, roughness);
    float ggx1  = calcGeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

//===============================================================================================================================
float3 calcReflectanceGlobal(float3 l, float3 lightColour, float3 n, float3 v, float3 F0, float3 albedo, float roughness, float metallic) {
    float3 h = normalize(l + v);                  // Calculate half vector for reflectance
    float attenuation = 1;               // For global light, the attentuation factor is very small
    float3 radiance = lightColour * attenuation;

    float ndf = calcDistributionGGX(n, h, roughness);
    float g = calcGeometrySmith(n, v, l, roughness);
    float3 f = calcFresnelSchlick(max(dot(h, v), 0.0), F0);

    float3 kS = f;
    float3 kD = float3(1.0f) - kS;
    kD *= 1.0 - metallic;

    float3 numerator = ndf * g * f;
    float denom = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
    float3 specular = numerator / max(denom, 0.001);

    // add to outgoing radiance Lo
    float NdotL = max(dot(n, l), 0.0);
    float3 lo = (kD * albedo / PI + specular) * radiance * NdotL;

    return lo;
}

//===============================================================================================================================
float3 calcReflectancePoint(float3 lightPos, float3 lightColour, float3 vPosition, float3 n, float3 v, float3 F0, float3 albedo, float roughness, float metallic) {
    // Calculate light direction from surface
    float3 l = lightPos - vPosition;

    // Need to deal with zero (or close to zero)_ dustance light sources
    float lLenSqr = dot(l, l);
    if (lLenSqr < POINT_DIST_EPSILON) {
        // Light is very close to point - so use normal from surface
        l = n;
        lLenSqr = POINT_DIST_EPSILON;
    } else {
        // Light is far enough from the surface for a sane calculation
        l = normalize(l);
    }

    float attenuation = 1.0 / lLenSqr;

    float3 h = normalize(l + v);                  // Calculate half vector for reflectance

    float3 radiance = lightColour * attenuation;

    float ndf = calcDistributionGGX(n, h, roughness);
    float g = calcGeometrySmith(n, v, l, roughness);
    float3 f = calcFresnelSchlick(max(dot(h, v), 0.0), F0);

    float3 kS = f;
    float3 kD = float3(1.0f) - kS;
    kD *= 1.0 - metallic;

    float3 numerator = ndf * g * f;
    float denom = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
    float3 specular = numerator / max(denom, 0.001);

    // add to outgoing radiance Lo
    float NdotL = max(dot(n, l), 0.0);
    float3 lo = (kD * albedo / PI + specular) * radiance * NdotL;

    return lo;
}

