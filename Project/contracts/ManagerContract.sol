pragma solidity ^0.5.0;
import "./CandidateContract.sol";
import "./VoterContract.sol";
import "./VoteTallyContract.sol";

contract Manager {
    address public owner;
    address voteTallyAddress;
    int temp;
    enum AUTH_LEVELS {
        NO_PRIVILEGE,
        CAN_CAST_VOTE,
        CAN_VIEW,
        CAN_CREATE_VOTER,
        CAN_CREATE_CANDIDATE,
        CAN_TRANSFER_OWNERSHIP,
        ALL_PRIVILEGES
    }

    enum VOTE_STATUS {
        UNKNOWN,
        NOT_CASTED,
        CASTED
    }

    // Mapping between the RollNo and Voter
    mapping (address => Voter) addressVoterMap;
    // Mapping between wallet of user and Voter
    mapping (uint256 => Voter) rollNoVoterMap;

    mapping (address => uint) voteTracker;

    // Mapping between the RollNo and Candidate 
    mapping (address => Candidate) candidateMap;
    address[] allCandidatesAddress;

    // Mapping from person's address to enum AUTH_LEVELS
    mapping (address => int16) public ECAuthorisedPeople;


    address voteTallyContractAddress;
    // TODO: 1. set head of EC 
    constructor() public {
        owner = msg.sender;
        ECAuthorisedPeople[owner] = (int16) (AUTH_LEVELS.ALL_PRIVILEGES);
        VoteTally newTallyContract = new VoteTally(owner);
        voteTallyAddress = newTallyContract.getSelfAddress();
    }

    function createVoter(
        address personAddr,
        string memory Name,
        string memory FathersName,
        string memory MothersName,
        string memory HallOfResidence,
        uint256 RollNo,
        int DOBTimestamp
    ) public returns (bool) {
        //Anyone on the network can become a Voter if not one already
        address accessor = msg.sender;
        require(
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.CAN_CREATE_VOTER) || 
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.ALL_PRIVILEGES)
        );

        if (rollNoVoterMap[RollNo] != Voter(0) || addressVoterMap[personAddr] != Voter(0))
            require(false); 

        Voter newVoter = new Voter(
            personAddr,
            Name,
            FathersName,
            MothersName,
            HallOfResidence,
            RollNo,
            DOBTimestamp
        );
        address temp = newVoter.getSelfAddress();
        addressVoterMap[personAddr] = newVoter;
        rollNoVoterMap[RollNo] = newVoter;
        voteTracker[personAddr] = (uint) ( VOTE_STATUS.NOT_CASTED );
        return true;
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

    function createCandidate(
        // The person who's being created as candidate
        address personAddr,
        string memory Name,
        int RollNo,
        address[] memory prop,
        address[] memory sec,
        string memory manifesto,
        string memory credentials
    ) public returns (bool) {
        // The one accessing the contract
        address accessor = msg.sender;
        require(
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.CAN_CREATE_CANDIDATE) || 
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.ALL_PRIVILEGES)
        );
        require(candidateMap[personAddr] == Candidate( 0 ));

        Candidate newCandidate = new Candidate(
            personAddr,
            Name,
            RollNo,
            prop,
            sec,
            manifesto,
            credentials
        );
        address temp = newCandidate.getSelfAddress();

        candidateMap[temp] = newCandidate;
        allCandidatesAddress.push(temp);
        return true;
    }

    function updateCandidateDetails() public {

    }

    function setTemp(int _temp) public {
        address accessor = msg.sender;
        require(
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.CAN_CREATE_CANDIDATE) || 
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.ALL_PRIVILEGES)
        );
        temp = _temp;
    }

    function getTemp() public returns (int) {
        return temp;
    }
    function castVote(address personAddr, address candidateAddr) public returns (bool) {
        address accessor = msg.sender;
        require(
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.CAN_CAST_VOTE) || 
            ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.ALL_PRIVILEGES)
        );
        require (addressVoterMap[personAddr] != Voter(0));
        if (voteTracker[personAddr] != (uint)(VOTE_STATUS.NOT_CASTED)) {
            return (false);
        } else {
            VoteTally VTInstance = VoteTally(voteTallyAddress);
            bool temp = VTInstance.castVote(candidateAddr);
            voteTracker[personAddr] = (uint)(VOTE_STATUS.CASTED);
            return temp;
        }
    }

    function countVotes(address candidateAddr) public view returns (uint) {
        address accessor = msg.sender;
        // Allow only the head to count someone's votes
        require(ECAuthorisedPeople[accessor] == (int16)(AUTH_LEVELS.ALL_PRIVILEGES));
        VoteTally VTInstance = VoteTally(voteTallyAddress);
        return VTInstance.getVotesOfCandidate(candidateAddr);
    }

    function verifyVotes() public {

    }

    function getAllCandidates() public view returns (address[] memory) {
        return allCandidatesAddress;
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
