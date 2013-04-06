var MASlidingMenu = require('/lib/MASlidingMenu');
var HomeView = require('/ui/HomeView');
var PicturesView = require('/ui/PicturesView');
var MenuView = require('/ui/MenuView');
var TweetView = require('/ui/TweetView');
var FeedbackView = require('ui/FeedbackView');

var home = new HomeView();
var pictures = new PicturesView();
var post = new TweetView();
var feedback = new FeedbackView();
// Each row with a view property when clicked will change to that view (any view works except tabgroups and windows)
// If the row does not have a view property, but the switch event still fires
var data = [
	{ title:'Feed', hasDetail:true, view: home },
	{ title:'Pictures', hasDetail:true, view: pictures },
	{ title:'Post', hasDetail:true, view: post },
	{ title:"Feedback", hasDetail:true, view: feedback}
];

var menu = new MenuView({
	rowData: data
});

var slidingMenu = new MASlidingMenu({
	left: menu, // the menu... only accepts a tableview
	draggable: true // set false to only use the API to open / close
});
slidingMenu.open();

// event fired when user selects a view from the nav
slidingMenu.addEventListener('buttonclick', function(e) {
	if (e.index === 3) { //Post button
		alert('You clicked on Button');
	}
});

// event fired when user selects a view from the nav
slidingMenu.addEventListener('switch', function(e) {
	//alert(e.menuRow);
	//alert(e.index);
	//alert(e.view); // This is the new view your switching to
});

// event fired while user is dragging the view to expose the menu
slidingMenu.addEventListener('sliding', function(e) {
	//alert(e.distance);
});

//Expose / close the menu programaticly
	//slidingMenu.slideView('left');
	//slidingMenu.slideView('view');

// Access the currently displayed view
	//slidingMenu.activeView();

