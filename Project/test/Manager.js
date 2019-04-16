const Manager = artifacts.require('Manager');
const Voter = artifacts.require('Voter');
const VoteTally = artifacts.require('VoteTally');
const Candidate = artifacts.require('Candidate');
const assert = require('assert');

let courseAddress = [];

contract("Manager", accounts => {
  const head = accounts[0];

  const v = {
    first: accounts[1],
    second: accounts[2],
    third: accounts[3],
  };

  const c = {
    first: accounts[4],
    second: accounts[5],
    third: accounts[6],
  };

  const outsider = {
    first: accounts[7],
    second: accounts[8],
    third: accounts[9],
  };

  const courses = {
    first: 730,
    second: 731,
    third: 732,
  };
  console.log(c);
  console.log(v);
  const mani = "FSOLDJFLKSJFDLKSJLVJLDJVL";
  const cred = "skjd fhdksh fkshsd fs djfl jdsljlsfjl";
  it("Should print address of the deployed contract", () => 
    Manager.deployed()
    .then((instance) => {
      console.log(instance.contract._address);
      console.log(instance.constructor.class_defaults.from);
    })
    .catch(console.error)
  );

  it("Should create a candidate", () => 
    Manager.deployed()
    .then((instance) => {
      const prop = [outsider.first];
      const sec = [outsider.second, outsider.third];
      return instance.createCandidate(
        c.first,
        "Foobar",
        1623423,
        prop,
        sec,
        mani,
        cred,
        {from : head});
    })
    .then((val) => {
      console.log(val.tx + "\n" + val.receipt.transactionHash);
    })
    .catch((err) => {
      throw err;
    })
  );


  const allCand = [];
  it("Should get all cand.", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.getAllCandidates(
        {from : head});
    })
    .then((val) => {
      console.log(val);
      allCand = val;
    })
    .catch((err) => {
      throw err;
    })
  );
  it("Should create a voter", () => 
    Manager.deployed()
    .then((instance) => {
      const prop = [outsider.first];
      const sec = [outsider.second, outsider.third];
      return instance.createVoter(
        v.first,
        "Foobar-1",
        "Foobar_Papa-1",
        "Foobar_Mama-1",
        "Hall 5",
        160392,
        Date.now(),
        {from : head});
    })
    .then((val) => {
      console.log(val.tx + "\n" + val.receipt.transactionHash);
    })
    .catch((err) => {
      throw err;
    })
  );
  it("Should create a voter 2", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.createVoter(
        v.second,
        "Foobar-2",
        "Foobar_Papa-2",
        "Foobar_Mama-2",
        "Hall 5",
        160234,
        Date.now(),
        {from : head});
    })
    .then((val) => {
      console.log(val.tx + "\n" + val.receipt.transactionHash);
    })
    .catch((err) => {
      throw err;
    })
  );

  it("Should create a voter 3", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.createVoter(
        v.third,
        "Foobar-3",
        "Foobar_Papa-3",
        "Foobar_Mama-3",
        "Hall 5",
        160399,
        Date.now(),
        {from : head});
    })
    .then((val) => {
      console.log(val.tx + "\n" + val.receipt.transactionHash);
    })
    .catch((err) => {
      throw err;
    })
  );

  it("Should cast a vote by voter 1 from Head", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.castVote(
        v.first,
        c.first,
        {from : head});
    })
    .then((val) => {
      console.log(val.tx + "\n" + val.receipt.transactionHash);
    })
    .catch((err) => {
      throw err;
    })
  );
  it("Should cast a vote by voter 2 from Head", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.castVote(
        v.second,
        c.second,
        {from : head});
    })
    .then((val) => {
      console.log(val.tx + "\n" + val.receipt.transactionHash);
    })
    .catch((err) => {
      throw err;
    })
  );
  it("Should get Candidate 1's votes from Head", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.countVotes.call(
        c.first,
        {from : head});
    })
    .then((val) => {
      console.log(val);
      /*console.log(val.tx + "\n" + val.receipt.transactionHash);*/
    })
    .catch((err) => {
      throw err;
    })
  );

  it("Should get Candidate 2's votes from Head", () => 
    Manager.deployed()
    .then((instance) => {
      return instance.countVotes.call(
        c.second,
        {from : head});
    })
    .then((val) => {
      console.log(val);
      /*console.log(val.tx + "\n" + val.receipt.transactionHash);*/
    })
    .catch((err) => {
      throw err;
    })
  );
  /*it("Should cast a vote by voter 1 from Head", () => 
   *  Manager.deployed()
   *  .then((instance) => {
   *    return instance.castVote(
   *      v.first,
   *      c.first,
   *      {from : head});
   *  })
   *  .then((val) => {
   *    console.log(val);
   *  })
   *  .catch((err) => {
   *    throw err;
   *  })
   *);
   *it("Should cast a vote by voter 2 from Head", () => 
   *  Manager.deployed()
   *  .then((instance) => {
   *    return instance.castVote(
   *      v.first,
   *      c.second,
   *      {from : head});
   *  })
   *  .then((val) => {
   *    console.log(val);
   *  })
   *  .catch((err) => {
   *    throw err;
   *  })
   *);*/
  /*  it("Should print address of the deployed contract", () => 
   *    Manager.deployed()
   *    .then((instance) => {
   *      console.log(instance.contract._address);
   *      console.log(instance.constructor.class_defaults.from);
   *    })
   *    .catch(console.error)
   *  );
   *
   *  it("Should getAdmin()", () => 
   *    Manager.deployed()
   *    .then((instance) => instance.getAdmin.call())
   *    .then(adminString => console.log(adminString))
   *    .catch(err => {
   *      throw err;	
   *    })
   *  );
   *
   *  it("getMyRollNo() should throw an error since a student isn't present", () => {
   *    return Manager.deployed()
   *      .then((instance) => instance.getMyRollNo())
   *      .catch(err => {
   *        assert(err instanceof Error);
   *      })
   *  }
   *  );
   *
   *  it("getMyMarks(rollCount) should throw an error since a student isn't present", () => {
   *    return Manager.deployed()
   *      .then((instance) => instance.getMyMarks.call(rollCount))
   *      .catch(err => {
   *        assert(err instanceof Error);
   *      })
   *  });
   *
   *  it("addStudent() should add a student with roll Number ", () => {
   *    return Manager.deployed()
   *      .then((instance) => instance.addStudent({
   *        from: students.first
   *      }))
   *      .then(response => {
   *        return response;
   *      })
   *      .catch(err => {
   *        throw err;	
   *      })
   *  }
   *  );
   *
   *  it("getMyRollNo() should return rollCount", () => {
   *    return Manager.deployed()
   *      .then((instance) => instance.getMyRollNo.call({
   *        from: students.first
   *      }))
   *      .then(response => {
   *        console.log(response.toString());
   *        assert(response.toString() === (rollCount).toString());
   *      })
   *      .catch(err => {
   *        throw err;
   *      })
   *  }
   *  );
   *  it("addStudent() should add 2nd student with roll Number ", () => {
   *    return Manager.deployed()
   *      .then((instance) => instance.addStudent({
   *        from: students.second
   *      }))
   *      .then(response => {
   *        return response;
   *      })
  *      .catch(err => {
      *        throw err;	
      *      })
    *  }
*  );
*
  *  it("getMyRollNo() should return rollCount + 1", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.getMyRollNo.call({
            *        from: students.second
            *      }))
      *      .then(response => {
          *        console.log(response.toString());
          *        assert(response.toString() === (rollCount + 1).toString());
          *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  }
      *  );
*  it("addStudent() should add a student with roll Number ", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.addStudent({
          *        from: students.third
          *      }))
    *      .then(response => {
        *        return response;
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*
  *  it("getMyRollNo() should return rollCount + 2", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.getMyRollNo.call({
            *        from: students.third
            *      }))
      *      .then(response => {
          *        console.log(response.toString());
          *        assert(response.toString() === (rollCount + 2).toString());
          *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  }
      *  );
*
  *  it("addCourse() called by non-admin should throw error", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.addCourse(courses.first, instructors.first, {
            *        from: instructors.first
            *      }))
      *      .then(response => assert(response != null))
      *      .catch(err => {
          *        // Check reverse condition so as to show error
          *        [>assert((err instanceof Error) === false);<]
          *      })
      *  }
      *  );
*
  *  it("addCourse() called by non-admin should throw error", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.addCourse(courses.first, instructors.first, {
            *        from: students.third 
            *      }))
      *      .then(response => assert(response != null))
      *      .catch(err => {
          *        // Check reverse condition so as to show error
          *        [>assert((err instanceof Error) === false);<]
          *      })
      *  }
      *  );
*
  *  it("getMyRollNo() should return rollCount", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.getMyRollNo.call({
            *        from: students.first
            *      }))
      *      .then(response => {
          *        console.log(response.toString());
          *        assert(response.toString() === (rollCount).toString());
          *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  });
*
  *  it("addCourse() called from admin should add course with ID - 730", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.addCourse(courses.first, instructors.first, {
            *        from: owner
            *      }))
      *      .then(response => { 
          *        addCoursePromises.push(response);
          *        assert (response != null);
          *        return response;
          *      })
      *      .catch(err => {
          *        throw err;	
          *      })
      *  }
      *  );
*
  *  it("addCourse() called again from admin with same instructor should throw error", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.addCourse(courses.first, instructors.first, {
            *        from: owner
            *      }))
      *      .then(response => {
          *        assert(false);
          *      })
      *      .catch(err => {
          *        [>throw err;	<]
          *        assert(err instanceof Error);
          *      })
      *  }
      *  );
*  it("addCourse() called again from admin with different instructor should throw error", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.addCourse(courses.first, instructors.second, {
          *        from: owner
          *      }))
    *      .then(response => {
        *        assert(false);
        *      })
    *      .catch(err => {
        *        [>throw err;	<]
        *        assert(err instanceof Error);
        *      })
    *  }
    *  );
*
  *  it("addCourse() called from admin should add course with ID - 731", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.addCourse(courses.second, instructors.second, {
            *        from: owner
            *      }))
      *      .then(response => { 
          *        addCoursePromises.push(response);
          *        assert (response != null);
          *        return response;
          *      })
      *      .catch(err => {
          *        throw err;	
          *      })
      *  }
      *  );
*  it("regCourse() called from non registered person (outsider) should throw error", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.first, {
          *        from: outsider.first 
          *      }))
    *      .then(response => {
        *        assert(false);
        *      })
    *      .catch(err => {
        *        [>throw err;	<]
        *        assert(err instanceof Error);
        *      })
    *  }
    *  );
*  it("regCourse() called from owner (non registered) should throw error", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.first, {
          *        from: owner 
          *      }))
    *      .then(response => {
        *        assert(false);
        *      })
    *      .catch(err => {
        *        [>throw err;	<]
        *        assert(err instanceof Error);
        *      })
    *  }
    *  );
*
  *  it("regCourse() called from course instructor should throw error", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.regCourse(courses.first, {
            *        from: instructors.first 
            *      }))
      *      .then(response => {
          *        assert(false);
          *      })
      *      .catch(err => {
          *        [>throw err;	<]
          *        assert(err instanceof Error);
          *      })
      *  }
      *  );
*  it("regCourse() called from registered first student OK - 730", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.first, {
          *        from: students.first 
          *      }))
    *      .then(response => {
        *        assert(response != null);
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*  it("regCourse() called from registered first student again should throw error", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.first, {
          *        from: students.first 
          *      }))
    *      .then(response => {
        *        assert(false);
        *      })
    *      .catch(err => {
        *        assert(err instanceof Error);
        *      })
    *  }
    *  );
*  it("regCourse() called from registered second student OK - 730", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.first, {
          *        from: students.second 
          *      }))
    *      .then(response => {
        *        assert(response != null);
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*  it("regCourse() called from registered third student OK - 730", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.first, {
          *        from: students.third 
          *      }))
    *      .then(response => {
        *        assert(response != null);
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*
  *
  *  it("regCourse() called from registered first student for a course not added should FAIL", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.regCourse(courses.first, {
            *        from: students.first 
            *      }))
      *      .then(response => {
          *        assert(false);
          *      })
      *      .catch(err => {
          *        assert(err instanceof Error);
          *      })
      *  }
      *  );
*  it("addCourse() called from admin should add course with ID - 732", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.addCourse(courses.third, instructors.third, {
          *        from: owner
          *      }))
    *      .then(response => { 
        *        addCoursePromises.push(response);
        *        assert (response != null);
        *        return response;
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*  it("regCourse() called from registered first student OK - 732", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.third, {
          *        from: students.first 
          *      }))
    *      .then(response => {
        *        assert(response != null);
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*  it("regCourse() called from registered third student OK - 732", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.regCourse(courses.third, {
          *        from: students.third 
          *      }))
    *      .then(response => {
        *        assert(response != null);
        *      })
    *      .catch(err => {
        *        throw err;	
        *      })
    *  }
    *  );
*
  *  it("get the contract addresses of all Course contracts", () => {
      *    let instance;
      *    return Manager.deployed()
      *      .then(inst => {
          *        instance = inst;
          *      })
      *      .then(() => addCoursePromises)
      *      .then((arr) => Promise.all(arr))
      *      .then((resolves) => {
          *        let allPromises = [];
          *        for(let i = 730; i <= 732; i++) {
          *          allPromises.push(instance.getCourseAddress.call(i));
          *        }
          *        return Promise.all(allPromises);
          *      })
      *      .then(arr => {
          *        arr.forEach(e => {
              *          courseAddress.push(e);
              *        });
          *      })
      *      .catch(err => {
          *        throw err;	
          *      })
      *  });
*
  *
  *
  *  // These will be assigned the Course contracts later
  *  let course1; 
*  let course2; 
*  let course3;
*
  *  it("Should log addresses of all Course Contracts", async function () {
      *    course1 = await Course.at(courseAddress[0]);
      *    course2 = await Course.at(courseAddress[1]);
      *    course3 = await Course.at(courseAddress[2]);
      *  });
*
  *  it("getMyMarks(rollCount) should return 0, 0, 0 and be OK", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.getMyMarks.call(courses.first, {
            *        from: students.first
            *      }))
      *      .then((val) => {
          *        console.log(val);
          *        assert((0).toString() === val[0].toString());
          *        assert((0).toString() === val[1].toString());
          *        assert((0).toString() === val[2].toString());
          *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  });
*  it("Mark attendance for first student by student himself for 730 - FAIL", () => {
    *    return course1.markAttendance(students.first, {
        *      from: students.first
        *    }).then(response => {
          *        assert(false);
          *      })
    *      .catch(err => {
        *        assert(err instanceof Error);
        *      })
    *  });
*
  *  it("Mark attendance for first student by instructor for 730 - OK", () => {
      *    return course1.markAttendance(rollCount, {
          *      from: instructors.first
          *    }).then(response => { 
            *        assert (response != null);
            *        return response;
            *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  });
*
  *  it("Mark attendance for first student by instructor for 730 - OK", () => {
      *    return course1.markAttendance(rollCount, {
          *      from: instructors.first
          *    }).then(response => { 
            *        assert (response != null);
            *        return response;
            *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  });
*  it("Mark attendance for first student by instructor for 730 - OK", () => {
    *    return course1.markAttendance(rollCount, {
        *      from: instructors.first
        *    }).then(response => { 
          *        assert (response != null);
          *        return response;
          *      })
    *      .catch(err => {
        *        throw err;
        *      })
    *  });
*  it("Add Midsem Marks for first student by instructor for 730 - OK", () => {
    *    return course1.addMidSemMarks(rollCount, 88, {
        *      from: instructors.first
        *    }).then(response => { 
          *        assert (response != null);
          *        return response;
          *      })
    *      .catch(err => {
        *        throw err;
        *      })
    *  });
*  it("Add Endsem Marks for first student by instructor for 730 - OK", () => {
    *    return course1.addEndSemMarks(rollCount, 77, {
        *      from: instructors.first
        *    }).then(response => { 
          *        assert (response != null);
          *        return response;
          *      })
    *      .catch(err => {
        *        throw err;
        *      })
    *  });
*
  *  it("getMyMarks(rollCount) should be OK", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.getMyMarks.call(courses.first, {
            *        from: students.first
            *      }))
      *      .then((val) => {
          *        console.log(val);
          *        assert((88).toString() === val[0].toString());
          *        assert((77).toString() === val[1].toString());
          *        assert((3).toString() === val[2].toString());
          *      })
      *      .catch(err => {
          *        throw err;
          *      })
      *  });
*
  *  it("kill(rollCount) by others should FAIL", () => {
      *    return Manager.deployed()
      *      .then((instance) => instance.kill({
            *        from: students.first
            *      }))
      *      .catch(err => {
          *        assert(err instanceof Error);
          *      })
      *  });
*  it("kill(rollCount) by admin should OK", () => {
    *    return Manager.deployed()
    *      .then((instance) => instance.kill({
          *        from: owner 
          *      }))
    *      .then((val) => {
        *        console.log(val);
        *      })
    *      .catch(err => {
        *        throw err;
        *      })
    *  });*/
})
