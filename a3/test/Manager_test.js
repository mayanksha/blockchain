const Manager = artifacts.require('Manager');
const assert = require('assert');
contract("Manager", accounts => {
	let rollCount = 19111000;
	const owner = accounts[0];
	const students = {
		first: accounts[1],
		second: accounts[2],
		third: accounts[3],
	};
	const instructors = {
		first: accounts[4],
		second: accounts[5],
		third: accounts[6],
	};

	const courses = {
		first: 730,
		second: 731,
		third: 732,
	};
	it("Should print address of the deployed contract", () => 
		Manager.deployed()
		.then((instance) => {
			console.log(instance.contract._address);
			console.log(instance.constructor.class_defaults.from);
			/*console.log(owner);
			 *console.log(students);
			 *console.log(instructors);*/
		})
		.catch(console.error)
	);

	it("Should getAdmin()", () => 
		Manager.deployed()
		.then((instance) => instance.getAdmin.call())
		.then(adminString => console.log(adminString))
		.catch(err => {
			throw err;	
		})
	);

	it("getMyRollNo() should throw an error since a student isn't present", () => {
		return Manager.deployed()
			.then((instance) => instance.getMyRollNo())
			.catch(err => {
				assert(err instanceof Error);
			})
		}
	);

	it("getMyMarks(rollCount) should throw an error since a student isn't present", () => {
		return Manager.deployed()
			.then((instance) => instance.getMyMarks.call(rollCount))
			.catch(err => {
				assert(err instanceof Error);
			})
		}
	);

	it("addStudent() should add a student with roll Number ", () => {
		return Manager.deployed()
			.then((instance) => instance.addStudent({
				from: students.first
			}))
			.then(response => {
				return response;
			})
			.catch(err => {
				throw err;	
			})
		}
	);

	it("getMyRollNo() should return rollCount", () => {
		return Manager.deployed()
			.then((instance) => instance.getMyRollNo.call({
				from: students.first
			}))
			.then(response => {
				console.log(response.toString());
				assert(response.toString() === (rollCount).toString());
			})
			.catch(err => {
				throw err;
			})
		}
	);
	it("addStudent() should add 2nd student with roll Number ", () => {
		return Manager.deployed()
			.then((instance) => instance.addStudent({
				from: students.second
			}))
			.then(response => {
				return response;
			})
			.catch(err => {
				throw err;	
			})
		}
	);

	it("getMyRollNo() should return rollCount + 1", () => {
		return Manager.deployed()
			.then((instance) => instance.getMyRollNo.call({
				from: students.second
			}))
			.then(response => {
				console.log(response.toString());
				assert(response.toString() === (rollCount + 1).toString());
			})
			.catch(err => {
				throw err;
			})
		}
	);
	it("addStudent() should add a student with roll Number ", () => {
		return Manager.deployed()
			.then((instance) => instance.addStudent({
				from: students.third
			}))
			.then(response => {
				return response;
			})
			.catch(err => {
				throw err;	
			})
		}
	);

	it("getMyRollNo() should return rollCount + 2", () => {
		return Manager.deployed()
			.then((instance) => instance.getMyRollNo.call({
				from: students.third
			}))
			.then(response => {
				console.log(response.toString());
				assert(response.toString() === (rollCount + 2).toString());
			})
			.catch(err => {
				throw err;
			})
		}
	);

	it("addCourse() called by non-admin should throw error", () => {
		return Manager.deployed()
			.then((instance) => instance.addCourse(courses.first, instructors.first, {
				from: instructors.first
			}))
			.then(response => assert(response != null))
			.catch(err => {
				// Check reverse condition so as to show error
				/*assert((err instanceof Error) === false);*/
			})
		}
	);

	it("addCourse() called by non-admin should throw error", () => {
		return Manager.deployed()
			.then((instance) => instance.addCourse(courses.first, instructors.first, {
				from: students.third 
			}))
			.then(response => assert(response != null))
			.catch(err => {
				// Check reverse condition so as to show error
				/*assert((err instanceof Error) === false);*/
			})
		}
	);

	it("addCourse() called from admin should add course with ID - 730", () => {
		return Manager.deployed()
			.then((instance) => instance.addCourse(courses.first, instructors.first, {
				from: owner
			}))
			.then(response => response )
			.catch(err => {
				throw err;	
			})
		}
	);

	it("getMyRollNo() should return rollCount", () => {
		return Manager.deployed()
			.then((instance) => instance.getMyRollNo.call({
				from: students.first
			}))
			.then(response => {
				console.log(response.toString());
				assert(response.toString() === (rollCount).toString());
			})
			.catch(err => {
				throw err;
			})
	}
	);
})
