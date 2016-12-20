match_positions = [
	[],
	[{"x":20,"y":15},{"x":20,"y":14}],
	[{"x":20,"y":15},{"x":21,"y":14}]
];/*,
	[{"x":27,"y":10},{"x":26,"y":10},{"x":27,"y":9},{"x":26,"y":9},{"x":26,"y":8},{"x":27,"y":8}],
	[{"x":27,"y":10},{"x":27,"y":9},{"x":28,"y":10},{"x":26,"y":10},{"x":28,"y":11},{"x":26,"y":9}],
	[{"x":27,"y":13},{"x":26,"y":13},{"x":30,"y":12},{"x":26,"y":12},{"x":28,"y":14},{"x":26,"y":14},{"x":26,"y":15},{"x":27,"y":15},{"x":28,"y":16},{"x":26,"y":11},{"x":26,"y":10},{"x":24,"y":12},{"x":25,"y":12},{"x":24,"y":14},{"x":25,"y":13},{"x":27,"y":12},{"x":25,"y":14},{"x":25,"y":15},{"x":28,"y":13},{"x":28,"y":17},{"x":23,"y":13}],
	[{"x":20,"y":15},{"x":21,"y":15},{"x":23,"y":13},{"x":23,"y":14},{"x":22,"y":13},{"x":19,"y":13},{"x":21,"y":14},{"x":23,"y":12},{"x":24,"y":13},{"x":25,"y":13},{"x":20,"y":13},{"x":21,"y":13},{"x":20,"y":14},{"x":20,"y":12},{"x":19,"y":12},{"x":22,"y":15},{"x":20,"y":17},{"x":20,"y":16},{"x":19,"y":16},{"x":18,"y":17},{"x":18,"y":14},{"x":19,"y":14},{"x":18,"y":15},{"x":17,"y":14},{"x":24,"y":14},{"x":22,"y":14}],
	[{"x":51,"y":10},{"x":52,"y":10},{"x":51,"y":7},{"x":51,"y":8}],
	[{"x":52,"y":14},{"x":53,"y":14},{"x":52,"y":11},{"x":52,"y":12},{"x":54,"y":11},{"x":53,"y":13},{"x":53,"y":10},{"x":51,"y":11},{"x":54,"y":14},{"x":51,"y":12},{"x":55,"y":12},{"x":52,"y":9},{"x":54,"y":12}],
	[{"x":46,"y":13},{"x":47,"y":13},{"x":46,"y":10},{"x":46,"y":11},{"x":44,"y":11},{"x":47,"y":12},{"x":47,"y":11},{"x":47,"y":14},{"x":45,"y":10},{"x":47,"y":15},{"x":47,"y":16},{"x":49,"y":14},{"x":48,"y":13},{"x":48,"y":14},{"x":46,"y":14},{"x":49,"y":13},{"x":46,"y":16},{"x":50,"y":12},{"x":51,"y":11}],
	[{"x":33,"y":13},{"x":33,"y":15},{"x":36,"y":13},{"x":34,"y":13},{"x":33,"y":10},{"x":37,"y":14},{"x":34,"y":11},{"x":33,"y":11},{"x":35,"y":10},{"x":33,"y":12},{"x":36,"y":10},{"x":34,"y":10},{"x":35,"y":12},{"x":32,"y":9},{"x":35,"y":11},{"x":32,"y":14},{"x":35,"y":9},{"x":36,"y":15},{"x":35,"y":13}],
	[{"x":41,"y":12},{"x":42,"y":12},{"x":41,"y":9},{"x":43,"y":11},{"x":41,"y":13},{"x":41,"y":11},{"x":40,"y":10},{"x":42,"y":11},{"x":40,"y":11},{"x":42,"y":13},{"x":42,"y":10},{"x":43,"y":10},{"x":40,"y":12},{"x":42,"y":14},{"x":42,"y":15},{"x":45,"y":11},{"x":44,"y":11},{"x":40,"y":9}],
	[{"x":41,"y":12},{"x":42,"y":12},{"x":41,"y":9},{"x":43,"y":11},{"x":40,"y":10},{"x":41,"y":13},{"x":44,"y":10},{"x":42,"y":10},{"x":43,"y":9},{"x":39,"y":11},{"x":42,"y":9},{"x":40,"y":9},{"x":44,"y":9},{"x":45,"y":9},{"x":44,"y":11},{"x":40,"y":14},{"x":39,"y":15},{"x":44,"y":12},{"x":43,"y":10}],
	[{"x":48,"y":13},{"x":49,"y":13},{"x":48,"y":10},{"x":50,"y":12},{"x":47,"y":11},{"x":48,"y":14},{"x":51,"y":11},{"x":49,"y":11},{"x":50,"y":10},{"x":52,"y":12},{"x":51,"y":10},{"x":49,"y":10},{"x":49,"y":12},{"x":47,"y":14},{"x":51,"y":14},{"x":47,"y":15},{"x":46,"y":16},{"x":46,"y":14},{"x":49,"y":14}]
];*/



