#ifndef AI_H
#define AI_H

#include "lines.h"
#include "ai/ai_position.h"
#include "ai/ai_options.h"
#include <map>
#include <string>


class AI : public Lines
{
    public:
        AI() = delete;
        AI( Lines ) = delete;
        AI( Lines& ) = delete;
        AI( move_list& m ) : Lines(m) {};
        XY find_move();
        unique_ptr<AI_position_recursive> start_position;
        void options_update();

        bool evaluate( long double );

        static std::map<string,long double> opts;
    protected:
        vector<AI_move*> collect_move_candidates( long double limit=0.55 );
        void flush_position_probabilities();
        void recalculate_estimates_recursive();

    private:
};

#endif // AI_H
