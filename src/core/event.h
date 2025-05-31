#ifndef _ENG_EVENT_H_
#define _ENG_EVENT_H_

#include "config.h"

// Forward declaration (SGDK)
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef short s16;
typedef s16 fix16;
typedef u8 bool;

// --- Configuration ---
#ifndef  EVENT_MAX_LISTENERS_PER_EVENT_TYPE
    #define EVENT_MAX_LISTENERS_PER_EVENT_TYPE 8
#endif

// TODO: Define the real ones, This is just for testing for now. 
//  Include system events first and reserve some ammout for them
// Supported event types
typedef enum {
    EVT_NONE = 0,          // Should not be used, good for uninitialized
    EVT_GAME_PAUSED,       // System: game was paused
    EVT_GAME_RESUMED,          // System: game was resumed
    // EVT_COLLISION,         // Physics: two entities collided
    // EVT_PLAYER_JUMPED,     // Gameplay: player initiated a jump
    // EVT_ENEMY_DIED,        // Gameplay: an enemy was defeated    
    EVT_CUSTOM,       // Custom event the can use generic data

    // ... add more specific events
    EVT_MAX_EVENT_TYPES    // Used to size arrays, keep it last
} EventType;

// EventType type;
typedef struct {
    EventType type;
    union {
        u8    u8_val[2];    // Array of two 8-bit unsigned integers
        u16   u16_val[2];   // Array of two 16-bit unsigned integers
        u32   u32_val[2];   // Array of two 32-bit unsigned integers
        fix16 fix16_val[2]; // Array of two 16-bit fixed-point numbers
        // You could also add single values if many events only need one:
        // u8  single_u8;
        // u16 single_u16;
        // etc.

        //Specific event structure
        // CollisionEventData collision;
    } data;
} Event;

// --- Event Handler Function Pointer ---
// Listeners are functions that take an Event pointer and optionally context_data.
// The `context_data` allows a listener to know its context (e.g., which entity it belongs to).
typedef bool (*EventHandler)(const Event* event, void* contextData);

// Initialize the event bus.
void Event_init();

// Shutdown the event bus.
void Event_shutdown();

// Subscribe a handler function to a specific event type.
// Returns TRUE on success, FALSE if no more listener slots are available.
// `contextData` is passed back to the handler when the event occurs.
bool Event_subscribe(EventType type, EventHandler handler, void* contextData);

// Unsubscribe a handler function from a specific event type.
// Important: Both 'handler' and 'contextData' must match the subscription.
// Returns TRUE if found and unsubscribed, FALSE otherwise.
bool Event_unsubscribe(EventType type, EventHandler handler, void* contextData);

// Publish (or "raise" or "dispatch") an event to all subscribed listeners.
void Event_publish(const Event* event);

// Get an event type descrption tag(debug purpuses);
char* Event_getDescription(const EventType type);

// Helper to quickly publish an event with no specific data
// void EventBus_PublishType(EventType type);

#endif //_ENG_EVENT_H_