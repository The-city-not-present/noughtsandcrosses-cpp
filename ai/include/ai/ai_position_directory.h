#ifndef AI_POSITION_DIRECTORY_H
#define AI_POSITION_DIRECTORY_H

#include <map>
#include "ai/ai_position.h"

class AI_position_prototype;
class AI_position_recursive;

class AI_position_directory
{
    public:
        AI_position_directory();
        AI_position_prototype* search( Field<Field_cell_type>* );
        AI_position_prototype* search( Field<Estimate_field_cell_type>* );
        AI_position_prototype* search( Field<Field_cell_type>*, XY );
        AI_position_prototype* search( Field<Estimate_field_cell_type>*, XY );
        AI_position_directory& operator << ( AI_position_recursive* );
        void purge( Field<Field_cell_type>* );
        void purge( Field<Estimate_field_cell_type>* );

        map<int,vector<AI_position_recursive*>> data;
};

#endif // AI_POSITION_DIRECTORY_H
