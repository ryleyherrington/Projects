Titanium.UI.setBackgroundColor('#000');

var tabGroup = Titanium.UI.createTabGroup();

var win1 = Titanium.UI.createWindow({  
    title:'Sentence Structures',
    backgroundColor:'#fff',
    barColor:'teal' 
});

var tab1 = Titanium.UI.createTab({  
    icon:'sentences.png',
    title:'Sentences',
    window:win1
});

var data = [
//hasChild:true puts >
//hasDetail:true puts >in blue circle
//hasCheck:true puts check mark
	{title:'Two Adjectives', 
		msg: 'This is when you begin a sentence with two adjectives followed by the noun they describe. \n\n Regal, aloof peacocks strut... \n\n Charming, agressive salesmen fill the room...    ', 
		color:'teal', selectedColor:'#fff'},
	{title:'Delayed Adjectives', 
		msg:'When using adjectives between the noun and the verb at the beginning of a sentence. \n\n Peacocks, regal and aloof, strut...\n\n Salesmen, exuberant and flashy, speak with...', 
		color:'teal', selectedColor:'#fff'},
	{title:'An Appositive', 
		msg: 'An appositive is a noun or noun phrase that renames another noun right beside it.\n\n  Perry, the regal, aloof peacock, flirts...\n\n The insect, a cockroach, is crawling across the kitchen table.',
		color:'teal', selectedColor:'#fff'},
	{title:'A Parallel Structure', 
		msg:'Parallel structure means using the same pattern of words to show that two or more ideas have the same level of importance. This can happen at the word, phrase, or clause level.\n\nRegal and aloof, Perry struts...\n\n Whimsical and excited, Betty dances...', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Question', 
		msg:'Simply using a question as a sentence.\n\n  Where could that regal, aloof peacock be? \n\n Why would that massive, kindhearted dog run away?', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Prepositional Phrase', 
		msg:"At the minimum, a prepositional phrase will begin with a preposition and end with a noun, pronoun, gerund, or clause, the 'object' of the preposition.\n\nIn our garden's shade the regal, aloof peacock stands...\n\n At the store the elegant, accessible salesperson mills...", 
		color:'teal', selectedColor:'#fff'},
	{title:'An Infinitive', 
		msg:'An infinitive will almost always begin with to followed by the simple form of the verb. \n\nTo strut all day in the garden is the joy of the peacock...\n\n To sleep in on Monday is the goal of many students...', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Gerund', 
		msg:"Every gerund ends in -ing. Gerunds function as nouns and will be subjects, subject complements, direct objects, indirect objects, and objects of prepositions.\n\nStrutting all day in the garden, watching the other birds, is the peacock's teal...\n\n Writing all day is a great pastime...", 
		color:'teal', selectedColor:'#fff'},
	{title:'A Perfect Infinitive', 
		msg:'These sentences start with "To have" + a past participle. \n\nTo have strutted all day in the garden was the true right of the peacocks...\n\n To have sold cars all day was the dream of...', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Perfect Participle', 
		msg:'The perfect participle indicates a completed action. You form the perfect participle by putting the present participle having in front of the past participle.\n\nHaving strutted all day in the garden, the peacock left for a cool bath...\n\n Having finished the test, he left the room.', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Present Participle', 
		msg:'A present participle is used with the verb "To Be" to indicate an action that is incomplete. Ex: I am running / I was running.\n\nSpotting the running children, the peacock makes his exit... \n\n Giggling heartily, the children forgot...', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Perfect Gerund', 
		msg:'The past form of gerunds are formed by using "having" + a past participle. This kind of gerund suggests that the gerund happened before something else.\n\nHaving strutted all day satisfied the peacock...\n\n Having met him earlier in life...', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Predicate Adjective',
		msg:'An adjective that usually comes after a linking verb and not before a noun.\n\nThe peacock was regal and aloof...\n\n The salesman was animated and funny...', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Predicate Noun', 
		msg:'A predicate noun is a single noun or a noun phrase that renames the subject of a sentence and follows a form of the verb “to be” or another linking verb.\n\n A regal, aloof bird was the peacock...\n\n A cute, fluffy puppy was the dog... ', 
		color:'teal', selectedColor:'#fff'},
	{title:'Another Parallel Structure', 
		msg:'A parallel structure that begins with clauses must keep on with clauses. Changing the voice of the verb (from active to passive or vice versa) will break the parallelism.\n\n Wrong: A regal bird and an aloof one, the peacock loves admirers and hated other peacocks...\n\n Right: Mary likes to hike, to swim, and to ride a bicycle.', 
		color:'teal', selectedColor:'#fff'},
	{title:'An Adverbial Clause', 
		msg:'An adverb clause begins with a subordinate conjunction (such as if, when, because, or although) and includes a subject and a predicate.\n\n While he strutted in the garden, the peacock kept watching for intruders...\n\n If Harry had left Westchestertonville, Mrs. Starboard would have been unhappy.', 
		color:'teal', selectedColor:'#fff'},
	{title:'An Adjective Clause', 
		msg:'It will contain a subject and verb, it will begin with a relative pronoun or a relative adverb. Usually answers these questions: What kind? How many? or Which one? \n\n An adjective clause does not express a complete thought, so to avoid writing a fragment you must connect each adjective clause to a main clause. \n\n Perry, who had all the colors of the rainbow, strutted in the garden...\n\n Laughter erupted from the teacher, who loved sentences which extol the virtues of English teachers.', 
		color:'teal', selectedColor:'#fff'},
	{title:'A Noun Clause', 
		msg:'A dependent clause that functions as a noun (that is, as a subject, object, or complement) within a sentence. \n\n That the peacock inspires love is actually myth in many cultures...\n\n English teachers dispense wisdom to anyone that will listen.', 
		color:'teal', selectedColor:'#fff'},
	{title:'An Exclamation', 
		msg:"A sentence type that is used to express a strong emotional state. \n\n Wow! That peacock's cry is loud.\n\n Amazing! I love English so much!", 
		color:'teal', selectedColor:'#fff'}
];

