#include "ai_options.h"
#include <map>
#include <string>
#include "ai/ai.h"

std::map<string,long double> AI::opts = {};

AI_options::AI_options()
{
    AI::opts["ai_evaluate_limit"] = 0.65;
    AI::opts["ai_evaluate_time"] = 3500;
    AI::opts["ai_probabilities_pow"] = 7.0;

    AI::opts["ai_estimates_5_me"] = 1.0;
    AI::opts["ai_estimates_4_me"] = 1.0;
    AI::opts["ai_estimates_3_me"] = 0.96;
    AI::opts["ai_estimates_2_me"] = 0.14;
    AI::opts["ai_estimates_1_me"] = 0.053;
    AI::opts["ai_estimates_0_me"] = 0.0;
    AI::opts["ai_estimates_x_me"] = 0.0;

    AI::opts["ai_estimates_5_notme"] = 1.0;
    AI::opts["ai_estimates_4_notme"] = 0.99985;
    AI::opts["ai_estimates_3_notme"] = 0.65;
    AI::opts["ai_estimates_2_notme"] = 0.12;
    AI::opts["ai_estimates_1_notme"] = 0.05;
    AI::opts["ai_estimates_0_notme"] = 0.0;
    AI::opts["ai_estimates_x_notme"] = 0.0;
}

void AI::options_update() {
    AI_estimates_field::calc_est_data[0]  = opts.at("ai_estimates_5_me");
    AI_estimates_field::calc_est_data[1]  = opts.at("ai_estimates_5_notme");
    AI_estimates_field::calc_est_data[2]  = opts.at("ai_estimates_4_me");
    AI_estimates_field::calc_est_data[3]  = opts.at("ai_estimates_4_notme");
    AI_estimates_field::calc_est_data[4]  = opts.at("ai_estimates_3_me");
    AI_estimates_field::calc_est_data[5]  = opts.at("ai_estimates_3_notme");
    AI_estimates_field::calc_est_data[6]  = opts.at("ai_estimates_2_me");
    AI_estimates_field::calc_est_data[7]  = opts.at("ai_estimates_2_notme");
    AI_estimates_field::calc_est_data[8]  = opts.at("ai_estimates_1_me");
    AI_estimates_field::calc_est_data[9]  = opts.at("ai_estimates_1_notme");
    AI_estimates_field::calc_est_data[10] = opts.at("ai_estimates_0_me");
    AI_estimates_field::calc_est_data[11] = opts.at("ai_estimates_0_notme");
    AI_estimates_field::calc_est_data[12] = opts.at("ai_estimates_x_me");
    AI_estimates_field::calc_est_data[13] = opts.at("ai_estimates_x_notme");

    AI_position_recursive::probabilities_pow = opts.at("ai_probabilities_pow");;

};


AI_options::~AI_options() {};

AI_options temp;

