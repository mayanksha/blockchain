import express = require('express')
import cors = require('cors')
import httpLogger = require('morgan')
import bodyParser = require('body-parser')
import assert = require('assert')
import fs_lib = require('fs');

const fs = fs_lib.promises;

import { Request, Response, NextFunction } from 'express';

const app: express.Express = express();
const env_PORT = 8000; 

const ABI_Names = ['Candidate.json' , 'Manager.json' , 'Migrations.json' , 'Voter.json' , 'VoteTally.json'];
app.use(bodyParser.json())
app.use(cors())
app.use(httpLogger('dev'))

app.get('/abi/:filename', async function(req: Request, res: Response, next) {
  const flag = false;
  const fileName = req.params.filename;
  if (ABI_Names.includes(fileName)) {
    const file = await fs.readFile(__dirname + '/contracts/' + fileName);
    res.send(JSON.parse(file.toString()).abi);
    res.end();
  }
  else {
    res.sendStatus(404);
    res.end();
  }
})
app.use('/*', (err, req: Request, res: Response, next: NextFunction) => {
	// Assertions errors are wrong user inputs
  console.error(err);
	if(err.code === 'ERR_ASSERTION'){
		// Bad HTTP Request
		res.sendStatus(400);
	}
	else {
		// Internal Server Error 
		res.status(500);
		res.end('500 - INTERNAL SERVER ERROR!');
	}
});

app.listen(env_PORT, err => {
  if (err) {
    throw err
  }
  console.log('Server listening on ' + env_PORT)
})
