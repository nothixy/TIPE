const process = require('process');
const fs = require('fs');
if (process.argv.length != 3) {
	console.log("Nom de fichier requis en entrée");
	process.exit(1);
}
var data = fs.readFileSync(process.argv[2]);
console.log(JSON.parse(data));
