/*
 * stateMachine.c
 *
 *  Created on: Jun 22, 2025
 *      Author: mosta
 */

#include "stateMachine.h"
/*  PFP  */
/*
 * Get current state
 * @param ctx: State machine context
 * @return: Current state ID
 */
static inline sm_state_t sm_get_current_state(const sm_context_t *ctx) {
    return ctx->current_state;
}

/*
 * Get previous state
 * @param ctx: State machine context
 * @return: Previous state ID
 */
static inline sm_state_t sm_get_previous_state(const sm_context_t *ctx) {
    return ctx->previous_state;
}

/*
 * Check if state machine is in specific state
 * @param ctx: State machine context
 * @param state: State to check
 * @return: true if in state, false otherwise
 */
static inline bool sm_is_in_state(const sm_context_t *ctx, sm_state_t state) {
    return ctx->current_state == state;
}

/*
 * Get number of events in queue
 * @param ctx: State machine context
 * @return: Number of events in queue
 */
static inline uint8_t sm_get_queue_count(const sm_context_t *ctx) {
    return ctx->queue_count;
}

// =============================================================================
// IMPLEMENTATION
// =============================================================================

// Helper function to find state definition
static const sm_state_def_t* sm_find_state_def(const sm_context_t *ctx, sm_state_t state) {
    for (uint8_t i = 0; i < ctx->num_states; i++) {
        if (ctx->states[i].state_id == state) {
            return &ctx->states[i];
        }
    }
    return NULL;
}

// Helper function to find transition
static const sm_transition_t* sm_find_transition(const sm_context_t *ctx, 
                                                sm_state_t from_state, 
                                                sm_event_t event) {
    for (uint8_t i = 0; i < ctx->num_transitions; i++) {
        const sm_transition_t *trans = &ctx->transitions[i];
        if (trans->from_state == from_state && trans->event == event) {
            return trans;
        }
    }
    return NULL;
}

// Execute state entry action
static void sm_execute_entry(sm_context_t *ctx, sm_state_t state,void *data) {
    const sm_state_def_t *state_def = sm_find_state_def(ctx, state);
    if (state_def && state_def->on_entry) {
        state_def->on_entry(ctx, SM_EVENT_ENTRY, data);
    }
}

// Execute state exit action
static void sm_execute_exit(sm_context_t *ctx, sm_state_t state, void *data) {
    const sm_state_def_t *state_def = sm_find_state_def(ctx, state);
    if (state_def && state_def->on_exit) {
        state_def->on_exit(ctx, SM_EVENT_EXIT, data);
    }
}

// Initialize state machine
sm_result_t sm_init(sm_context_t *ctx, 
                   const sm_state_def_t *states, 
                   uint8_t num_states,
                   const sm_transition_t *transitions, 
                   uint8_t num_transitions,
                   sm_state_t initial_state,
                   void *user_data) {
    
    if (!ctx || !states || !transitions) {
        return SM_ERROR_NULL_POINTER;
    }
    
    // Initialize context
    ctx->current_state = SM_STATE_NONE;
    ctx->previous_state = SM_STATE_NONE;
    ctx->states = states;
    ctx->num_states = num_states;
    ctx->transitions = transitions;
    ctx->num_transitions = num_transitions;
    ctx->user_data = user_data;
    
    // Initialize event queue
    ctx->queue_head = 0;
    ctx->queue_tail = 0;
    ctx->queue_count = 0;
    
    #ifdef SM_ENABLE_STATISTICS
    ctx->state_changes = 0;
    ctx->events_processed = 0;
    ctx->events_dropped = 0;
    #endif
    
    // Validate initial state
    if (sm_find_state_def(ctx, initial_state) == NULL) {
        return SM_ERROR_INVALID_STATE;
    }
    
    // Set initial state
    ctx->current_state = initial_state;
    sm_execute_entry(ctx, initial_state,NULL);
    
    return SM_OK;
}

// Post event to queue
sm_result_t sm_post_event(sm_context_t *ctx, sm_event_t event, void *event_data) {
    if (!ctx) {
        return SM_ERROR_NULL_POINTER;
    }
    
    // Check if queue is full
    if (ctx->queue_count >= SM_MAX_EVENTS) {
        #ifdef SM_ENABLE_STATISTICS
        ctx->events_dropped++;
        #endif
        return SM_ERROR_QUEUE_FULL;
    }
    
    // Add event to queue
    sm_event_data_t *event_slot = &ctx->event_queue[ctx->queue_tail];
    event_slot->type = event;
    event_slot->data = event_data;
    event_slot->timestamp = 0;  // Set timestamp if needed
    
    ctx->queue_tail = (ctx->queue_tail + 1) % SM_MAX_EVENTS;
    ctx->queue_count++;
    
    return SM_OK;
}

// Process single event
sm_result_t sm_process_event(sm_context_t *ctx) {
    if (!ctx) {
        return SM_ERROR_NULL_POINTER;
    }
    
    // Check if queue is empty
    if (ctx->queue_count == 0) {
        return SM_ERROR_QUEUE_EMPTY;
    }
    
    // Get event from queue
    sm_event_data_t *event_data = &ctx->event_queue[ctx->queue_head];
    sm_event_t event = event_data->type;
    void *data = event_data->data;
    
    ctx->queue_head = (ctx->queue_head + 1) % SM_MAX_EVENTS;
    ctx->queue_count--;
    
    #ifdef SM_ENABLE_STATISTICS
    ctx->events_processed++;
    #endif
    
    // Look for transition
    const sm_transition_t *transition = sm_find_transition(ctx, ctx->current_state, event);
    
    if (transition) {
        // Check guard condition
        if (transition->guard && !transition->guard(ctx, event, data)) {
            // Guard failed, don't transition
            return SM_OK;
        }
        
        // Execute transition
        sm_state_t old_state = ctx->current_state;
        
        // Execute exit action
        sm_execute_exit(ctx, old_state,data);
        
        // Execute transition action
        if (transition->action) {
            transition->action(ctx, event, data);
        }
        
        // Change state
        ctx->previous_state = old_state;
        ctx->current_state = transition->to_state;
        
        #ifdef SM_ENABLE_STATISTICS
        ctx->state_changes++;
        #endif
        
        // Execute entry action
        sm_execute_entry(ctx, ctx->current_state,data);
        
    } else {
        // No transition found, let current state handle the event
        const sm_state_def_t *state_def = sm_find_state_def(ctx, ctx->current_state);
        if (state_def && state_def->on_event) {
            state_def->on_event(ctx, event, data);
        }
    }
    
    return SM_OK;
}

// Process all events
uint8_t sm_process_all_events(sm_context_t *ctx) {
    if (!ctx) {
        return 0;
    }
    
    uint8_t processed = 0;
    while (sm_process_event(ctx) == SM_OK) {
        processed++;
    }
    
    return processed;
}

// Force state transition
sm_result_t sm_force_transition(sm_context_t *ctx, sm_state_t new_state) {
    if (!ctx) {
        return SM_ERROR_NULL_POINTER;
    }
    
    // Validate new state
    if (sm_find_state_def(ctx, new_state) == NULL) {
        return SM_ERROR_INVALID_STATE;
    }
    
    // Execute transition
    sm_state_t old_state = ctx->current_state;
    
    sm_execute_exit(ctx, old_state,NULL);
    
    ctx->previous_state = old_state;
    ctx->current_state = new_state;
    
    #ifdef SM_ENABLE_STATISTICS
    ctx->state_changes++;
    #endif
    
    sm_execute_entry(ctx, new_state,NULL);
    
    return SM_OK;
}

