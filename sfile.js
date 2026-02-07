const fsc = require("fs");
const fs = require("fs/promises");
const out = fsc.createWriteStream("sfile.txt");
const ignore = [ ".git", "logfile", "SDL" ];
const ignore_ext = [ ".o", ".ttf", ".gif", ".png", ".jpg" ]

async function readdir(path){
	console.log(path);
	for(let meta of await fs.readdir(path, { withFileTypes: true })){
		if(ignore.includes(meta.name)){
			continue ;
		}

		//console.log(meta);
		if(ignore_ext.filter(ext => meta.name.endsWith(ext))[0]){
			continue ;
		}

		if(meta.isDirectory()){
			await readdir(`${path}/${meta.name}`);

			continue ;
		}

		if(!meta.isFile())
			continue ;

		const file = await fs.readFile(`${path}/${meta.name}`);
		out.write(`\n-- ${path}/${meta.name} -- \n`
			+ `${file.toString()}\n\n`
		);
	}
}

readdir("src");
readdir("lorecraft");
