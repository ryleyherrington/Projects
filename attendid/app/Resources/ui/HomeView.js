var MASlidingView = require('lib/MASlidingView');

var HomeView = function(args){
	var self = new MASlidingView({
		//backgroundColor:'#EBEBEB'
		backgroundColor:'#00bfff'
	});
	var swipeleft = Ti.UI.createLabel({
		text : 'Swipe right for menu',
		color : 'white',
		font : {fontSize:12},
		height : 25,
		top : 0,
		textAlign : 'center'
	});
	self.add(swipeleft);
	
	var searchTextField = Titanium.UI.createTextField({
		color:'#00bfff',
		hintText:'Enter and event hashtag', 
		height:35,
		top:35,
		left:10,
		width:200,
		borderStyle:Titanium.UI.INPUT_BORDERSTYLE_ROUNDED,
		hintText:'#event-name'
	});
	self.add(searchTextField);
	
	var searchButton = Ti.UI.createButton({
		color:'#00bfff',
		top:35,
		left:220,
		width: 90,
		height: 35,
		title:'Search'
	});
	self.add(searchButton);
 function friendTags(){
	var xhr = Titanium.Network.createHTTPClient();
	xhr.onerror = function(e) {
		alert("Looks like we can't get an internet connection. Options: \n Try to close this app and reopen it.\n Try to connect to the wifi. \n Give up and move away. Maybe to a beach."); 
	}; 

	xhr.onload = function() {
		resultsData = [];
		var results = JSON.parse(this.responseText);
		for (var i=0; i < results.results.length; i++) {
			var row = Titanium.UI.createTableViewRow({
				msg:results.results[i].text,
				//backgroundColor:'#EBEBEB'
				backgroundColor:'#F9C396',
				height:50,
				width:320
			});
			var imageView = Titanium.UI.createImageView({
				image:results.results[i].profile_image_url,
				height:64,
				hires:true,
				width:43,
				left:5
			});
			var tweet = Titanium.UI.createLabel({
				text:results.results[i].text,
				left:65,
				color:'white',
				selectedColor:'#F9C396',
				font:{fontSize:14, fontWeight:'bold', fontFamily:'Helvetica Neue'},
				width:'auto',
				height:'auto'
			});
			row.add(imageView);
			row.add(tweet);
			resultsData.push(row);
		};
		var resultsTableView = Ti.UI.createTableView({
			data:resultsData,
			top:85 //55
		});
			function showOtherClickEventInfo(e) {
				var msg = e.rowData.msg;
				var alert = Titanium.UI.createAlertDialog({
					title:msg,
					message:''
				});
				alert.show();					
			}
		
		resultsTableView.addEventListener('click', function(e){
			showOtherClickEventInfo(e);
		});
		
		self.add(resultsTableView);
		resultsWin.open({modal:true});
	};

	// open the client
	var search = searchTextField.value;
	if (search == ''){
		search = '#portland';
	}
	//xhr.open('GET','http://search.twitter.com/search.json?q=' + Titanium.Network.encodeURIComponent("#" + search) + '&rpp=100&include_entities=true&result_type=mixed');
	xhr.open('GET','http://search.twitter.com/search.json?q=' +
			 Titanium.Network.encodeURIComponent(search) +
			 '&rpp=100&include_entities=true&result_type=mixed');
	// send the data
	xhr.send();
	}
	searchButton.addEventListener('click', function(e) {
   		friendTags();
	});

	friendTags();
	return self;
};

module.exports = HomeView;
