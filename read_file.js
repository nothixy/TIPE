const fs = require('fs');
const process = require('process');

if (process.argv.length <= 2) {
	console.log("Not enough arguments");
} else {
	let filename = process.argv[2];
	let data = fs.readFileSync(filename);
	let parsed = JSON.parse(data);
	console.log(parsed);
}