var tableview = Titanium.UI.createTableView({
	data:data,
	backgroundColor:'white'
});

function showClickEventInfo(e) {
	var row = e.row;
	var msg = e.rowData.msg;
	Titanium.UI.createAlertDialog({title:e.rowData.title,ok:'Got it', message:msg}).show();
}

tableview.addEventListener('click', function(e) {
	showClickEventInfo(e);
});
tableview.addEventListener('longclick', function(e) {
	showClickEventInfo(e, true);
});

win1.add(tableview);

var win2 = Titanium.UI.createWindow({  
    title:'Language Tips',
    backgroundColor:'#fff',
    barColor:'teal' 
});
var tab2 = Titanium.UI.createTab({  
    icon:'light_thumb-up.png',
    title:'Tips',
    window:win2
});
var tips = [
	{title:"Avoid these:",hasChild:true, msg: "Is, am, are, was, were, seems, appears, looks like.", color:'teal', selectedColor:'#fff'},
	{title:"Use these:",hasChild:true, msg: "Argues, asserts, concludes, considers, discusses, emphasizes, examines, explores, focuses on, implies, mentions, notes, points out, states, suggests.", color:'teal', selectedColor:'#fff'},
	{title:"Splitting Infinitives", msg: "The general rule is that no word should separate the to of an infinitive from the simple form of the verb that follows. \n\n Wrong: Jenny hopes to quickly finish her homework. \n\n Right: Jenny hopes to finish her homework quickly. ", color:'teal', selectedColor:'#fff'},
//	{title:"", msg: "", color:'teal', selectedColor:'#fff'},
	{title:"Oxford Comma", msg: "These are used immediately before a coordinating conjunction (usually and, or, and sometimes nor) preceding the final item in a list of three or more items. Example: a list of three countries can be punctuated as either 'Portugal, Spain, and France' (with the serial comma) or as 'Portugal, Spain and France'", color:'teal', selectedColor:'#fff'},
	{title:'Comma Splice', 
		msg:'A comma splice is an error caused by joining two strong clauses with only a comma instead of separating the clauses with a conjunction, a semicolon, or a period. A run-on sentence, which is incorrect, is created by joining two strong clauses without any punctuation. \n\n Incorrect:Time flies when we are having fun, we are always having fun. (Comma splice)\n\n Time flies when we are having fun we are always having fun.(Run-on) \n\n Time flies when we are having fun; we are always having fun!', 
		color:'teal', selectedColor:'#fff'},
	{title:"It's", msg: "It's- this is a contraction for 'it is' or 'it has'. If you can replace it's with 'it is' or 'it has' then you can use it.", color:'teal', selectedColor:'#fff'},
	{title:"Its", msg: "Its is a possessive pronoun meaning of it or belonging to it. Replace it with her/him to see if it still makes sense.", color:'teal', selectedColor:'#fff'},
	{title:"There", msg: "Use there when referring to a place, whether concrete ('over there by the building') or more abstract ('it must be difficult to live there').\n\n Also use there with the verb BE (is, am, are, was, were) to indicate the existence of something, or to mention something for the first time.\n\n If you wrote there, will the sentence still make sense if you replace it with here? If so, you're using it correctly.", color:'teal', selectedColor:'#fff'},
	{title:"Their", msg: "This version presents ownership. It's their shirt.\n\n If you chose their, will the sentence still make sense if you replace it with our? If so, you've chosen the correct word.", color:'teal', selectedColor:'#fff'},
	{title:"They're", msg: "Contraction for 'they are.' If you can replace the word with 'they are' and it makes sense, use it.", color:'teal', selectedColor:'#fff'},
	{title:"Your", msg: "Your is a possessive adjective. It describes a noun by telling us to whom it belongs. \n\n Ex: I love your new dress", color:'teal', selectedColor:'#fff'},
	{title:"You're", msg: "You're is actually a contraction of 'you are.' \n\nEx: You're cute.", color:'teal', selectedColor:'#fff'},
	{title:"Were", msg: "2nd person indicative, plural past indicative, and past subjunctive of the verb 'be'.", color:'teal', selectedColor:'#fff'},
	{title:"We're", msg: "A contraction for the words 'we are.'", color:'teal', selectedColor:'#fff'},
	{title:"Where", msg: "To what place, point, or end. \n\n Where are we going?", color:'teal', selectedColor:'#fff'},
	{title:"Lose", msg: "To come to be without, or to fail. The opposite of win.", color:'teal', selectedColor:'#fff'},
	{title:"Loose", msg: "Something that doesn't fit, something unbound.", color:'teal', selectedColor:'#fff'},
	{title:"To", msg: "Use to as a preposition before a noun or as an infinitive before a verb.\n\n Ex: Please take me to the store.", color:'teal', selectedColor:'#fff'},
	{title:"Too", msg: "Use too as a synonym for also or to indicate excessiveness before a verb. Usually, if you can replace 'too' with 'also' in the same sentence then you are correct in using it.", color:'teal', selectedColor:'#fff'},
	{title:"Two", msg: "Use two to spell out the number 2. If you can replace 'two' with '2' in the same sentence, then you are using it correctly.", color:'teal', selectedColor:'#fff'},
	{title:"Weird", msg: "I before E except after C and when sounding like 'eigh' as in neighbor or weigh. Not wierd.", color:'teal', selectedColor:'#fff'},
	{title:"Definitely", msg: "Without doubt (used for emphasis). There is no 'A' in this word.", color:'teal', selectedColor:'#fff'},
	{title:"Effect", msg: "Usually used as a noun. Replace 'effect' with a noun and it still makes sense, then you're using it the right way.", color:'teal', selectedColor:'#fff'},
	{title:"Affect", msg: "Usually acts as a verb, if you can replace 'affect' with another verb and it makes sense then you're using it correctly.", color:'teal', selectedColor:'#fff'},
	{title:"Whether", msg: "Expressing a doubt or choice between alternatives: 'he seemed undecided whether to go or stay'.", color:'teal', selectedColor:'#fff'},
	{title:"Weather", msg: "The state of the atmosphere at a place and time as regards heat, cloudiness, dryness, sunshine, wind, rain, etc.", color:'teal', selectedColor:'#fff'},
	{title:"A lot", msg: "Change the word 'lot' to 'bunch' and decide if you would say 'abunch' or 'a bunch'. Alot is a town in India, not a way to describe an abundance of something.", color:'teal', selectedColor:'#fff'},
	{title:"Separate", msg: "Things forming units by themselves, or a verb that means to cause to move or be apart. Seperate is not spelled correctly.", color:'teal', selectedColor:'#fff'},
	{title:"Then", msg: "'Then' is used for a time. Ex: I ran to school, then I quickly finished my homework before class.", color:'teal', selectedColor:'#fff'},
	{title:"Than", msg: "'Than' is a comparison between two things. I'm better at writing essays than most peacocks.", color:'teal', selectedColor:'#fff'},
	{title:"Except", msg: "Usually used as a noun. Replace it with a 'result' in your mind to understand it. Ex: The sound effects were amazing, or The rain had no effect on Ryley's hairdo.", color:'teal', selectedColor:'#fff'},
	{title:"Accept", msg: "Usually used as a verb. Affect with an a means 'to influence,' as in, 'The arrows affected the dog,' or 'The rain affected Ryley's hair.' Affect can also mean, roughly, 'to act in a way that you don't feel,' as in, 'She affected an air of superiority.'", color:'teal', selectedColor:'#fff'},
];

