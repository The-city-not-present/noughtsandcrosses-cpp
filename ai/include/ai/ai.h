#ifndef AI_H
#define AI_H

#include "lines.h"
#include "ai/ai_position.h"


class AI : public Lines
{
    public:
        AI() = delete;
        AI( Lines ) = delete;
        AI( Lines& ) = delete;
        AI( move_list& m ) : Lines(m) {};
        XY find_move();
        unique_ptr<AI_position_recursive> start_position;

        void evaluate();

    protected:
        vector<AI_move*> collect_move_candidates();
        void flush_position_probabilities();
        void recalculate_estimates_recursive();

    private:
};

#endif // AI_H
