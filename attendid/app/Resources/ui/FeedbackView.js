var MASlidingView = require('lib/MASlidingView');

var FeedbackView = function(args){
	var feed_view = new MASlidingView({
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
	feed_view.add(swipeleft);
	
	var pictures = Ti.UI.createWebView({
		url : 'http://ryleyherrington.com/attendid/app-contact.php',
		top: 30
	});
	
	pictures.addEventListener('load', function(e) {
		Ti.API.info('webview loaded: '+ e.url);
	});
	
	feed_view.add(pictures);
  
	return feed_view;
};

module.exports = FeedbackView;