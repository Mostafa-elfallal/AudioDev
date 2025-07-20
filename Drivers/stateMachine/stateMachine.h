/*
 * Embedded State Machine Driver
 * Optimized for memory-constrained embedded systems
 * Features:
 * - Minimal RAM usage
 * - Fast execution
 * - Configurable event queue size
 * - Hierarchical state support
 * - Guard conditions
 * - Entry/Exit actions
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Configuration - Adjust based on your system requirements
#define SM_MAX_EVENTS 16        // Event queue size
#define SM_MAX_STATES 32        // Maximum number of states
#define SM_MAX_TRANSITIONS 64   // Maximum number of transitions


// State and Event type definitions
typedef uint8_t sm_state_t;
typedef uint8_t sm_event_t;

// Special state values
#define SM_STATE_NONE     0xFF
#define SM_STATE_INITIAL  0x00

// Special event values
#define SM_EVENT_NONE     0xFF
#define SM_EVENT_ENTRY    0xFE
#define SM_EVENT_EXIT     0xFD

// Return codes
typedef enum {
    SM_OK = 0,
    SM_ERROR_INVALID_STATE,
    SM_ERROR_INVALID_EVENT,
    SM_ERROR_QUEUE_FULL,
    SM_ERROR_QUEUE_EMPTY,
    SM_ERROR_NULL_POINTER,
    SM_ERROR_INVALID_TRANSITION
} sm_result_t;

// Forward declarations
typedef struct sm_context sm_context_t;
typedef struct sm_transition sm_transition_t;
typedef struct sm_state_def sm_state_def_t;

// Function pointer types
typedef bool (*sm_guard_fn_t)(sm_context_t *ctx, sm_event_t event, void *event_data);
typedef void (*sm_action_fn_t)(sm_context_t *ctx, sm_event_t event, void *event_data);

// Event structure
typedef struct {
    sm_event_t type;
    void *data;
    uint32_t timestamp;  // Optional: for event timing
} sm_event_data_t;

// State definition structure
struct sm_state_def {
    sm_state_t state_id;
    sm_action_fn_t on_entry;     // Called when entering state
    sm_action_fn_t on_exit;      // Called when exiting state
    sm_action_fn_t on_event;     // Called for events not handled by transitions
    sm_state_t parent_state;     // For hierarchical states (SM_STATE_NONE if root)
    const char *name;            // Optional: for debugging
};

// Transition structure
struct sm_transition {
    sm_state_t from_state;
    sm_event_t event;
    sm_state_t to_state;
    sm_guard_fn_t guard;         // Optional: condition to check before transition
    sm_action_fn_t action;       // Optional: action to execute during transition
};

// State machine context
struct sm_context {
    // State information
    sm_state_t current_state;
    sm_state_t previous_state;
    
    // State and transition tables
    const sm_state_def_t *states;
    uint8_t num_states;
    const sm_transition_t *transitions;
    uint8_t num_transitions;
    
    // Event queue
    sm_event_data_t event_queue[SM_MAX_EVENTS];
    uint8_t queue_head;
    uint8_t queue_tail;
    uint8_t queue_count;
    
    // User data
    void *user_data;
    
    // Statistics (optional - can be disabled to save memory)
    #ifdef SM_ENABLE_STATISTICS
    uint32_t state_changes;
    uint32_t events_processed;
    uint32_t events_dropped;
    #endif
};

// =============================================================================
// CORE STATE MACHINE FUNCTIONS
// =============================================================================

/*
 * Initialize state machine context
 * @param ctx: State machine context
 * @param states: Array of state definitions
 * @param num_states: Number of states
 * @param transitions: Array of transition definitions
 * @param num_transitions: Number of transitions
 * @param initial_state: Initial state ID
 * @param user_data: User data pointer
 * @return: SM_OK on success, error code on failure
 */
sm_result_t sm_init(sm_context_t *ctx, 
                   const sm_state_def_t *states, 
                   uint8_t num_states,
                   const sm_transition_t *transitions, 
                   uint8_t num_transitions,
                   sm_state_t initial_state,
                   void *user_data);

/*
 * Post event to state machine
 * @param ctx: State machine context
 * @param event: Event type
 * @param event_data: Optional event data
 * @return: SM_OK on success, error code on failure
 */
sm_result_t sm_post_event(sm_context_t *ctx, sm_event_t event, void *event_data);

/*
 * Process one event from queue
 * @param ctx: State machine context
 * @return: SM_OK if event processed, SM_ERROR_QUEUE_EMPTY if no events
 */
sm_result_t sm_process_event(sm_context_t *ctx);

/*
 * Process all events in queue
 * @param ctx: State machine context
 * @return: Number of events processed
 */
uint8_t sm_process_all_events(sm_context_t *ctx);

/*
 * Force state transition (bypasses normal event processing)
 * @param ctx: State machine context
 * @param new_state: Target state
 * @return: SM_OK on success, error code on failure
 */
sm_result_t sm_force_transition(sm_context_t *ctx, sm_state_t new_state);

#endif // STATE_MACHINE_H