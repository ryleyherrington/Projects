var MASlidingView = require('lib/MASlidingView');

var TweetView = function(args){
	var tweetwin = new MASlidingView({
		backgroundColor:'#EBEBEB'
	});
	var shareButton = Ti.UI.createButton({
	    width: 90, 
	    bottom: 10, 
	    height: 30,
	    title: 'Tweet!'
	});
	tweetwin.add(shareButton);
	
	var msg = Ti.UI.createTextArea({
		borderWidth: 2,
		borderColor: '#bbb',
		borderRadius: 5,
		color: '#888',
		font: {fontSize:20, fontWeight:'bold'},
		//keyboardType: Ti.UI.KEYBOARD_NUMBER_PAD,
		//returnKeyType: Ti.UI.RETURNKEY_GO,
		textAlign: 'left',
		value: '',
    	hintText:'Tweet here...', 
		top: 20,
		width: 300, 
		height : 70
	});
	tweetwin.add(msg);
	
	/*var Picker = Ti.UI.createPicker({
		selectionIndicator : true
	});

	var data = [];
		data[0] = Ti.UI.createPickerRow({title:' #Attendid', custom_item:'a'});
		data[1] = Ti.UI.createPickerRow({title:' #AttendidJamesons', custom_item:'j'});
		data[2] = Ti.UI.createPickerRow({title:' #AttendidJohnhenrys', custom_item:'jh'});
		data[3] = Ti.UI.createPickerRow({title:' #AttendidDavis', custom_item:'d'});		
		data[4] = Ti.UI.createPickerRow({title:' #AttendidHorseHead', custom_item:'hh'});
		data[5] = Ti.UI.createPickerRow({title:' #AttendidLevelup', custom_item:'lu'});
		data[6] = Ti.UI.createPickerRow({title:' #AttendidTaylors', custom_item:'t'});
		data[7] = Ti.UI.createPickerRow({title:' #AttendidCowfish', custom_item:'c'});

	Picker.add(data);
	
	Picker.addEventListener('change', function(e) {
		//alert('You selected: row:' + e.row.title + ', column: ' + e.column + ', custom_item: ' + e.row.custom_item);
		event_title = e.row.title;
		event_title = event_title.replace('[object TiUITextArea]', '');
	});
	tweetwin.add(Picker);
	
	*/
	var event_title = '';
	
	var tableData = [];

var table = Ti.UI.createTableView({ objName: 'table' });

for (var i = 0; i <= 20; i++){
  var row = Ti.UI.createTableViewRow({
    className: 'row',
    objName: 'row',
    touchEnabled: true,
    height: 100
  });
  
  var enabledWrapperView = Ti.UI.createView({
    backgroundColor:'#008FD5',
    objName: 'enabledWrapperView',
    rowID: i,
    width: Ti.UI.FILL, height: '100%'
  });
  
  var disabledWrapperView = Ti.UI.createView({
    backgroundColor:'#A2E0FF',
    objName: 'disabledWarpperView',
    touchEnabled: false,
    width: 300, height: '80%'
  });
  enabledWrapperView.add(disabledWrapperView);
  
  var label = Ti.UI.createLabel({
    backgroundColor:'#313F48',
    color: 'white',
    objName: 'label',
    text: i,
    touchEnabled: false,
    left: 0,
    width: 200
  });
  disabledWrapperView.add(label);
  
  row.add(enabledWrapperView);
  tableData.push(row);
}

table.setData(tableData);

table.addEventListener('swipe', function(e){
  if (e.source && e.source.objName !== 'table'){
    //Ti.API.info('Row swiped: ' + e.source);
    //Ti.API.info('Row swiped: ' + e.source.objName);
    Ti.API.info('Row ID : ' + e.source.rowID);
  }
});

tweetwin.add(table);
	
	var social = require('social');
	var twitter = social.create({
	    site: 'Twitter', 
	    consumerKey: 'mFpsRS3vFTFbu23LEKz7Q', 
	    consumerSecret: 'IKwYGYa3GrW68425bt1SzXEMAdTPjlRMvF51bTnYS4' 
	});
	
	shareButton.addEventListener('click', function() {
		//var lengthtest = msg.value + event_title + ' via @ttendid'
	    var alert = Titanium.UI.createAlertDialog({
			title:"Are you sure you want to tweet this:",
			buttonNames:['Yes', 'No'], 
			message: msg.value + event_title + ' via @ttendid'
		});
		alert.addEventListener('click', function(ev) {
    		if (ev.index == 0) { // clicked "Yes"
				twitter.share({
       				message: msg.value + event_title + ' via @ttendid', 
					success: function() {
						INFO('Tweeted!');
					},
					error: function(error) {
						INFO('Oh no! ' + error);
					}
				});
			}else if (ev.index == 1) { // clicked "No"
				alert.close;
			} 
 		});
		alert.show();							
	});
	
	
	return tweetwin;
};

module.exports = TweetView;
