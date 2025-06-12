
/**
 *  \file gen-engine.h
 *  \brief Main engine header
 *  \author Fernando Machado
 *  \date 06/2025
 *
 * Provides all engine include files for implementing games using gen-engine 
 */

#ifndef _GEN_ENGINE_H_
#define _GEN_ENGINE_H_

#include <genesis.h>

// Game engine library imports
#include "config.h"
#include "utils.h"
#include "logger.h"
#include "asserts.h"
#include "profiler.h"
#include "memory.h"
#include "input.h"
#include "event.h"
#include "components.h"
#include "ecs.h"
#include "systems.h"
#include "scene.h"

#include "../scene_main.h"

#endif // _GEN_ENGINE_H_