var tableview = Titanium.UI.createTableView({
	data:tips
});
function showClickEventInfo(e) {
	var row = e.row;
	var msg = e.rowData.msg;
	Titanium.UI.createAlertDialog({title:e.rowData.title,ok:'Done', message:msg}).show();
}

tableview.addEventListener('click', function(e) {
	showClickEventInfo(e);
});
tableview.addEventListener('longclick', function(e) {
	showClickEventInfo(e, true);
});

win2.add(tableview);

var win3 = Titanium.UI.createWindow({  
    title:'Tone Descriptors',
    backgroundColor:'#fff',
    barColor:'teal'
});
var tab3 = Titanium.UI.createTab({  
    icon:'tone.png',
    title:'Tone',
    window:win3
});
var tone = [
	{title:"Aggressive", msg: "1. Ready or likely to attack or confront; characterized by or resulting from aggression. \n\n 2.Pursuing one's aims and interests forcefully.", color:'teal', selectedColor:'#fff'},
	{title:'Angry', msg:'Having a strong feeling of or showing annoyance, displeasure, or hostility; full of anger.', color:'teal', selectedColor:'#fff'},
	{title:'Annoyed', msg:'1. Irritate (someone); make (someone) a little angry. \n\n 2. Harm or attack repeatedly.', color:'teal', selectedColor:'#fff'},
	{title:'Anxious', msg:'Experiencing worry, unease, or nervousness, typically about an imminent event or something with an uncertain outcome.', color:'teal', selectedColor:'#fff'},
	{title:'Bitter', msg:'Adjective: Having a sharp, pungent taste or smell; not sweet. \n\n Noun: Beer that is strongly flavored with hops and has a bitter taste.', color:'teal', selectedColor:'#fff'},
	{title:'Bold', msg:'Adjective: (of a person, action, or idea) Showing an ability to take risks; confident and courageous.\n\n Noun: A typeface with thick strokes.', color:'teal', selectedColor:'#fff'},
	{title:'Cheerful', msg: 'Noticeably happy and optimistic.',color:'teal', selectedColor:'#fff'},
	{title:'Concerned', msg:'Worried, troubled, or anxious.', color:'teal', selectedColor:'#fff'},
	{title:'Condescending', msg:'Acting in a way that betrays a feeling of patronizing superiority.', color:'teal', selectedColor:'#fff'},
	{title:'Curious', msg:'Eager to know or learn something.', color:'teal', selectedColor:'#fff'},
	{title:'Depressed', msg:'In a state of general unhappiness or despondency.', color:'teal', selectedColor:'#fff'},
	{title:'Determined', msg:'Having made a firm decision and being resolved not to change it.', color:'teal', selectedColor:'#fff'},
	{title:'Discouraged', msg:'Cause (someone) to lose confidence or enthusiasm.', color:'teal', selectedColor:'#fff'},
	{title:'Disinterested', msg:'Not influenced by considerations of personal advantage.', color:'teal', selectedColor:'#fff'},
	{title:'Enthusiastic', msg:'Having or showing intense and eager enjoyment, interest, or approval.', color:'teal', selectedColor:'#fff'},
	{title:'Frightened', msg:'Afraid or anxious.', color:'teal', selectedColor:'#fff'},
	{title:'Frustrated', msg:'Feeling or expressing distress and annoyance, esp. because of inability to change or achieve something.', color:'teal', selectedColor:'#fff'},
	{title:'Furious', msg:'Extremely angry.', color:'teal', selectedColor:'#fff'},
	{title:'Helpful', msg:'Giving or ready to give help.', color:'teal', selectedColor:'#fff'},
	{title:'Humorous', msg:'Causing lighthearted laughter and amusement; comic', color:'teal', selectedColor:'#fff'},
	{title:'Indecisive', msg:'Not settling an issue.', color:'teal', selectedColor:'#fff'},
	{title:'Indifferent', msg:'Having no particular interest or sympathy; unconcerned.', color:'teal', selectedColor:'#fff'},
	{title:'Injured', msg:'Suffer harm or damage.', color:'teal', selectedColor:'#fff'},
	{title:'Inquisitive', msg:'Curious or inquiring.', color:'teal', selectedColor:'#fff'},
	{title:'Insincere', msg:'Not expressing genuine feelings.', color:'teal', selectedColor:'#fff'},
	{title:'Inspirational', msg:'Providing or showing creative or spiritual inspiration.', color:'teal', selectedColor:'#fff'},
	{title:'Interested', msg:'Showing curiosity or concern about something or someone.', color:'teal', selectedColor:'#fff'},
	{title:'Intimate', msg:'Adjective: Closely acquainted; familiar, close.\n\n Noun: A very close friend.', color:'teal', selectedColor:'#fff'},
	{title:'Ironic', msg:'1. Using or characterized by irony. \n\n 2. Happening in the opposite way to what is expected, thus typically causing wry amusement.', color:'teal', selectedColor:'#fff'},
	{title:'Irritated', msg:'1. Make (someone) annoyed, impatient, or angry. \n\n 2.Cause inflammation or other discomfort in (a part of the body).', color:'teal', selectedColor:'#fff'},
	{title:'Light-hearted', msg:'blithe, carefree and happy', color:'teal', selectedColor:'#fff'},
	{title:'Lively', msg:'Full of life and energy; active and outgoing.', color:'teal', selectedColor:'#fff'},
	{title:'Meditative', msg:'Of, involving, or absorbed in meditation or considered thought.', color:'teal', selectedColor:'#fff'},
	{title:'Negative', msg:'Adjective: Consisting in or Characterized by the absence rather than the presence of distinguishing features. \n\n Noun: A word or statement that expresses denial, or refusal. \n\n Exclamation: No: "Any snags in the plan? "Negative, sir."', color:'teal', selectedColor:'#fff'},
	{title:'Nervous', msg:'Easily agitated or alarmed; tending to be anxious; highly strung', color:'teal', selectedColor:'#fff'},
	{title:'Nosy', msg:"Showing too much curiosity about other people's affairs.", color:'teal', selectedColor:'#fff'},
	{title:'Offended', msg:'Cause to feel upset, annoyed, or resentful.', color:'teal', selectedColor:'#fff'},
	{title:'Opinionated', msg:"Conceitedly assertive and dogmatic in one's opinions.", color:'teal', selectedColor:'#fff'},
	{title:'Oppressed', msg:'Keep (someone) in subservience and hardship, esp. by the unjust exercise of authority.', color:'teal', selectedColor:'#fff'},
	{title:'Playful', msg:'Fond of games and amusement; lighthearted.', color:'teal', selectedColor:'#fff'},
	{title:'Positive', msg:'A good, affirmative, or constructive quality or attribute.', color:'teal', selectedColor:'#fff'},
	{title:'Resentful', msg:'Feeling or expressing bitterness or indignation at having been treated unfairly.', color:'teal', selectedColor:'#fff'},
	{title:'Sad', msg:'Feeling or showing sorrow; unhappy.', color:'teal', selectedColor:'#fff'},
	{title:'Sarcastic', msg:'Marked by or given to using irony in order to mock or convey contempt.', color:'teal', selectedColor:'#fff'},
	{title:'Serious', msg:'1. (of a person) Solemn or thoughtful in character or manner.\n\n 2.(of a subject, state, or activity) Demanding careful consideration or application.', color:'teal', selectedColor:'#fff'},
	{title:'Shocked', msg:'Cause (someone) to feel surprised and upset.', color:'teal', selectedColor:'#fff'},
	{title:'Sincere', msg:'Free from pretense or deceit; proceeding from genuine feelings.', color:'teal', selectedColor:'#fff'},
	{title:'Spirited', msg:'Full of energy, enthusiasm, and determination.', color:'teal', selectedColor:'#fff'},
	{title:'Upset', msg:'disquieted: afflicted with or marked by anxious uneasiness or trouble or grief.', color:'teal', selectedColor:'#fff'},
	{title:'Witty', msg:'Showing or characterized by quick and inventive verbal humor.', color:'teal', selectedColor:'#fff'},
	{title:'Worried', msg:"Give way to anxiety or unease; allow one's mind to dwell on difficulty or troubles.", color:'teal', selectedColor:'#fff'}
];

