#!/bin/node

const fsc = require("fs");
const ostream = fsc.createWriteStream(`${__dirname}/../build/compile_log`);
function summarize(err, stdout, stderr){
	let warnings = stderr.split(" warnings").slice(0, -1) || ''
	let errors = stderr.split(" errors").slice(0, -1) || '';

	warnings = warnings.join(" warnings").split('~').pop().trim();
	errors = errors.join(" errors").split('~').pop().trim();

	let filename = (stdout.split("\/\/")[0] || '');


	console.log(
		"\x1b[94m\x1b[97m", filename,
		"\n\x1b[0m  \x1b[93m " + (warnings || 0),
		"\n\x1b[0m  \x1b[91m " + (errors || 0),
		"\x1b[0m\n"
	);

	console.log([
		"\x1b[90m─".repeat(process.stdout.columns),
	].join('\n'))

	ostream.write(`\x1b[94m# ${filename}\n\x1b[0m`)
	ostream.write(stderr);
	ostream.write(`\n\x1b[94m# ----------------------\x1b[0m\n`)

}

const { exec } = require("child_process");
const fs = require("fs/promises");
new Promise(async function(res){
	res();

	const movup = process.env.MOVUP;
	if(movup){
		for(let i = 0; i < Number(movup); i++)
			process.stdout.write("\x1b[3A");
	}

	let entries = [
		...(await fs.readdir("op")).filter(f => f.endsWith(".c"))
	];

	console.log([
		"─".repeat(process.stdout.columns) + "\r",
		"	┤ 󰚗 \x1b[96mCompilation\x1b[0m Summary ├\n",
		//"─".repeat(process.stdout.columns)
	].join(''))

	entries.map(entry => exec(`echo ${entry} \/\/ &&
g++ -I./ \\
 -fdiagnostics-color=always \
 -L../SDL3 -L../SDL3_ttf -lSDL3_image -lSDL3_ttf -lSDL3 \\
 	\`pkg-config --cflags --libs sdl3 sdl3-image\` \\
 -Wall -Wextra -pedantic \\
 -c op/${entry.slice(0, -2)}.c \\
 -o op/${entry.slice(0, -2)}.o
	`, summarize));

	let macro = entries.map(f => "op_" + f.slice(0, -2) + '()').join('; ');
	macro = `#define OP_INCLUDE() ${macro}`
	entries = entries.map(function(file){
		return `void op_${file.slice(0, -2)}(void);`;
	});
	entries.push(macro);

	await fs.writeFile("op.h", entries
		.join('\n')
	);
})
