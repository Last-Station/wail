const fs = require("fs/promises");
new Promise(async function(res){
	res();

	function get_file_todos(todo){
		return todo.toString().split('\n')
			.map((line, i) => `\x1b[90m${i}\x1b[0m|${line}`)
			.filter(line => line.includes("TODO:"))
			.map(line => [
				line.split('|')[0],
				line.split("TODO:")[1]
			]);
	}

	async function rdir(path, on_todo){
		const list = await fs.readdir(path, { withFileTypes: true });
		for(const file of list){
			if(file.name == '.' || file.name == '..')
				continue ;

			if(file.isDirectory()){
				await rdir(path + '/' + file.name, on_todo);

				continue ;
			}

			if(!file.isFile())
				continue ;

			const data = await fs.readFile(path + '/' + file.name);
			await on_todo(get_file_todos(data),

				"\x1b[90m" + path + '/\x1b[0m' + file.name
			);
		}
	}

	process.stdout
		.write("\n" + ("─".repeat(process.stdout.columns))
			+ "\x1b[0m\r"
		)
	;
	console.log("	┤ 󱛣 \x1b[93mToDo(s)\x1b[0m Listings ├");
	//console.log("\x1b[90m─".repeat(process.stdout.columns), "\x1b[0m");
	const todos = [];
	async function todo(todos, filename){
		if(!todos[0])
			return ;

		const nodesc = " \x1b[90m(no description provided)\x1b[0m"
		console.log(`\x1b[92m \x1b[97m${filename}\x1b[0m`);
		console.log((await Promise.all(todos)).map(todo =>
			` \x1b[95m\x1b[0m ${todo[0]}`
			+ `${todo[1] || nodesc}\x1b[0m`
		).join('\n'));
		console.log("\x1b[90m─".repeat(process.stdout.columns),
			"\x1b[0m"
		);
	}
	await rdir(__dirname + '/src', todo);
	await rdir(__dirname + '/lorecraft', todo);

	console.log("");
})
