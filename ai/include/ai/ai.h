#ifndef AI_H
#define AI_H

#include "lines.h"
#include "ai/ai_position.h"
#include "ai/ai_options.h"
#include <map>
#include <string>

#include <functional>

using adjust_limit_fn = std::function<int (long double)>;
using evaluate_cond_fn = std::function<bool()>;
long double adjust_limit_val( long double limit_base, adjust_limit_fn, int );


class AI : public Lines
{
    public:
        AI() = delete;
        AI( Lines ) = delete;
        AI( Lines& ) = delete;
        AI( move_list& m ) : Lines(m) {};
        XY find_move();
        shared_ptr<AI_position_recursive> start_position;
        void options_update();

        bool evaluate( long double, evaluate_cond_fn );

        static std::map<string,long double> opts;
    protected:
        vector<AI_move*> collect_move_candidates( long double limit=0.55 );
        int collect_move_candidates_fake( long double limit=0.55 );
        void flush_position_probabilities();
        void recalculate_estimates_recursive();
        void evaluate_node( AI_move& );

    private:
};

#endif // AI_H
