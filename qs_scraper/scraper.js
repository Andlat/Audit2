//Scrape questions from https://www.triviaquestionsnow.com/
//The questions only have the answer, so the other options have to be manually created

//#### Show the questions. Not needed for the scraper ####
let elmnt = document. getElementsByClassName('click-to-show')
var c=0;t = setInterval(()=>{ elmnt[c].click(); ++c; if(c == elmnt.length) clearInterval(t);},100);


//#### Scrape the questions ####
//#### String with data is printed in the console ####
let q = document. getElementsByClassName('question')
let data = "";

for(let i=0; i < q.length; ++i){
	let r = String.fromCharCode(65+ ((Math.random() * 10) % 4));
	
	let diff = q[i].children[1].innerHTML.trim();
	let qs = q[i].children[2].children[0].innerHTML.trim();
	let rs = q[i].children[3].children[2].children[3].innerHTML.trim();
	
	data += diff + '\n' + qs + '\n';
	
	for(let j=0; j < 4; ++j){
		data += String.fromCharCode(j+65) + ') ';
		
		if(j==r.charCodeAt(0)-65)
			data += rs;
			
		data += '\n';
	}
	data += r + '\n';
}

console.log(data);