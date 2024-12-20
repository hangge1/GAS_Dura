// Copyright by person HDD  

#pragma once

#include "CoreMinimal.h"

const float HIGHLIGHT_COLOR_RED = 250.0f;
const float HIGHLIGHT_COLOR_BLUE = 251.0f;
const float HIGHLIGHT_COLOR_TAN = 252.0f;

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel3