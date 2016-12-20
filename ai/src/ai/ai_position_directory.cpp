#include "ai_position_directory.h"
#include <algorithm>
#include "field_value_iterator.h"

AI_position_directory::AI_position_directory() {}

AI_position_directory& AI_position_directory::operator << ( shared_ptr<AI_position_prototype> item ) {
    data[item->moves_count].push_back(item);
    return *this;
};

/*shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Estimate_field_cell_type>& field ) {
    return shared_ptr<AI_position_prototype>(nullptr);
};

shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Field_cell_type>& field ) {
    return shared_ptr<AI_position_prototype>(nullptr);
};
*/


shared_ptr<AI_position_prototype> AI_position_directory::search( AI_estimates_field* field, XY point ) {
    auto &pos_vector = data[field->moves_count+1];
    for( auto &position : pos_vector ) {
        bool identical = true;
        auto it1 = field_val_begin(&static_pointer_cast<AI_position_recursive>(position)->estimates_field);
        auto it2 = field_val_begin(field);
        while(
            (it1!=field_val_end(&static_pointer_cast<AI_position_recursive>(position)->estimates_field))||
            (it2!=field_val_end(field))
        ) {
            if( (it1.pivot!=it2.pivot)&&(it1.pivot==point)&&(*it1==((field->moves_count&1)==0?cross:nought)) ) {
                ++it1;
                continue;
            };
            if( (it1.pivot!=it2.pivot)||(*it1!=*it2) ) {
                identical = false;
                break;
            };
            ++it1;
            ++it2;
        };
        //while
        if( identical )
            return position;
    };
    return shared_ptr<AI_position_prototype>(nullptr);
};

/*
shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Field_cell_type>& field, XY p ) {
    return shared_ptr<AI_position_prototype>(nullptr);
};*/

void AI_position_directory::purge( Field<Field_cell_type>* field ) {
    // TODO: реализовать
};

void AI_position_directory::purge( Field<Estimate_field_cell_type>* field ) {
};


AI_position_directory AI_position_recursive::position_directory = {};