function http_test( arg ) {
	var opts = {};
	opts.ai_probabilities_pow = arg;
	http_server_ai( opts );
};

/*temp_arr = [];
for( var i=0; i<10; ++i ) {
	var delta = 0;
	for( var j=0; j<20; ++j )
		delta += (Math.random()-0.5);
	temp_arr[i] = Math.exp( Math.log(3.9)+delta );
};*/

temp_arr = [ 0.9, 1.6, 2.2, 3.0, 20.0, 4.6, 6.0, 7.5, 9.0, 10.5 ]

function http_bench_default() {	http_server_ai();	};
http_bench_default.title = 'http_ai_pow_default_e';

function http_bench_innovative() {	http_test(-1);	};
http_bench_innovative.title = 'http_ai_pow_innovative_e';

function http_bench_0() {	http_test( temp_arr[0] );	};
http_bench_0.title = 'http_ai_pow_'+temp_arr[0]+'_e';

function http_bench_1() {	http_test( temp_arr[1] );	};
http_bench_1.title = 'http_ai_pow_'+temp_arr[1]+'_e';

function http_bench_2() {	http_test( temp_arr[2] );	};
http_bench_2.title = 'http_ai_pow_'+temp_arr[2]+'_e';

function http_bench_3() {	http_test( temp_arr[3] );	};
http_bench_3.title = 'http_ai_pow_'+temp_arr[3]+'_e';

function http_bench_4() {	http_test( temp_arr[4] );	};
http_bench_4.title = 'http_ai_pow_'+temp_arr[4]+'_e';

function http_bench_5() {	http_test( temp_arr[5] );	};
http_bench_5.title = 'http_ai_pow_'+temp_arr[5]+'_e';

function http_bench_6() {	http_test( temp_arr[6] );	};
http_bench_6.title = 'http_ai_pow_'+temp_arr[6]+'_e';

function http_bench_7() {	http_test( temp_arr[7] );	};
http_bench_7.title = 'http_ai_pow_'+temp_arr[7]+'_e';

function http_bench_8() {	http_test( temp_arr[8] );	};
http_bench_8.title = 'http_ai_pow_'+temp_arr[8]+'_e';

function http_bench_9() {	http_test( temp_arr[9] );	};
http_bench_9.title = 'http_ai_pow_'+temp_arr[9]+'_e';

function http_bench_a0() {	http_test( 8.0 );	};
http_bench_a0.title = 'http_ai_pow_8_e';

function http_bench_a1() {	http_test( 10.0 );	};
http_bench_a1.title = 'http_ai_pow_10_e';

function http_bench_a2() {	http_test( 11.0 );	};
http_bench_a2.title = 'http_ai_pow_11_e';

function http_bench_a3() {	http_test( 14.0 );	};
http_bench_a3.title = 'http_ai_pow_14_e';


var match_participants = [
	andrej_move_0_0_3,
	andrej_move_0_0_4,
	andrej_move_0_0_5,
	andrej_move_0_0_6,
	http_bench_default,
	http_bench_innovative,
	http_bench_0,
	http_bench_1,
	http_bench_2,
	http_bench_3,
	http_bench_4,
	http_bench_5,
	http_bench_6,
	http_bench_7,
	http_bench_8,
	http_bench_a0,
	http_bench_a1,
	http_bench_a2,
	http_bench_a3
];

