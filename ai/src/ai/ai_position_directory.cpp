#include "ai_position_directory.h"
#include <algorithm>

AI_position_directory::AI_position_directory() {}

AI_position_directory& AI_position_directory::operator << ( shared_ptr<AI_position_prototype> item ) {
    data[item->moves_count].push_back(item);
    return *this;
};

shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Estimate_field_cell_type>& field ) {
    return shared_ptr<AI_position_prototype>(nullptr);
};

shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Field_cell_type>& field ) {
    return shared_ptr<AI_position_prototype>(nullptr);
};

shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Estimate_field_cell_type>& field, XY p ) {
    return shared_ptr<AI_position_prototype>(nullptr);
    auto &pos_vector = data[field.moves_count+1];
    for( auto &position : pos_vector ) {
        bool identical = true;
        auto it1 = static_pointer_cast<AI_position_recursive>(position)->estimates_field.begin();
        auto it2 = field.begin();
        while(
            (it1!=static_pointer_cast<AI_position_recursive>(position)->estimates_field.end())&&
            ((Field_cell_type)(*it1)).is_undefined()
        )
            ++it1;
        while(
            (it2!=field.end())&&
            ((Field_cell_type)(*it2)).is_undefined()
        )
            ++it2;
        //while
        if( identical )
            return position;
    };
    return shared_ptr<AI_position_prototype>(nullptr);
};

shared_ptr<AI_position_prototype> AI_position_directory::search( Field<Field_cell_type>& field, XY p ) {
    return shared_ptr<AI_position_prototype>(nullptr);
};

void AI_position_directory::purge( Field<Field_cell_type>* field ) {
    // TODO: реализовать
};

void AI_position_directory::purge( Field<Estimate_field_cell_type>* field ) {
};


AI_position_directory AI_position_recursive::position_directory = {};

