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
        shared_ptr<AI_position_recursive> start_position;

        void evaluate();

    protected:
        void collect_move_candidates();

    private:
};

#endif // AI_H