var tableview = Titanium.UI.createTableView({
	data:tone
});

function showClickEventInfo(e) {
	var row = e.row;
	var msg = e.rowData.msg;
	Titanium.UI.createAlertDialog({title:e.rowData.title,ok:'Done', message:msg}).show();
}


// create table view event listener
tableview.addEventListener('click', function(e) {
	showClickEventInfo(e);
});
tableview.addEventListener('longclick', function(e) {
	showClickEventInfo(e, true);
});

win3.add(tableview);

var win4 = Titanium.UI.createWindow({  
    title:'Punctuation',
    backgroundColor:'#fff',
    barColor:'teal'
});
var tab4 = Titanium.UI.createTab({  
    icon:'punctuation.png',
    title:'Punctuation',
    window:win4
});
var punctuation = [
	{title:'. Period', 
		msg:"1. Use a period at the end of a sentance that makes a statement, or at the end of a command. Ex: Hand in the homework no later than Friday. \n\n 2. Use a period at the end of an indirect question. Ex: The teacher asked why John had left out the harder exercises. \n\n Use a period with abbreviations. Ex: Did you enjoy living in Washington, D.C.?",
		color:'teal', selectedColor:'#fff'},
	{title:'? Question Mark', 
		msg:"1. Use a question mark at the end of the direct question. Ex: What are you doing today? \n\n 2. Use it for a tag question or a rhetorical question. Ex: He should start a diet, shouldn't he? \n\n 3. Use it for a series of brief questions. Ex: Who is responsible for the failure? The coach? The players? The staff?",
		color:'teal', selectedColor:'#fff'},
	{title:'! Exclamation Point', 
		msg:"1. Use an exclamation point at the end of an emphatic declaration, interjection, or command. Ex: 'No!' he yelled. 'Start it now!' \n\n 2. It can be used to close questions that are meant to convey extreme emotion. Ex: What are you thinking! Stop!",
		color:'teal', selectedColor:'#fff'},
	{title:'... Ellipsis', 
		msg:"1. An ellipsis can be used when omitting material from a quote. Ex: 'The graduation ceremony honored the twenty founding members... visiting the school.' \n\n 2. This is often used to indicate a pause in the flow of a sentence. Ex: Aimee thought and thought... and then thought some more before she finally came to a conclusion.",
		color:'teal', selectedColor:'#fff'},
	{title:'- Hyphen',
		msg:"A hyphen is used to join words and to separate syllables of a single word. Ex: I like twentieth-century writers. \n\n Beware that certain prefixes (co-, pre-, mid-, de-, non-, anti-, etc.) may or may not be hyphenated. ",
		color:'teal', selectedColor:'#fff'},
	{title:'"" Quotation Marks',
		msg:'Quotation marks are used to set off material that represents quoted or spoken language. Ex: The phrase "lovely, dark and deep" begins to suggest ominous overtones.',
		color:'teal', selectedColor:'#fff'},
	{title:';  Semi Colon', 
		msg:"1. Use a semicolon when trying to join two sentences that are related. \n\n Ex: Call me tomorrow; I will give you my answer then. \n\n 2)Use the semicolon to separate units of a series when one or more of the units contain commas. \n\n Ex: This conference has people who have come from Portland, Oregon; Austin, Texas; and Orlando, Florida.",
		color:'teal', selectedColor:'#fff'},
	{title:':  Colon', 
		msg: "1.Use the colon after a complete sentence to introduce a list of items.\n\n Ex:I want the following items: lettuce, tomatoes, and peppers. \n\n 2. Use the colon to introduce a direct quote that is more than three lines in length. \n\n Ex: The author illustrates this issue in the first chapter when he wrote: '...'",
		color:'teal', selectedColor:'#fff'},
	{title:',  Comma', 
		msg:"1.Use commas to separate words and word groups with a series of three or more. \n\n 2.Use a comma to separate two adjectives when the word and can be inserted between them.\n\n Ex: He is a strong, healthy man. \n Use commas to set off expressions that interrupt sentence flow.\n\n Ex: I am, as you have probably noticed, very nervous about this.",
		color:'teal', selectedColor:'#fff'},
];

