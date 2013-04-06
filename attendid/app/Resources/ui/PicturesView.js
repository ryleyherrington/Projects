var MASlidingView = require('lib/MASlidingView');

var PicturesView = function(args){
	var view = new MASlidingView({
		backgroundColor:'#EBEBEB'
	});
	
	var swipeleft = Ti.UI.createLabel({
		text : 'Swipe right for menu',
		backgroundColor: "#EBEBEB",
		color : 'black',
		font : {fontSize:12},
		height : 25,
		top : 0,
		textAlign : 'center'
	});
	view.add(swipeleft);
	
	var pictures = Ti.UI.createWebView({
		url : 'http://ryleyherrington.com/attendid/pictures.html',
		top: 30
	});
	pictures.addEventListener('load', function(e) {
		Ti.API.info('webview loaded: '+ e.url);
	});
	view.add(pictures);
  
	return view;
};

module.exports = PicturesView;