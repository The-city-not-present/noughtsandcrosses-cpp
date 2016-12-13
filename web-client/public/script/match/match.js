match_positions = [
	[],
	[{"x":20,"y":15},{"x":20,"y":14}],
	[{"x":20,"y":15},{"x":21,"y":14}],
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
];

function http_test( n, reduce ) {
	var opts = {};
	opts.ai_estimates = {
		"e5_me"    : temp_arr[n][0],
		"e5_notme" : temp_arr[n][1],
		"e4_me"    : temp_arr[n][2],
		"e4_notme" : temp_arr[n][3],
		"e3_me"    : temp_arr[n][4],
		"e3_notme" : temp_arr[n][5],
		"e2_me"    : temp_arr[n][6],
		"e2_notme" : temp_arr[n][7],
		"e1_me"    : temp_arr[n][8],
		"e1_notme" : temp_arr[n][9],
		"e0_me"    : temp_arr[n][10],
		"e0_notme" : temp_arr[n][11],
		"ex_me"    : temp_arr[n][12],
		"ex_notme" : temp_arr[n][13]
	};
	if( reduce )
		opts["ai_evaluate_limit"] = 0.35;
	http_server_ai( opts );
};

/*var temp_ar = [1,1,1,0.996561952005591,0.9156547245262137,0.8008889830784672,0.29486087880551215,0.16900443525397754,0.10896755345565076,0.08094968827530985,0.029543283525848445,0.00005037803752084075,0.00002129892918675167,0];
var temp_ar_reduced = [1,1,1,0.996561952005591,0.9156547245262137,0.8008889830784672,0.29486087880551215,0.16900443525397754,0.10896755345565076,0.08094968827530985,0.029543283525848445,0.00005037803752084075,0.00002129892918675167,0];
for(var i=0;i<14;i++)
	temp_ar_reduced[i] = temp_ar_reduced[i]*temp_ar_reduced[i]*temp_ar_reduced[i];
temp_ar_reduced_gradient = [];
for(var i=0;i<14;i++)
	temp_ar_reduced_gradient[i] = temp_ar[i]*(14-i)/14+temp_ar_reduced[i]*i/14;
//[ 1, 1, 1, 0.9897212759004829, 0.7677065071882779, 0.5137087449114055, 0.025636071060371912, 0.004827189035840084, 0.0012938728520440856, 0.0005304513292940941, 0, 0, 0, 0 ]*/
var temp_ar = [ 1, 1, 1, 0.99985, 0.85, 0.82, 0.26, 0.18, 0.055, 0.05, 0, 0, 0, 0 ];

temp_ar[-1] = 1;
temp_ar[14] = 0;

var temp_arr = [];
for( i=0; i<7; i++ ) {
	temp_arr[i] = [];
	for( c=0; c<14; c++ ) {
		var k = (Math.random()+Math.random()+Math.random()+Math.random())/4;
		temp_arr[i][c] = temp_ar[c+1] + ( temp_ar[c-1] - temp_ar[c+1] ) * k;
	};
	var good = true;
	for( j=0; j<13; j++ )
		good = good && (temp_arr[i][j]>=temp_arr[i][j+1]);
	if( !good )
		--i;
};

temp_arr["default"] = temp_ar;
//temp_arr["reduced"] = temp_ar_reduced;
//temp_arr["gradient"] = temp_ar_reduced_gradient;

function http_bench_default() {	http_test( "default", false );	};
//function http_bench_reduced() {	http_test( "reduced", true );	};
//function http_bench_gradient() {	http_test( "gradient", false );	};

function http_bench_0() {	http_test( 0, false );	};
http_bench_0.title = 'http_ai_'+JSON.stringify(temp_arr[0])+'_default';

function http_bench_1() {	http_test( 1, false );	};
http_bench_1.title = 'http_ai_'+JSON.stringify(temp_arr[1])+'_default';

function http_bench_2() {	http_test( 2, false );	};
http_bench_2.title = 'http_ai_'+JSON.stringify(temp_arr[2])+'_default';

function http_bench_3() {	http_test( 3, false );	};
http_bench_3.title = 'http_ai_'+JSON.stringify(temp_arr[3])+'_default';

function http_bench_4() {	http_test( 4, false );	};
http_bench_4.title = 'http_ai_'+JSON.stringify(temp_arr[4])+'_default';

function http_bench_5() {	http_test( 5, false );	};
http_bench_5.title = 'http_ai_'+JSON.stringify(temp_arr[5])+'_default';

function http_bench_6() {	http_test( 6, true );	};
http_bench_6.title = 'http_ai_'+JSON.stringify(temp_arr[6])+'_default';

function http_bench_7() {	http_test( 2, true );	};
http_bench_7.title = 'http_ai_'+JSON.stringify(temp_arr[2])+'_0.35';

function http_bench_8() {	http_test( 3, true );	};
http_bench_8.title = 'http_ai_'+JSON.stringify(temp_arr[3])+'_0.35';

function http_bench_9() {	http_test( 4, true );	};
http_bench_9.title = 'http_ai_'+JSON.stringify(temp_arr[4])+'_0.35';


var match_participants = [
	//http_server_ai,
	andrej_move_0_0_3,
	andrej_move_0_0_4,
	andrej_move_0_0_5,
	andrej_move_0_0_6,
	http_bench_default,
	//http_bench_reduced,
	//http_bench_gradient,
	http_bench_0,
	http_bench_1,
	http_bench_2,
	http_bench_3,
	http_bench_4,
	//http_bench_5,
	//http_bench_6,
	//http_bench_7,
	//http_bench_8,
	//http_bench_9,
];