var tableview = Titanium.UI.createTableView({
	data:punctuation
});
function showClickEventInfo(e) {
	var row = e.row;
	var msg = e.rowData.msg;
	Titanium.UI.createAlertDialog({title:e.rowData.title,ok:'Done',color:'black', message:msg}).show();
}

tableview.addEventListener('click', function(e) {
	showClickEventInfo(e);
});
tableview.addEventListener('longclick', function(e) {
	showClickEventInfo(e, true);
});
win4.add(tableview);

var win5 = Titanium.UI.createWindow({  
    title:'Parts of Speech',
    backgroundColor:'#fff',
    barColor:'teal'
});
var tab5 = Titanium.UI.createTab({  
    icon:'speech.png',
    title:'Parts of Speech',
    window:win5
});
var parts = [
	{title:"Adjective", msg: "Describes a noun. \n\nExamples: Angry, itchy, deafening, four, adorable, colossal, melodic, blue, broad, rainy.", color:'teal', selectedColor:'#fff'},
	{title:"Adverb", msg: "Describes a verb. \n\nExamples: Carefully, eagerly, nowhere, outside, in order to, purposely, always, sometimes, after, yesterday, here, there.", color:'teal', selectedColor:'#fff'},
	{title:"Conjunction", msg: "Joins clauses, sentences, or words.\n\nExamples: And, but, or, yet, for, nor, so", color:'teal', selectedColor:'#fff'},
	{title:"Interjection", msg: "A short exclamation, sometimes inserted into a sentence.\n\nExamples: Ah, alas, dear, hey, oh, ouch, uh, um, well.", color:'teal', selectedColor:'#fff'},
	{title:"Noun", msg: "Person, place, or thing.\n\nExamples: Furniture, ghost, giraffe, carpenter, calendar, England, school.", color:'teal', selectedColor:'#fff'},
	{title:"Preposition", msg: "Links a noun to another word. Tip: A preposition is something a dog can do to a log. \n\n A dog can jump OVER a log, dig UNDER a log, hop AROUNG a log, crawl THROUGH a hollow log...\n\n Examples: Before, against, except, following, over, through, underneath, ", color:'teal', selectedColor:'#fff'},
	{title:"Pronoun", msg:"Replaces a noun.\n\nExamples: I, you, he, she, it, they, we.", color:'teal', selectedColor:'#fff'},
	{title:"Relative Adverb", msg:"An adverb (where, when, or why) that introduces a relative clause. \n\n We hear little of the day-to-day successes but only of the odd occasion when conflict arises. 'When' is the relative adverb.", color:'teal', selectedColor:'#fff'},
	{title:"Relative Pronoun", msg: 'Relative pronouns: introduces a relative clause. It is called a "relative" pronoun because it "relates" to the word that it modifies\n\nExamples: That, which, who, whom, whose, whichever, whoever, whomever ', color:'teal', selectedColor:'#fff'},
	{title:"Verb", msg: "Action or state. \n\nExamples: Knit, hang, jog, notice, rinse, kick, promise, fix, obey, open, imagine, escape.", color:'teal', selectedColor:'#fff'},
];


var tableview = Titanium.UI.createTableView({
    data:parts
});
function showClickEventInfo(e) {
    var row = e.row;
    var msg = e.rowData.msg;
    Titanium.UI.createAlertDialog({title:e.rowData.title,ok:'Done',color:'black', message:msg}).show();
}

tableview.addEventListener('click', function(e) {
    showClickEventInfo(e);
});
tableview.addEventListener('longclick', function(e) {
    showClickEventInfo(e, true);
});
win5.add(tableview);

tabGroup.addTab(tab1);  
tabGroup.addTab(tab2);
tabGroup.addTab(tab3);
tabGroup.addTab(tab4);
tabGroup.addTab(tab5);
setTimeout(function() {
    tabGroup.open();
}, 2000);
// open tab group
//tabGroup.open();
