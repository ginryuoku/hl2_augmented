$(document).ready(function() {

	$(function(){ //DOM Ready
	
		$(".gridster ul").gridster({
		widget_margins: [5, 5],
		widget_base_dimensions: [60, 60],
		min_cols: 16,
		max_cols: 16,
		min_rows: 8
		});
	
	});
});