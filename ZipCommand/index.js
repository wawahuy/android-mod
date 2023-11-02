const AdmZip = require('adm-zip');
const fs = require('fs');

const file = process.argv[2];
process.argv.splice(0, 3);
const mapping = process.argv; 

const zipFile = process.cwd() + '\\' + file;
console.log(zipFile);

const zip = new AdmZip(zipFile);

for (let i = 0; i < mapping.length / 2; i += 2) {
    console.log(mapping[i], ' ---> ', mapping[i + 1]);
    zip.addFile(mapping[i + 1], fs.readFileSync(mapping[i])); 
}

zip.writeZip(zipFile);

