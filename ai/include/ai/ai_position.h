#ifndef AI_POSITION_H
#define AI_POSITION_H

#include <vector>

#include "ai/ai_estimates_field.h"
#include "ai/ai_move.h"
#include "ai/ai_position_directory.h"

class AI_move;
class AI_position_directory;


// == 1. AI_position_prototype ==

class AI_position_prototype {
    public:
        virtual ~AI_position_prototype() {};

/* 1. */int moves_count;
/* 2. */double_pair estimate;

        double probability_global = {0};
        virtual void update_probability_global( double ) = 0;
};




// == 2. AI_position_recursive ==

class AI_position_recursive : public AI_position_prototype
{
    public:
        AI_position_recursive() = delete;
        AI_position_recursive( Field<Field_cell_type>* );
        AI_position_recursive( Field<Field_cell_type>*, XY );
        AI_position_recursive( Field<Estimate_field_cell_type>* );
        AI_position_recursive( Field<Estimate_field_cell_type>*, XY );

        ~AI_position_recursive();

        AI_estimates_field estimates_field;
        vector<unique_ptr<AI_move>> moves;

        void update_probability_global( double );

        static AI_position_directory position_directory;
    protected:
        void collect_moves_and_calculate_estimates(); // called once in constructor

    private:
};




// == 3. AI_position_static ==

class AI_position_static : public AI_position_prototype {
    public:
        AI_position_static() = delete;
        AI_position_static( int, double_pair );

        void update_probability_global( double );

        ~AI_position_static();

        static AI_position_directory position_directory;
};

#endif // AI_POSITION_H
