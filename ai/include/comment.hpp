namespace comment {
    // тупые костыли вместо нормальных atoi
    string str5( string arg ) { return string("     "+arg).substr(arg.length(),string::npos); };
    string str5( int arg ) { stringstream s;s << arg; return str5(s.str()); };
    string str5( long double arg ) {int a=arg*1000;stringstream s;for(int i=1000;i>=1; i/=10){s<<a/i;if(i==1000)s<<".";a=a%i;};return s.str();};
    string str3( string arg ) { return string("   "+arg).substr(arg.length(),string::npos); };
    string str3( int arg ) { stringstream s;s << arg; return str3(s.str()); };

    string comment_position_deployment( AI_position_recursive*const node, string prefix={}, int moves_to_show = {5} ) {
        string result;
        string endl = "\\n";
        string result_append;
        result_append = prefix+"estimate: [ "+str5(node->estimate[0])+"  "+str5(node->estimate[1])+" ]  count= "+str5((int)node->moves.size());
        result += result_append + string((size_t)(90-result_append.size()),(char)' ')+"#"+str3(node->id()) + endl;
        prefix += "    ";
        int counter = 0;
        bool soft = false;
        bool last_is_soft = false;
        for( auto& i : node->moves ) {
            if( ++counter>moves_to_show )
                soft = true;
            if(
               !soft ||
               i.position->is_recursive()
            ) {
                result += prefix+str3(counter)+": ("+str3(i.move.x)+","+str3(i.move.y)+" ) e=  [ "+str5(i.get_estimate()[0])+"  "+str5(i.get_estimate()[1])+" ] p=  "+str5(i.probability) + "   r=  " + str5(i.position->reliability) + (i.position->is_recursive()?"  ->":"") + endl;
                last_is_soft = false;
            } else {
                if( soft && !last_is_soft ) {
                    result += prefix +"..."+endl;
                    last_is_soft = true;
                };
            };
            if( i.position->is_recursive() )
                result += comment_position_deployment( (AI_position_recursive*)i.position.get(), prefix );
        };
        return result;
    };

    string ai_comment( AI& ai ) {
        if( !ai.start_position )
            return "no position";
        return comment_position_deployment( ai.start_position.get(), "", 1000 );
    };
};

