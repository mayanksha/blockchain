pragma solidity 0.5.1;

contract Course {
    
    address admin;
    address ManagerContract;
    address instructor;
    int courseNo;
    
    struct Marks{
        int midsem;
        int endsem;
        int attendance;
    }
    
    mapping (int => Marks) student;
    mapping (int => bool) isEnrolled;
    
    constructor(int c, address inst, address adm) public {
    	courseNo = c;
		instructor = inst;
		admin = adm;

		// Manager is the one calling constructor
		ManagerContract = msg.sender;
		return;
    }
    
	function kill() public accessToAdminOnly {
		//The admin has the right to kill the contract at any time.
		//Take care that no one else is able to kill the contract
		selfdestruct((address)((uint160)(admin)));
	}
    
    function enroll(int rollNo) public accessToManagerContract {
        //This function can only be called by the ManagerContract
        //Enroll a student in the course if not already registered
	
		// Assert that user is not enrolled
		require(isEnrolled[rollNo] == false);

		student[rollNo].midsem = 0;
		student[rollNo].endsem = 0;
		student[rollNo].attendance = 0;

		isEnrolled[rollNo] = true;
		return;
    }
    
    function markAttendance(int rollNo) public accessToInstructors {
        //Only the instructor can mark the attendance
        //Increment the attendance variable by one
        //Make sure the student is enrolled in the course

		// Assert that user is enrolled
		require(isEnrolled[rollNo] == true);


		student[rollNo].attendance += 1;
		return;
    }
    
    function addMidSemMarks(int rollNo, int marks) public accessToInstructors {
        //Only the instructor can add midsem marks
        //Make sure that the student is enrolled in the course

		// Assert that user is enrolled
		require(isEnrolled[rollNo] == true);

		// TODO: Check this condition
		//require(student[rollNo] != Marks(0));

		student[rollNo].midsem = marks;
		return;
    }
    
    function addEndSemMarks(int rollNo, int marks) public accessToInstructors{
        //Only the instructor can add endsem marks
        //Make sure that the student is enrolled in the course

		// Assert that user is enrolled
		require(isEnrolled[rollNo] == true);

		// TODO: Check this condition
		//require(student[rollNo] != Marks(0));

		student[rollNo].endsem = marks;
		return;
    }
    
    function getMidsemMarks(int rollNo) public view accessToManagerContract returns(int)  {
        //Can only be called by the ManagerContract
        //return the midSem, endSem and attendance of the student
        //Make sure to check the student is enrolled

		// Assert that user is enrolled
		require(isEnrolled[rollNo] == true);

		// TODO: Check this condition
		//require(student[rollNo] != Marks(0));
		return student[rollNo].midsem;
    }
    
    
    function getEndsemMarks(int rollNo) public view accessToManagerContract returns(int) {
        //Can only be called by the ManagerContract
        //return the midSem, endSem and attendance of the student
        //Make sure to check the student is enrolled

		// Assert that user is enrolled
		require(isEnrolled[rollNo] == true);

		// TODO: Check this condition
		//require(student[rollNo] != Marks(0));
		return student[rollNo].endsem;
    }
    
    
    function getAttendance(int rollNo) public view accessToManagerContract returns(int) {
        //Can only be called by the ManagerContract
        //return the midSem, endSem and attendance of the student
        //Make sure to check the student is enrolled

		// Assert that user is enrolled
		require(isEnrolled[rollNo] == true);

		// TODO: Check this condition
		//require(student[rollNo] != Marks(0));
		return student[rollNo].attendance;
    }
    
    function isEnroll(int rollNo) public view accessToAll returns(bool)  {
        //Returns if a roll no. is enrolled in a particular course or not
        //Can be accessed by anyone

		// Assert that user is enrolled
		bool c1 = isEnrolled[rollNo];
// 		bool c2 = student[rollNo];
		return c1;
    }
    //modifier studentRegisteredAnd
	modifier accessToAll {
		require(true);
		_;
	}

	modifier accessToAdminOnly {
		require(msg.sender == admin);
		_;
	}

	// TODO: Should admins be allowed access?
	modifier accessToInstructors {
		bool isAdmin = (msg.sender == admin);
		bool isInstructor = (msg.sender == instructor);
		require(isAdmin || isInstructor);
		_;
	}
	modifier accessToManagerContract {
        require(msg.sender == ManagerContract);
        _;
	}
}
