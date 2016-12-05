namespace comment {
    string str4( string arg ) { return string("    "+arg).substr(arg.length(),string::npos); };
    string str4( int arg ) { stringstream s;s << arg; return str4(s.str()); };
    string str4( double arg ) { stringstream s;s << arg; return string(s.str()+"    ").substr(0,4); };
    string str3( string arg ) { return string("   "+arg).substr(arg.length(),string::npos); };
    string str3( int arg ) { stringstream s;s << arg; return str3(s.str()); };

    string comment_position_deployment( AI_position_recursive*const node, string prefix={} ) {
        string result;
        const int count = 5;
        string endl = "<br />";
        result += prefix+"estimate: [ "+str4(node->estimate[0])+"  "+str4(node->estimate[1])+" ]  count= "+str4((int)node->moves.size()) + endl;
        prefix += "    ";
        int counter = 0;
        bool soft = false;
        bool last_is_soft = false;
        for( auto& i : node->moves ) {
            if( ++counter>count )
                soft = true;
            if(
               !soft ||
               i.position->is_recursive()
            ) {
                result += prefix+"("+str3(i.move.x)+","+str3(i.move.y)+" ) e=  [ "+str4(i.get_estimate()[0])+"  "+str4(i.get_estimate()[1])+" ] p=  "+str4(i.probability) + (i.position->is_recursive()?"  ->":"") + endl;
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
        return comment_position_deployment( ai.start_position.get() );
    };
};

