
var temp_temp_ar = [
/* 5 */	1, 1,
/* 4 */	1, 0.998,
/* 3 */	0.5, 0.5,
/* 2 */	0.5, 0.5,
/* 1 */	0.5, 0.5,
/* 0 */	0, 0,
	0, 0 ];

temp_temp_arr = [];
for( var i=0; i<300; i++ ) {
	temp_temp_arr[i] = [];
	var b = [];
	for( var j=0; j<6; j++ )
		b[j] = Math.random()*11 - 1;
	b.sort().reverse();
	for( var j=0; j<14; j++ )
		temp_temp_arr[i][j] = temp_temp_ar[j];
	for( var j=0; j<6; j++ )
		temp_temp_arr[i][j+4] = Math.pow(1.4,b[j]) / Math.pow(1.4,10);
};

function http_opts_temp( n ) {
	var opts = {};
	opts.ai_estimates = {
		"e5_me"    : temp_temp_arr[n][0],
		"e5_notme" : temp_temp_arr[n][1],
		"e4_me"    : temp_temp_arr[n][2],
		"e4_notme" : temp_temp_arr[n][3],
		"e3_me"    : temp_temp_arr[n][4],
		"e3_notme" : temp_temp_arr[n][5],
		"e2_me"    : temp_temp_arr[n][6],
		"e2_notme" : temp_temp_arr[n][7],
		"e1_me"    : temp_temp_arr[n][8],
		"e1_notme" : temp_temp_arr[n][9],
		"e0_me"    : temp_temp_arr[n][10],
		"e0_notme" : temp_temp_arr[n][11],
		"ex_me"    : temp_temp_arr[n][12],
		"ex_notme" : temp_temp_arr[n][13]
	};
	return opts;
};

function test_temp_next() {
	if( test_temp_next.i>=300 )
		return;

	var request_obj = {};
	request_obj.moves = [{"x":27,"y":12},{"x":28,"y":11},{"x":27,"y":11},{"x":29,"y":10},{"x":27,"y":10},{"x":28,"y":9},{"x":27,"y":8}];
	request_obj.options = http_opts_temp(test_temp_next.i);

	$.ajax( {
		url: "/noughtsandcrosses/code/",
		type: "post",
		data: JSON.stringify(request_obj)+"\0" ,
		success: function (response) {
			var r = response;
			if( r["error"]!=undefined )
				console.log('ошибка:  '+r.error);
			if( (response.x==27)&&(response.y==9) )
				console.log("success! parameters = "+JSON.stringify(temp_temp_arr[test_temp_next.i]));
		},
		error: function(jqXHR, textStatus, errorThrown) {
			alert('ошибка:  '+textStatus+'  '+errorThrown);
			console.log(textStatus, errorThrown);
		}
	});
	test_temp_next.i++;

};

test_temp_next.i = 0;

