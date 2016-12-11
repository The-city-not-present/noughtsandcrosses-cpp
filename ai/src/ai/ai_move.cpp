#include "ai/ai_move.h"
#include "ai/ai.h"

AI_move::AI_move( AI_position_recursive* parent, XY p, double_pair e ) :
        move(p),
        est_base(e),
        parent_position( parent)
{
    position = unique_ptr<AI_position_prototype>( new AI_position_static{ parent->moves_count+1, e } );
};

const double_pair AI_move::get_estimate() {
    return position->estimate;
    return double_pair{
        position->estimate[0]*1+est_base[0]*(1.0-1),
        position->estimate[1]*1+est_base[1]*(1.0-1)
    };
    return double_pair{
        position->estimate[0]*position->reliability+est_base[0]*(1.0-position->reliability),
        position->estimate[1]*position->reliability+est_base[1]*(1.0-position->reliability)
    };
}


