#include "ai_position_directory.h"
#include <algorithm>

AI_position_directory::AI_position_directory() {}

AI_position_directory& AI_position_directory::operator << ( AI_position_recursive* item ) {
    data[item->moves_count].push_back(item);
    return *this;
};

AI_position_prototype* AI_position_directory::search( Field<Estimate_field_cell_type>* field ) {
    return nullptr;
};

AI_position_prototype* AI_position_directory::search( Field<Field_cell_type>* field ) {
    return nullptr;
};

AI_position_prototype* AI_position_directory::search( Field<Estimate_field_cell_type>* field, XY p ) {
    return nullptr;
};

AI_position_prototype* AI_position_directory::search( Field<Field_cell_type>* field, XY p ) {
    return nullptr;
};

void AI_position_directory::purge( Field<Field_cell_type>* field ) {
    // TODO: реализовать
};

void AI_position_directory::purge( Field<Estimate_field_cell_type>* field ) {
};


AI_position_directory AI_position_recursive::position_directory = {};

