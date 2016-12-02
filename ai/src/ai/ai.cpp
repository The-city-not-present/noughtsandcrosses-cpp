#include "ai/ai.h"

XY AI::find_move() {
    if( field->moves_count==0 )
        return XY{20,15};
    start_position = make_shared<AI_position_recursive>( &*field );
    return XY{ (*(*start_position).moves[0]).move.x+refmove.x, (*(*start_position).moves[0]).move.y+refmove.y };
    return XY{ field->ctx.x_min-1+refmove.x, field->ctx.y_min-1+refmove.y };
};

void AI::evaluate() {
    collect_move_candidates();
};

void AI::collect_move_candidates() {
    for( auto& i : AI_position_recursive.position_directory )
};
