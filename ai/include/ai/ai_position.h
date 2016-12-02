#ifndef AI_POSITION_H
#define AI_POSITION_H

#include <vector>

#include "ai/ai_estimates_field.h"
#include "ai/ai_move.h"

class AI_move;


// == 1. AI_position_prototype ==

class AI_position_prototype {
    public:
        //AI_position_prototype() = delete;

/* 1. */int moves_count;
/* 2. */est_pair estimate;

};




// == 2. AI_position_recursive ==

class AI_position_recursive : public AI_position_prototype
{
    public:
        AI_position_recursive() = delete;
        AI_position_recursive( Field<Lines_val>* );
        AI_position_recursive( Field<Lines_val>*, XY );

        AI_estimates_field estimates_field;
        vector<unique_ptr<AI_move>> moves;

    protected:
        void collect_moves(); // called once in constructor
        void calculate_estimate();

    private:
};




// == 3. AI_position_static ==

class AI_position_static : public AI_position_prototype {
    public:
        AI_position_static() = delete;
        AI_position_static( int, est_pair );
};

#endif // AI_POSITION_H
