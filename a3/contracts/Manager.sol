pragma solidity 0.5.1;
import "./Course.sol";

contract Manager {
	//Address of the school administrator
	address admin;

	mapping (address => int) student;
	mapping (address => bool) isStudent;
	mapping (int => bool) isCourse;
	mapping (int => Course) course;

	int rollCount = 19111000;

	//Constructor
	constructor() public {
		admin = msg.sender;
	}
	function getAdmin() public accessToAll returns (address) {
		return admin;
	}
	// function
	function kill() public accessToAdminOnly {
		//The admin has the right to kill the contract at any time.
		//Take care that no one else is able to kill the contract
		selfdestruct((address)((uint160)(admin)));
	}

	function addStudent() public accessToAll {
		//Anyone on the network can become a student if not one already
		//Remember to assign the new student a unique roll number
		address studentAddr = msg.sender;

		require(isStudent[studentAddr] == false);
		require(student[studentAddr] == 0);

		student[studentAddr] = rollCount++;
		isStudent[studentAddr] = true;
		return;
	}

	function addCourse(int courseNo, address instructor) public accessToAdminOnly {
		//Add a new course with course number as courseNo, and instructor at address instructor
		//Note that only the admin can add a new course. Also, don't create a new course if course already exists

		require (isCourse[courseNo] == false);
		require (course[courseNo] == Course(0));

		// Create a new Course Contract
        Course newCourse = new Course(courseNo, instructor, admin);

		course[courseNo] = newCourse;
		isCourse[courseNo] = true;
		// course[courseNo] = 
	}

	function regCourse(int courseNo) public accessToRegisteredStudentOnly(courseNo) {
		//Register the student in the course if he is a student on roll and the courseNo is valid

		address studentAddr = msg.sender;
		int rollNo = student[studentAddr];
		Course currCourse = course[courseNo];

		currCourse.enroll(rollNo);

		return;
	}

	function getMyMarks(int courseNo) public view accessToRegisteredStudentOnly(courseNo) returns(int, int, int)  {
		//Check the courseNo for validity
		//Should only work for valid students of that course
		//Returns a tuple (midsem, endsem, attendance)

		address studentAddr = msg.sender;
		int rollNo = student[studentAddr];
		
		Course currCourse = course[courseNo];

		int midsem = currCourse.getMidsemMarks(rollNo);
		int endsem = currCourse.getEndsemMarks(rollNo);
		int attendance = currCourse.getAttendance(rollNo);

		// TODO: What values to return in case of error?
		return (midsem, endsem, attendance);
	}

	function getMyRollNo() public view returns(int)  {
		//Utility function to help a student if he/she forgets the roll number
		//Should only work for valid students
		//Returns roll number as int
		
		// Check if the student has a roll number or not
		require(isStudent[msg.sender] == true);
		require(student[msg.sender] != 0);

		int rollNo = student[msg.sender];
		return rollNo;
	}
	modifier accessToRegisteredStudentOnly(int courseNo) {
		// Check if the student has a roll number or not
		require(isStudent[msg.sender] != false);
		require(student[msg.sender] != 0);

		// Check if the course exists
		require (isCourse[courseNo] == true);
		require (course[courseNo] != Course(0));

		_;
	}

	modifier accessToAll {
		require(true);
		_;
	}

	modifier accessToAdminOnly {
		require(msg.sender == admin);
		_;
	}

	//modifier accessToStudentOnly {
		//address studentAddr = msg.sender;
		//require(isStudent[studentAddr] == true);
		//require(student[studentAddr] != 0);
	//}

}
