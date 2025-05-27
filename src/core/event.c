#include "logger.h"
#include "event.h"

// Listener structure
typedef struct {
    EventHandler handler;
    void* contextData;
} EventListener;

// --- Helper Functions ---
static char* g_event_type_strings[EVT_MAX_EVENT_TYPES] = {
    "None", "Game Paused", "Game Resumed", "Custom"
};

// --- Static Data for the Event Bus ---
// Array of listener lists. Each inner array holds listeners for one EventType.
static EventListener g_event_listeners[EVT_MAX_EVENT_TYPES][EVENT_MAX_LISTENERS_PER_EVENT_TYPE];

// Counts of active listeners for each event type.
// u8 is fine if EVENT_MAX_LISTENERS_PER_EVENT_TYPE <= 255
static u8 g_event_listener_counts[EVT_MAX_EVENT_TYPES];// --- Implementation ---

void Event_init(void) {    
    memset(g_event_listener_counts, 0, sizeof(g_event_listener_counts));
    memset(g_event_listeners, 0, sizeof(g_event_listeners));
    
    LOGGER_INFO("Event subsystem initialized.");
}

void Event_shutdown() {
    // TODO: Cleanup queued entries
    LOGGER_INFO("Shutting down event subsystem");
}

bool Event_subscribe(EventType type, EventHandler handler, void* contextData) {
    //TODO: Replace these logger errors to asserts
    if (type <= EVT_NONE || type >= EVT_MAX_EVENT_TYPES) {
        LOGGER_ERROR("Event: Invalid event type for subscription: %u", type);
        return FALSE;
    }

    if (handler == NULL) {
        LOGGER_ERROR("Event: Null handler for subscription");
        return FALSE; // Null handler
    }
    
    if (g_event_listener_counts[type] < EVENT_MAX_LISTENERS_PER_EVENT_TYPE) {
        // Check for duplicates (optional, but good practice)
        for (u8 i = 0; i < g_event_listener_counts[type]; ++i) {
            if (g_event_listeners[type][i].handler == handler && 
                g_event_listeners[type][i].contextData == contextData)
            {
                LOGGER_WARN("Event: : Handler already subscribed to type: %u", type);
                return TRUE; // Already subscribed, consider it a success
            }
        }
        
        g_event_listeners[type][g_event_listener_counts[type]].handler = handler;
        g_event_listeners[type][g_event_listener_counts[type]].contextData = contextData;
        g_event_listener_counts[type]++;

        LOGGER_INFO("Event: Subscribed to type %u. Total listeners for type: %u", type, g_event_listener_counts[type]);        
        return TRUE;
    } else {
        LOGGER_WARN("Event: No more listener slots for event type: %u", type);
        return FALSE; // No more slots
    }
}

bool Event_unsubscribe(EventType type, EventHandler handler, void* contextData) {
    //TODO: Replace these logger errors to asserts
    if (type <= EVT_NONE || type >= EVT_MAX_EVENT_TYPES) {
        LOGGER_ERROR("Event: Invalid event type for unsubscription: %u", type);
        return FALSE;
    }

    if (handler == NULL) {
        LOGGER_ERROR("Event: Null handler for unsubscription");
        return FALSE; // Null handler
    }

    for (u8 i = 0; i < g_event_listener_counts[type]; ++i) {
        if (g_event_listeners[type][i].handler == handler &&
            g_event_listeners[type][i].contextData == contextData) 
        {
            //TODO: Alter to instead of moving all listeners only move the last to the current free position            
            // Found the listener. Remove it by shifting subsequent listeners down.
            // This keeps the list compact.
            for (u8 j = i; j < g_event_listener_counts[type] - 1; ++j) {
                g_event_listeners[type][j] = g_event_listeners[type][j + 1];
            }
            g_event_listener_counts[type]--;

            // Clear the (now unused) last slot
            g_event_listeners[type][g_event_listener_counts[type]].handler = NULL;
            g_event_listeners[type][g_event_listener_counts[type]].contextData = NULL;

            // KLog_U1("EventBus: Unsubscribed from type %u.", type);
            LOGGER_INFO("Event: Usubscribed to type %u. Total listeners for type: %u", type, g_event_listener_counts[type]);
            return TRUE;
        }
    }

    LOGGER_WARN("Event: Handler not found for unsubscription from type: %u", type);
    return FALSE; // Listener not found
}

void Event_publish(const Event* event) {
    //TODO: Replace these logger errors to asserts
    if (event == NULL || event->type <= EVT_NONE || event->type >= EVT_MAX_EVENT_TYPES) {        
        LOGGER_ERROR("Event: Invalid event or event type for publish");
        return;
    }

    LOGGER_INFO("Event: Publishing event type: %u", event->type);

    //TODO: Reevaluate the necessity of a caching
    u8 currentListenerCount = g_event_listener_counts[event->type]; // Cache count in case a handler unsubscribes
    for (u8 i = 0; i < currentListenerCount; ++i) {
        if (g_event_listeners[event->type][i].handler != NULL) { // Double check, though should be
            g_event_listeners[event->type][i].handler(event, g_event_listeners[event->type][i].contextData);
        }
    }
}

char* Event_getDescription(const EventType type) {
    //TODO: Replace these logger errors to asserts
    if (type <= EVT_NONE || type >= EVT_MAX_EVENT_TYPES) {
        LOGGER_ERROR("Event: Invalid event type for getting description: %u", type);
        return FALSE;
    }
    return g_event_type_strings[type];
}