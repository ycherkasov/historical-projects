#include "lifetime_manager.h"

std::priority_queue<lifetime_wrapper> lifetime_tracker_manager::destroy_queue_;

lifetime_tracker::~lifetime_tracker(){}

void C_CALLING_CONVENTION_QUALIFIER at_exit_fn()
{
    // Cleanup in the pre-set lifetime order
    lifetime_tracker_manager::cleanup();
}
