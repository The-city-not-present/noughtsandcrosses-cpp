#ifndef AI_MOVE_H
#define AI_MOVE_H

#include "lines.h"
#include "ai/ai_estimates_field.h"
#include "ai/ai_position.h"

class AI_position_prototype;
class AI_position_recursive;
class AI_position_static;


class AI_move
{
    public:
        AI_move( AI_position_recursive*, XY, double_pair );

        XY move;
        AI_position_recursive* parent_position;
        unique_ptr<AI_position_prototype> position;
        double_pair& get_estimate();
        double_pair est_base;
        double probability = {0};

    protected:

    private:
};

#endif
