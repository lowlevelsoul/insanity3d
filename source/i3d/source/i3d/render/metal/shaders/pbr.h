//===============================================================================================================================
// CONFIDENTIAL AND PROPRIETARY INFORMATION / NOT FOR DISCLOSURE WITHOUT WRITTEN PERMISSION
// Copyright (C) 2020, 2021 James Steele. All Rights Reserved.
//===============================================================================================================================

#ifndef __PBR_H__
#define __PBR_H__

extern float3 calcReflectanceGlobal(float3 l, float3 lightColour, float3 n, float3 v, float3 F0, float3 albedo, float roughness, float metallic);
extern float3 calcReflectancePoint(float3 lightPos, float3 lightColour, float3 vPosition, float3 n, float3 v, float3 F0, float3 albedo, float roughness, float metallic);

#endif

