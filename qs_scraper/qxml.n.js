let libxml = require('libxmljs');
let fs = require('fs').promises;

if(process.argv.includes('--help')){
	console.log('node qxml -f questions.txt -xml data.xml [--append][--help]\n\t-f\t\tfile containing the questions in a txt format\n\t-xml\t\toutput file\n\t--append\tappend the new ml data to existing one in the xml output file\n\t--help\t\tshows this help screen');
		return 0;
}

if(!process.argv.includes('-xml', '-f')){
	console.log("Please input all file arguments: -f questions.txt -xml data.xml");
	return 0;
}

const parser = {
	getXml: (xmlData)=>{
		let xml = libxml.parseXmlString(xmlData, { noblanks: true });
		return Promise.resolve(xml);
	},
	orderQuestionsWithID: (xml)=>{
		let lvl = xml.root().childNodes();
		let ID = 1;

		for(let key in lvl){
			lvl[key].childNodes().map(q =>{
					q.attr({id:ID});
					++ID;
				});
		}
		return Promise.resolve(xml);
	},
	convertQsToXml: (data)=>{
		let lvl = {easy:'easy', medium:'medium', hard:'hard', veryhard:'veryhard'}

		let xml = new libxml.Document();
		let xmlRoot = xml.node('root');

		//Create the difficulty nodes
		let lvlNodes = {easy:xmlRoot.node(lvl.easy), medium: xmlRoot.node(lvl.medium), hard: xmlRoot.node(lvl.hard), veryhard:xmlRoot.node(lvl.veryhard)};

		//Split the questions line by line
		data = data.split('\n').map(s => s.trim());

		//Create the XML for the questions
		for(let i=0; i < parseInt(data.length/7); ++i){//7 lines per question
			let j=i*7;
			let lvl_key = data[j].toLowerCase();

			let q = lvlNodes[lvl_key].node('question').attr({lang:'en', lvl:lvl_key});
			q.node('statement', data[j+1]);

			let a = q.node('answers');

			let correctAnswerOffset = data[j+6].charCodeAt(0)-65;
			let s = data[j+2+correctAnswerOffset];
			a.node('correct', s.substring(3,s.length));
			for(let k=0; k < 4; ++k){
				if(k == correctAnswerOffset) continue;

				s = data[j+2+k];
				a.node('wrong', s.substring(3,s.length));
			}
		}

		return Promise.resolve(xml);
	},
	write: (data)=>{
		fs.writeFile(process.argv[process.argv.indexOf('-xml')+1], data)
		.catch(err=>{
			console.error(err);
			return -3;
		});
	}
}

if(!process.argv.includes('--append')){//Just write to file
	fs.readFile(process.argv[process.argv.indexOf('-f')+1], 'utf-8')
	.then(parser.convertQsToXml)
	.then(parser.orderQuestionsWithID)
	.then(parser.write)
	.catch(err=>{
		console.error(err);
		return -2;
	});
}else{//Special case if need to append to existing file
	fs.readFile(process.argv[process.argv.indexOf('-xml')+1], 'utf-8')//Get data from existing xml file
	.then(parser.getXml)
	.then(xml=>{
		let nb = xml.root().childNodes().length;

		fs.readFile(process.argv[process.argv.indexOf('-f')+1], 'utf-8')//Parse questions from the txt file
		.then(parser.convertQsToXml)
		.then(newXml=>{
			//Append new xml to the existing data
			let root = xml.root();
			let newNodes = newXml.root().childNodes();
			for(let i=0; i < newNodes.length; ++i){
				let nextNode = newNodes[i].attr({'id': ++nb})
				root.addChild(nextNode);
			}

			parser.write(xml);
		})
		.catch(err=>{
			console.error(err);
			return -2;
		});
	})
	.catch(err=>{
		console.error(err);
		return -1;
	});
}
