$(document).ready(function() {

	$(function(){ //DOM Ready
	 
		$(".gridster ul").gridster({
		widget_margins: [5, 5],
		widget_base_dimensions: [50, 50],
		max_cols: 15,
		min_cols: 15,
		min_rows: 7
		});
	
	});
});