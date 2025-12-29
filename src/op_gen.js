#!/bin/node

const fs = require("fs/promises");
new Promise(async function(res){
	res();

	let entries = [
		...(await fs.readdir("op")).filter(f => f.endsWith(".c"))
	];

	let macro = entries.map(f => "op_" + f.slice(0, -2) + '()').join(' ');
	macro = `#define OP_INCLUDE() ${macro}`
	entries = entries.map(function(file){ return `#include "op/${file}"`; });
	entries.push(macro);

	await fs.writeFile("op.h", entries
		.join('\n')
	);
})
