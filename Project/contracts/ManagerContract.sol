pragma solidity ^0.5.0;
import "./CandidateContract.sol";
import "./VoterContract.sol";
import "./VoteTallyContract.sol";

contract Manager {
    address public owner;

    enum AUTH_LEVELS {
        NO_PRIVILEGE,
        CAN_VIEW,
        CAN_CREATE,
        CAN_CAST_VOTE,
        ALL_PRIVILEGES
    }

    // Mapping between the RollNo and Voter
    mapping (address => Voter) addressVoterMap;
    // Mapping between wallet of user and Voter
    mapping (uint256 => Voter) rollNoVoterMap;

    // Mapping from person's address to enum AUTH_LEVELS
    mapping (address => int16) public ECAuthorisedPeople;


    address voteTallyContractAddress;
    // TODO: 1. set head of EC 
    constructor() public {
        owner = msg.sender;
        VoteTally newTallyContract = new VoteTally();
    }

    function createVoter(
        address personAddr,
        string memory Name,
        string memory FathersName,
        string memory MothersName,
        string memory HallOfResidence,
        uint256 RollNumber,
        int DOBTimestamp
    ) public {
        //Anyone on the network can become a Voter if not one already

		require (rollNoVoterMap[RollNumber] == Voter(0));

        Voter newVoter = new Voter(
            msg.sender,
            Name,
            FathersName,
            MothersName,
            HallOfResidence,
            RollNumber,
            DOBTimestamp
        );
    }

    function getVoterDetails(uint256 RollNo) public returns (Voter) {

        // Can be the address of the Voter or of someone within EC with adequate privileges
        address personAddr = msg.sender;
        
        // In the case the person provides some RollNo, you gotta check his Roll No
        if (RollNo == 0) {
            //  Check that the person has actually registered as a voter or not
            require (addressVoterMap[personAddr] != Voter(0));
            return addressVoterMap[personAddr].getSelfContract();
        }

        // This case will happen when this fn is called by someone from EC
        else {
            // Check Auth level
            require (
                ECAuthorisedPeople[personAddr] != 0 && 
                ECAuthorisedPeople[personAddr] >= (int16)( AUTH_LEVELS.CAN_VIEW )
            );
            // Check RollNo exists as Voter or not
            require (rollNoVoterMap[RollNo] !=  Voter(0));
            return rollNoVoterMap[RollNo].getSelfContract();
        }
    }

    // Roll No is immutable?
    //function updateVotersDetails(int256 RollNo) public {
        //// TODO: Finish this

        //// Can be the address of the Voter or of someone within EC with adequate privileges
        //address personAddr = msg.sender;
        
        //// In the case the person provides some RollNo, you gotta check his Roll No
        //if (!RollNo) {
            ////  Check that the person has actually registered as a voter or not
            //require (addressVoterMap[personAddr] != Voter(0));
            //addressVoterMap[personAddr].updateAllDetails();
        //}

        //// This case will happen when this fn is called by someone from EC
        //else {
            //// Check Auth level
            //require (
                //ECAuthorisedPeople[personAddr] != 0 && 
                //ECAuthorisedPeople[personAddr] >= AUTH_LEVELS.CAN_VIEW
            //);
            //// Check RollNo exists as Voter or not
            //require (rollNoVoterMap[RollNo]  != Voter(0));
            //rollNoVoterMap[RollNo].getSelfContract();
        //}
        
    //}

    function registerFingerprintData() public {

    }

    function updateFingerprintData() public {

    }

    function createCandidate() public {

    }

    function updateCandidateDetails() public {

    }

    function castVote() public {

    }

    function countVotes() public {

    }

    function verifyVotes() public {

    }

    function getCandidatePenalties() public {

    }

    // Should this function be moved to CandidateContract?
    function getCandidateManifesto() public {

    }

    function penalizeCandiate() public {

    }

    function dismissCandidateNomination() public {

    }

    function addProposerForCandidate() public {

    }

    function addSeconderForCandidate() public {

    }

    function verifyProposerIsValidPerson() public {

    }

    function verifySeconderIsValidPerson() public {

    }

    // Change the owner to a new head of EC
    function transferOwnership() public {

    }

    function registerVoteCastingComputer() public {

    }

    // All the access modifiers here
    modifier accessToAdminOnly {
        require(msg.sender == owner);
        _;
    }

    //modifier accessToVoterOnly {

    //}

    //modifier accessToCandidateOnly {

    //}

    //modifier accessToHeadOnly {

    //}

    //modifier accessToRegisteredComputersOnly {

    //}

    //// Should have access only to register people, and update their details
    //// upon their fingerprint verification
    //modifier accessToECWorkersOnly {
        
    //}
    //modifier PersonAndECOnly {
  
    //}
	function kill() public accessToAdminOnly {
		//The owner has the right to kill the contract at any time.
		//Take care that no one else is able to kill the contract
		selfdestruct((address)((uint160)(owner)));
	}
}
